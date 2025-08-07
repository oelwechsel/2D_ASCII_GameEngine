#pragma once
#include <Flux.h>
#include "PlayerControllerScript.h"
#include "EnemyControllerScript.h"


class FightRendererScript : public Flux::IScript
{
	static FightRendererScript* s_Instance;

	//-------------------------------------------//
	//----------------Variables------------------//
	//-------------------------------------------//

private:

	
	//-------------------------------------------//
	//----------------Functions------------------//
	//-------------------------------------------//

public:

	

	static FightRendererScript* Get()
	{
		return s_Instance;
	}

	static FightRendererScript& Instance()
	{
		return *s_Instance;
	}

private:


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
		
	}


	void OnImGuiRender() override
	{
		if (GameManagerScript::Instance().m_isInFight) 
		{
			Flux::ImGuiWrapper::Begin("Left Platform", ImVec2(200, 100), ImVec2(300, 1000), ImGuiWindowFlags_NoCollapse);
			Flux::ImGuiWrapper::Text(PlayerControllerScript::Instance().e_PlayerPlatform == PlayerControllerScript::Platform::Left ? "Player is here." : "No player here.");
			Flux::ImGuiWrapper::End();

			Flux::ImGuiWrapper::Begin("Right Platform", ImVec2(200, 100), ImVec2(600, 1000), ImGuiWindowFlags_NoCollapse);
			Flux::ImGuiWrapper::Text(PlayerControllerScript::Instance().e_PlayerPlatform == PlayerControllerScript::Platform::Right ? "Player is here." : "No player here.");
			Flux::ImGuiWrapper::End();

			Flux::ImGuiWrapper::Begin("R00T", ImVec2(800, 800), ImVec2(100, 100), ImGuiWindowFlags_NoCollapse);
			Flux::ImGuiWrapper::Text("ROOT HP: %d", EnemyControllerScript::Instance().m_BossEnemyHP);

			// DrawLayeredFrames?

			Flux::ImGuiWrapper::End();
		}
	}

	void OnDestroy() override
	{
		if (s_Instance == this)
			s_Instance = nullptr;
	}
};

inline FightRendererScript* FightRendererScript::s_Instance = nullptr;

REGISTER_SCRIPT(FightRendererScript);
