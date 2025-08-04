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

    float m_inputCooldown = 0.0f;
    float m_inputDelay = 0.1f; 

    void Update(float deltaTime) override
    {
        m_inputCooldown -= deltaTime;
        if (m_inputCooldown > 0.0f)
            return; 

        if (Flux::Input::IsKeyPressed(FX_KEY_W))
        {
            GameManagerScript::Instance().entities[0].y--;
            m_inputCooldown = m_inputDelay;
            if (MapRendererScript::Get())
                MapRendererScript::Get()->UpdateRenderTiles();
            return;
        }
        else if (Flux::Input::IsKeyPressed(FX_KEY_A))
        {
            GameManagerScript::Instance().entities[0].x--;
            m_inputCooldown = m_inputDelay;
            if (MapRendererScript::Get())
                MapRendererScript::Get()->UpdateRenderTiles();
            return;
        }
        else if (Flux::Input::IsKeyPressed(FX_KEY_S))
        {
            GameManagerScript::Instance().entities[0].y++;
            m_inputCooldown = m_inputDelay;
            if (MapRendererScript::Get())
                MapRendererScript::Get()->UpdateRenderTiles();
            return;
        }
        else if (Flux::Input::IsKeyPressed(FX_KEY_D))
        {
            GameManagerScript::Instance().entities[0].x++;
            m_inputCooldown = m_inputDelay;
            if (MapRendererScript::Get())
                MapRendererScript::Get()->UpdateRenderTiles();
            return;
        }

    }

};


// Uncomment the code line below for your script to be registered by the Engine Script Manager
REGISTER_SCRIPT(PlayerControllerScript);
