#pragma once
#include <Flux.h>
#include "MapRendererScript.h"
#include "NPCWindow.h"


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
        m_moveTimer += deltaTime;
        if (m_moveTimer < m_moveCooldown)
            return;

        int dx = 0, dy = 0;

        bool isEPressedNow = Flux::Input::IsKeyPressed(FX_KEY_E);

        if (isEPressedNow && !m_wasEPressedLastFrame)
        {
            auto& entities = GameManagerScript::Instance().entities;
            auto& player = entities[0];

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
                        if (!GameManagerScript::Instance().m_playerIsInteracting)
                        {
                            NPCWindow::Instance().ShowNPCWindow(entities[i]);
                            GameManagerScript::Instance().m_playerIsInteracting = true;
                        }
                        else
                        {
                            NPCWindow::Instance().HideNPCWindow();
                            GameManagerScript::Instance().m_playerIsInteracting = false;
                        }

                        break;
                    }
                }
            }
        }

        m_wasEPressedLastFrame = isEPressedNow;

        if (!GameManagerScript::Instance().m_playerIsInteracting)
        {

            // Movement 
            if (Flux::Input::IsKeyPressed(FX_KEY_W)) dy = -1;
            if (Flux::Input::IsKeyPressed(FX_KEY_S)) dy = 1;
            if (Flux::Input::IsKeyPressed(FX_KEY_A)) dx = -1;
            if (Flux::Input::IsKeyPressed(FX_KEY_D)) dx = 1;

            auto& entities = GameManagerScript::Instance().entities;
            auto& player = entities[0];

            int targetX = player.x + dx;
            int targetY = player.y + dy;

            for (size_t i = 1; i < entities.size(); ++i)
            {
                if (entities[i].x == targetX && entities[i].y == targetY)
                    return;
            }

            auto mapScript = MapRendererScript::Get();
            if (!mapScript)
                return;

            if (mapScript->IsTileBlocked(targetX, targetY))
                return;

            player.x = targetX;
            player.y = targetY;
            m_moveTimer = 0.0;

            mapScript->UpdateRenderTiles();
        }
    }

};


// Uncomment the code line below for your script to be registered by the Engine Script Manager
REGISTER_SCRIPT(PlayerControllerScript);
