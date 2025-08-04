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

        if (Flux::Input::IsKeyPressed(FX_KEY_W))
        {
            GameManagerScript::Instance().entities[0].y--;
            
            moveTimer = 0;
            if (MapRendererScript::Get())
                MapRendererScript::Get()->UpdateRenderTiles();
        }
        if (Flux::Input::IsKeyPressed(FX_KEY_A))
        {
            GameManagerScript::Instance().entities[0].x--;
            moveTimer = 0;
            if (MapRendererScript::Get())
                MapRendererScript::Get()->UpdateRenderTiles();
        }
        if (Flux::Input::IsKeyPressed(FX_KEY_S))
        {
            GameManagerScript::Instance().entities[0].y++;
            moveTimer = 0;
            if (MapRendererScript::Get())
                MapRendererScript::Get()->UpdateRenderTiles();
        }
        if (Flux::Input::IsKeyPressed(FX_KEY_D))
        {
            GameManagerScript::Instance().entities[0].x++;
            moveTimer = 0;
            if (MapRendererScript::Get())
                MapRendererScript::Get()->UpdateRenderTiles();
        }
    }
};


// Uncomment the code line below for your script to be registered by the Engine Script Manager
REGISTER_SCRIPT(PlayerControllerScript);
