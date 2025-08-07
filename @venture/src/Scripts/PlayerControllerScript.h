#pragma once
#include <Flux.h>
#include "MapRendererScript.h"
#include "NPCWindowScript.h"
#include "ConsoleManagerScript.h"


class PlayerControllerScript : public Flux::IScript
{
	//-------------------------------------------//
	//----------------Variables------------------//
	//-------------------------------------------//

private:
	// define your private Variables here

    double m_moveCooldown = 0.08; // Sekunden zwischen Bewegungen
    double m_moveTimer = 0.0;     // Zeit seit letzter Bewegung

    bool m_wasEPressedLastFrame = false;

public:
	// define your public Variables here

	//-------------------------------------------//
	//----------------Functions------------------//
	//-------------------------------------------//

public:
	// define your public Functions here

private:
	// define your private Functions here

	//---------------------------------------------------------------//
	//----------------predefined override functions------------------//
	//---------------------------------------------------------------//

	void Start() override
	{
	}

    void Update(float deltaTime) override
    {
        if (!Flux::ImGuiWrapper::IsWindowFocused("@venture"))
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

        if (isEPressedNow && !m_wasEPressedLastFrame)
        {
            if (gm.m_playerIsInteracting)
            {
                NPCWindowScript::Instance().HideNPCWindow();
                gm.m_playerIsInteracting = false;
            }
            else
            {
                std::vector<std::pair<int, int>> adjacentPositions = {
                    {player.x, player.y - 1},
                    {player.x, player.y + 1},
                    {player.x - 1, player.y},
                    {player.x + 1, player.y}
                };

                for (size_t i = 1; i < entities.size(); ++i)
                {
                    for (const auto& [x, y] : adjacentPositions)
                    {
                        if (entities[i].x == x && entities[i].y == y)
                        {
                            NPCWindowScript::Instance().ShowNPCWindow(entities[i]);

                            std::string command = "$ cat ./home/" + entities[i].name + "/welcome_message.txt";
                            ConsoleManagerScript::Instance().m_console.AddLog(Flux::ImGuiConsole::LogLevel::Info, "%s", command.c_str());

                            for (const auto& line : entities[i].dialogueLines) {
                                ConsoleManagerScript::Instance().m_console.AddLog(line.c_str());
                            }

                            gm.m_playerIsInteracting = true;
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
        }

        if ((dx != 0 || dy != 0) && !gm.m_playerIsInteracting)
        {
            int targetX = player.x + dx;
            int targetY = player.y + dy;

            for (size_t i = 1; i < entities.size(); ++i)
            {
                if (entities[i].x == targetX && entities[i].y == targetY)
                    return;
            }

            auto mapScript = MapRendererScript::Get();
            if (!mapScript || mapScript->IsTileBlocked(targetX, targetY))
                return;

            player.x = targetX;
            player.y = targetY;
            m_moveTimer = 0.0f;

            mapScript->UpdateRenderTiles();
        }
    }


    void OnImGuiRender() override 
    {

    }

};


// Uncomment the code line below for your script to be registered by the Engine Script Manager
REGISTER_SCRIPT(PlayerControllerScript);
