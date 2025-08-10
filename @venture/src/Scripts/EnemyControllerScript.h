#pragma once
#include <Flux.h>
#include "GameManagerScript.h"

class EnemyControllerScript : public Flux::IScript
{
    static EnemyControllerScript* s_Instance;

    //-------------------------------------------//
    //----------------Variables------------------//
    //-------------------------------------------//

private:
    
    enum class Direction { e_Left, e_Right };

    float m_AnimationTime = 0.0f;
    int m_CurrentFrame = 0;
    Direction m_LastTargetDirection = Direction::e_Left;

    struct Animation 
    {
        std::vector<Flux::LayeredFrame> frames;
        float frameDuration;
    };

    std::unordered_map<std::string, Animation> m_Animations;
    Animation* m_ActiveAnimation = nullptr;

public:

    enum class EnemyState 
    {
        e_CutsceneStart,
        e_Targeting,
        e_WaitingForPlayer,
        e_TargetGetHit,
        e_Attacking,
        e_CutsceneEnd
    };
    EnemyState e_BossEnemyFightState;

    int m_BossEnemyHP = 4;
    int m_BossEnemyPhase = 1;

    EnemyState e_PreviousState = EnemyState::e_CutsceneStart;

    //-------------------------------------------//
    //----------------Functions------------------//
    //-------------------------------------------//

public:

    static EnemyControllerScript* Get()
    {
        return s_Instance;
    }

    static EnemyControllerScript& Instance()
    {
        return *s_Instance;
    }

    const Animation* GetActiveAnimation() const { return m_ActiveAnimation; }
    int GetCurrentFrame() const { return m_CurrentFrame; }


    void StartFight()
    {
        e_BossEnemyFightState = EnemyState::e_CutsceneStart;
        PlayAnimation("fight_start");  
        m_CurrentFrame = 0;
        m_AnimationTime = 0.0f;
    }

private:

    //---------------------------------------------------------------//
    //----------------predefined override functions------------------//
    //---------------------------------------------------------------//

    void Start() override
    {
        if (s_Instance && s_Instance != this)
        {
            FX_WARN("GameManagerScript: Instance already exists. Ignoring this one.");
            return;
        }

        s_Instance = this;

        srand(static_cast<unsigned>(time(nullptr))); // Initialise randomizer

        auto loadAnim = [&](const std::string& key, const std::string& file, float dur) 
            {
            Animation anim;
            anim.frameDuration = dur;
            anim.frames = Flux::FileLoader::LoadAsciiFrames(file);
            if (anim.frames.empty()) 
            {
                FX_ERROR("Animation '{}' from file '{}' has no frames!", key.c_str(), file.c_str());
            }
            m_Animations[key] = std::move(anim);
            };

        loadAnim("target_left", "BOSS_Target_Left.txt", 0.2f);
        loadAnim("target_right", "BOSS_Target_Right.txt", 0.2f);
        loadAnim("attack_left", "BOSS_Attack_Left.txt", 0.15f);
        loadAnim("attack_right", "BOSS_Attack_Right.txt", 0.15f);
        loadAnim("fight_start", "CUTSCENE_1.txt", 0.15f);
        loadAnim("fight_end", "CUTSCENE_2.txt", 0.15f);
        loadAnim("target_getHit_left", "BOSS_GetHit_Left.txt", 0.15f);
        loadAnim("target_getHit_right", "BOSS_GetHit_Right.txt", 0.15f);

    }

    void PlayAnimation(const std::string& _name)
    {
        auto it = m_Animations.find(_name);
        if (it != m_Animations.end() && !it->second.frames.empty())
        {
            m_ActiveAnimation = &it->second;
            m_CurrentFrame = 0;
            m_AnimationTime = 0.0f;
        }
        else
        {
            FX_ERROR("Animation '{}' not found or has no frames!", _name.c_str());
            m_ActiveAnimation = nullptr;
        }
    }


    bool AnimationFinished() const
    {
        return m_ActiveAnimation && m_CurrentFrame >= (int)m_ActiveAnimation->frames.size() - 1;
    }



    void Update(float _deltaTime) override
    {
        if (m_ActiveAnimation)
        {
            m_AnimationTime += _deltaTime;

            if (m_AnimationTime >= m_ActiveAnimation->frameDuration)
            {
                m_AnimationTime = 0.0f;

                if (e_BossEnemyFightState == EnemyState::e_WaitingForPlayer)
                {
                    m_CurrentFrame = (m_CurrentFrame + 1) % (int)m_ActiveAnimation->frames.size();
                }
                else
                {
                    if (m_CurrentFrame < (int)m_ActiveAnimation->frames.size() - 1)
                        m_CurrentFrame++;
                }
            }
        }

        static EnemyState e_PreviousState = EnemyState::e_CutsceneStart;

        if (e_BossEnemyFightState != e_PreviousState)
        {
            switch (e_BossEnemyFightState)
            {
            case EnemyState::e_CutsceneStart:
                PlayAnimation("fight_start");
                break;

            case EnemyState::e_Targeting:
                m_LastTargetDirection = (rand() % 2 == 0) ? Direction::e_Left : Direction::e_Right;
                PlayAnimation(m_LastTargetDirection == Direction::e_Left ? "target_left" : "target_right");
                e_BossEnemyFightState = EnemyState::e_WaitingForPlayer;
                break;

            case EnemyState::e_WaitingForPlayer:
                break;

            case EnemyState::e_TargetGetHit: 
                PlayAnimation(m_LastTargetDirection == Direction::e_Left ? "target_getHit_left" : "target_getHit_right");
                break;

            case EnemyState::e_Attacking:
                PlayAnimation(m_LastTargetDirection == Direction::e_Left ? "attack_left" : "attack_right");
                break;

            case EnemyState::e_CutsceneEnd:
                PlayAnimation("fight_end");
                break;
            }
        }

        e_PreviousState = e_BossEnemyFightState;

        switch (e_BossEnemyFightState)
        {
        case EnemyState::e_CutsceneStart:
            if (AnimationFinished())
                e_BossEnemyFightState = EnemyState::e_Targeting;
            break;

        case EnemyState::e_WaitingForPlayer:
            break;

        case EnemyState::e_TargetGetHit:
            if (AnimationFinished())
            {
               e_BossEnemyFightState = EnemyState::e_Attacking;
            }
            break;

        case EnemyState::e_Attacking:
            if (AnimationFinished())
            {
                bool playerHit = false;
                if (m_LastTargetDirection == Direction::e_Left &&
                    GameManagerScript::Instance().e_PlayerPlatform == GameManagerScript::Platform::e_Left)
                    playerHit = true;
                else if (m_LastTargetDirection == Direction::e_Right &&
                    GameManagerScript::Instance().e_PlayerPlatform == GameManagerScript::Platform::e_Right)
                    playerHit = true;

                if (playerHit)
                {
                    Flux::Application::Get().CloseApplication();
                    return;
                }

                e_BossEnemyFightState = EnemyState::e_Targeting;
            }
            break;

        case EnemyState::e_CutsceneEnd:
            if (AnimationFinished())
            {
                Flux::Application::Get().CloseApplication();
                return;
            }
            break;
        }
    }

    void OnDestroy() override
    {
        if (s_Instance == this)
            s_Instance = nullptr;
    }
};

inline EnemyControllerScript* EnemyControllerScript::s_Instance = nullptr;

// Uncomment the code line below for your script to be registered by the Engine Script Manager
REGISTER_SCRIPT(EnemyControllerScript);
