#pragma once
#include <Flux.h>
#include "MapRendererScript.h"

class PlayerControllerScript : public Flux::IScript
{
	//-------------------------------------------//
	//----------------Variables------------------//
	//-------------------------------------------//

private:
	// define your private Variables here

    double moveCooldown = 0.08; // Sekunden zwischen Bewegungen
    double moveTimer = 0.0;     // Zeit seit letzter Bewegung

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
        moveTimer += deltaTime;
        if (moveTimer < moveCooldown)
            return;

        int dx = 0, dy = 0;

        if (Flux::Input::IsKeyPressed(FX_KEY_W)) dy = -1;
        else if (Flux::Input::IsKeyPressed(FX_KEY_S)) dy = 1;
        else if (Flux::Input::IsKeyPressed(FX_KEY_A)) dx = -1;
        else if (Flux::Input::IsKeyPressed(FX_KEY_D)) dx = 1;
        else
            return;

        auto& entities = GameManagerScript::Instance().entities;
        auto& player = entities[0];

        int targetX = player.x + dx;
        int targetY = player.y + dy;

        // EnitiyCollision
        for (size_t i = 1; i < entities.size(); ++i)
        {
            if (entities[i].x == targetX && entities[i].y == targetY)
                return;
        }

        // TileCollision
        auto mapScript = MapRendererScript::Get();
        if (!mapScript)
            return;

        if (mapScript->IsBlockedTile(targetX, targetY))
            return;

        player.x = targetX;
        player.y = targetY;
        moveTimer = 0.0;

        mapScript->UpdateRenderTiles();
    }


};


// Uncomment the code line below for your script to be registered by the Engine Script Manager
REGISTER_SCRIPT(PlayerControllerScript);
