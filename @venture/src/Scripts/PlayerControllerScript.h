#pragma once
#include <Flux.h>
#include "MapRendererScript.h"
#include "NPCWindowScript.h"
#include "ConsoleManagerScript.h"
#include "EnemyControllerScript.h"


class PlayerControllerScript : public Flux::IScript
{

    static PlayerControllerScript* s_Instance;

	//-------------------------------------------//
	//----------------Variables------------------//
	//-------------------------------------------//

private:
	// define your private Variables here

    double m_moveCooldown = 0.08;
    double m_moveTimer = 0.0;

    bool m_wasEPressedLastFrame = false;

    int lastNPC;

public:
	// define your public Variables here

	//-------------------------------------------//
	//----------------Functions------------------//
	//-------------------------------------------//

public:
	// define your public Functions here

    static PlayerControllerScript* Get()
    {
        return s_Instance;
    }

    static PlayerControllerScript& Instance()
    {
        return *s_Instance;
    }

private:
	// define your private Functions here

	//---------------------------------------------------------------//
	//----------------predefined override functions------------------//
	//---------------------------------------------------------------//

	void Start() override
	{
        if (s_Instance && s_Instance != this)
        {
            // Es gibt bereits eine gültige Instanz – wir ignorieren diese neue
            FX_WARN("GameManagerScript: Instance already exists. Ignoring this one.");
            return;
        }

        s_Instance = this;
	}

    void Update(float deltaTime) override
    {
        if (ConsoleManagerScript::Instance().m_ConsoleFocused || GameManagerVariables::Instance().m_isInFight)
            return;

        m_moveTimer += deltaTime;
        if (m_moveTimer < m_moveCooldown)
            return;

        int dx = 0, dy = 0;

        bool isEPressedNow = Flux::Input::IsKeyPressed(FX_KEY_E);
        bool isW = Flux::Input::IsKeyPressed(FX_KEY_W);
        bool isS = Flux::Input::IsKeyPressed(FX_KEY_S);
        bool isA = Flux::Input::IsKeyPressed(FX_KEY_A);
        bool isD = Flux::Input::IsKeyPressed(FX_KEY_D);

        if (isW) dy = -1;
        if (isS) dy = 1;
        if (isA) dx = -1;
        if (isD) dx = 1;

        auto& gm = GameManagerScript::Instance();
        auto& entities = gm.entities;
        auto& player = entities[0];

        if (isEPressedNow && !m_wasEPressedLastFrame && !gm.m_NPCsDead)
        {
            if (gm.m_playerIsInteracting)
            {
                NPCWindowScript::Instance().HideNPCWindow();
                gm.m_playerIsInteracting = false;

                entities[lastNPC].m_HasInteractedWith = true;
                if (entities[lastNPC].afterInteraction) {
                    entities[lastNPC].afterInteraction();
                }

                gm.HasInteractedWithAllNPCs();
            }
            else
            {
                std::vector<std::pair<int, int>> adjacentPositions = {
                    {player.m_xPos, player.m_yPos - 1},
                    {player.m_xPos, player.m_yPos + 1},
                    {player.m_xPos - 1, player.m_yPos},
                    {player.m_xPos + 1, player.m_yPos}
                };

                for (size_t i = 1; i < entities.size(); ++i)
                {
                    for (const auto& [x, y] : adjacentPositions)
                    {
                        if (entities[i].m_xPos == x && entities[i].m_yPos == y)
                        {
                            NPCWindowScript::Instance().ShowNPCWindow(entities[i]);

                            std::string command = "$ cat ./home/" + entities[i].m_Name + "/welcome_message.txt";
                            ConsoleManagerScript::Instance().m_overworldConsole.AddLog(Flux::ImGuiConsole::LogLevel::e_Info, "%s", command.c_str());

                            const auto& lines = gm.LoadDialogueLinesForSymbol(entities[i].m_DirName);
                            for (const auto& line : lines) 
                            {
                                ConsoleManagerScript::Instance().m_overworldConsole.AddCustomLog(entities[i].m_Name, entities[i].m_LogColor, line.c_str());
                            }

                            if (entities[i].onInteract) {
                                entities[i].onInteract();
                            }

                            gm.m_playerIsInteracting = true;
                            lastNPC = i;
                            break;
                        }
                    }
                }
            }
        }

        m_wasEPressedLastFrame = isEPressedNow;

        if (gm.m_playerIsInteracting && ( isW || isS || isA || isD))
        {
            NPCWindowScript::Instance().HideNPCWindow();
            gm.m_playerIsInteracting = false;

            entities[lastNPC].m_HasInteractedWith = true;
            if (entities[lastNPC].afterInteraction) {
                entities[lastNPC].afterInteraction();
            }

            gm.HasInteractedWithAllNPCs();
        }

        if ((dx != 0 || dy != 0) && !gm.m_playerIsInteracting)
        {
            int targetX = player.m_xPos + dx;
            int targetY = player.m_yPos + dy;

            if (!gm.m_NPCsDead) {
                for (size_t i = 1; i < entities.size(); ++i)
                {
                    if (entities[i].m_xPos == targetX && entities[i].m_yPos == targetY)
                        return;
                }
            }

            auto mapScript = MapRendererScript::Get();
            if (!mapScript || mapScript->IsTileBlocked(targetX, targetY))
                return;

            player.m_xPos = targetX;
            player.m_yPos = targetY;
            m_moveTimer = 0.0f;

            mapScript->UpdateRenderTiles();

            if (player.m_xPos == 39 && player.m_yPos == 16) {
                GameManagerVariables::Instance().m_isInFight = true;
                EnemyControllerScript::Instance().StartFight();
            }
        }
    }


    void OnImGuiRender() override 
    {

    }

    void OnDestroy() override
    {
        if (s_Instance == this)
            s_Instance = nullptr;
    }
};

inline PlayerControllerScript* PlayerControllerScript::s_Instance = nullptr;

// Uncomment the code line below for your script to be registered by the Engine Script Manager
REGISTER_SCRIPT(PlayerControllerScript);
