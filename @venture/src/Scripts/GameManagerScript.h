#pragma once
#include <Flux.h>

class GameManagerScript : public Flux::IScript
{
public:
    static GameManagerScript* s_Instance;

    GameManagerScript() = default;

    std::vector<Flux::Entity> entities;

    bool m_playerIsInteracting = false;

    bool m_isInFight = false;

public:

    static GameManagerScript* Get()
    {
        return s_Instance;
    }

    static GameManagerScript& Instance()
    {
        return *s_Instance;
    }

    void Start() override
    {
        if (s_Instance && s_Instance != this)
        {
            // Es gibt bereits eine gültige Instanz – wir ignorieren diese neue
            FX_WARN("GameManagerScript: Instance already exists. Ignoring this one.");
            return;
        }

        s_Instance = this;

        // Rest des Codes
        Flux::Entity player(14, 14, '@', "@", true);
        entities.push_back(player);

        Flux::Entity questionNPC(33, 32, '?', "?", 0.2f, ImVec2(180, 370), false);
        questionNPC.layeredFrames = Flux::FileLoader::LoadAsciiFrames("QUEST_Animation.txt");
        questionNPC.dialogueLines = { "Wanna learn something good, kid?", "At some point you will need SUDO" };
        entities.push_back(questionNPC);

        Flux::Entity andNPC(11, 9, '&', "&", 0.2f, ImVec2(240, 350), false);
        andNPC.layeredFrames = Flux::FileLoader::LoadAsciiFrames("AND_Animation.txt");
        andNPC.dialogueLines = { "Everyone is gone... I will be gone too" };
        entities.push_back(andNPC);

        Flux::Entity percentNPC(7, 23, '%', "%", 0.2f, ImVec2(300, 450), false);
        percentNPC.layeredFrames = Flux::FileLoader::LoadAsciiFrames("PERC_Animation.txt");
        percentNPC.dialogueLines = { "Pssssssssssst look at what I got" };
        entities.push_back(percentNPC);

        Flux::Entity equalsNPC(20, 20, 'a', "=", 0.2f, ImVec2(230, 350), false);
        equalsNPC.layeredFrames = Flux::FileLoader::LoadAsciiFrames("EQUA_Animation.txt");
        equalsNPC.dialogueLines = { "Oh man, now we've really done it", "*sweating profusely*" };
        entities.push_back(equalsNPC);

        Flux::Entity exclamationNPC(18, 14, '!', "!", 0.3f, ImVec2(170, 390), false);
        exclamationNPC.layeredFrames = Flux::FileLoader::LoadAsciiFrames("EXCL_Animation.txt");
        exclamationNPC.dialogueLines = { "*sob* *sob* *sob*" };
        entities.push_back(exclamationNPC);
    }

    void Update(float deltaTime) override {}

    void OnDestroy() override
    {
        if (s_Instance == this)
            s_Instance = nullptr;
    }
};

inline GameManagerScript* GameManagerScript::s_Instance = nullptr;

REGISTER_SCRIPT(GameManagerScript);
