#pragma once
#include <Flux.h>
#include "EnemyControllerScript.h"
#include "GameManagerScript.h"


class FightRendererScript : public Flux::IScript
{
	static FightRendererScript* s_Instance;

	std::vector<ImU32> m_layerColors = {
		IM_COL32(255, 255, 255, 255),
		IM_COL32(255, 255, 255, 200),
		IM_COL32(255, 255, 255, 100),
		IM_COL32(255, 255, 255, 32),
		IM_COL32(255, 255, 255, 32),
	};

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
			Flux::ImGuiWrapper::Text(GameManagerScript::Instance().e_PlayerPlatform == GameManagerScript::Platform::Left ? "Player is here." : "No player here.");
			Flux::ImGuiWrapper::End();

			Flux::ImGuiWrapper::Begin("Right Platform", ImVec2(200, 100), ImVec2(600, 1000), ImGuiWindowFlags_NoCollapse);
			Flux::ImGuiWrapper::Text(GameManagerScript::Instance().e_PlayerPlatform == GameManagerScript::Platform::Right ? "Player is here." : "No player here.");
			Flux::ImGuiWrapper::End();

			Flux::ImGuiWrapper::Begin("Enemy", ImVec2(400, 400), ImVec2(1000, 100), ImGuiWindowFlags_NoCollapse);

			auto* enemyController = EnemyControllerScript::Get();
			if (!enemyController)
			{
				FX_ERROR("EnemyControllerScript::Get() returned nullptr!");
				Flux::ImGuiWrapper::End();
				return;
			}

			auto* anim = enemyController->GetActiveAnimation();
			if (!anim)
			{
				FX_ERROR("No active animation set in EnemyControllerScript!");
				Flux::ImGuiWrapper::End();
				return;
			}

			if (anim->frames.empty())
			{
				FX_ERROR("Active animation has no frames!");
				Flux::ImGuiWrapper::End();
				return;
			}

			int frameIndex = enemyController->GetCurrentFrame();
			if (frameIndex < 0 || frameIndex >= (int)anim->frames.size())
			{
				FX_ERROR("Frame index out of range!", frameIndex, (int)anim->frames.size());
				Flux::ImGuiWrapper::End();
				return;
			}

			auto* drawList = Flux::ImGuiWrapper::GetWindowDrawList();
			ImVec2 startPos = Flux::ImGuiWrapper::GetCursorScreenPos();
			const auto& frame = anim->frames[frameIndex];
			if (!frame.layers.empty())
			{
				Flux::ImGuiWrapper::AnimateLayers(drawList, startPos, frame.layers, m_layerColors);
			}
			else
			{
				FX_WARN("Current frame has no layers!");
			}



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
