#pragma once
#include <Flux.h>
#include "_internal/customizables/Consoles/TestConsole.h"
#include "_internal/customizables/Consoles/FightConsole.h"
#include "GameManagerVariables.h"

class ConsoleManagerScript : public Flux::IScript
{
	//-------------------------------------------//
	//----------------Variables------------------//
	//-------------------------------------------//

private:

	static ConsoleManagerScript* s_Instance;


	int m_ScreenWidthWindows = GetSystemMetrics(SM_CXSCREEN);
	int m_ScreenHeightWindows = GetSystemMetrics(SM_CYSCREEN);

	ImVec2 m_Scale;
public:

	//-------------------------------------------//
	//----------------Functions------------------//
	//-------------------------------------------//

	TestConsole m_OverworldConsole;
	FightConsole m_FightConsole;
	bool m_ConsoleFocused;

public:
	static ConsoleManagerScript* Get()
	{
		return s_Instance;
	}

	static ConsoleManagerScript& Instance()
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
			FX_WARN("GameManagerScript: Instance already exists. Ignoring this one.");
			return;
		}

		s_Instance = this;
	}

	void Update(float _deltaTime) override {}

	void OnImGuiRender() override
	{
		m_Scale = Flux::ImGuiWrapper::GetScale();
		if (!GameManagerVariables::Instance().m_IsInFight)
		{
			m_OverworldConsole.Draw("console", "User @ connected...", ImVec2(m_ScreenWidthWindows * 0.63f, m_ScreenHeightWindows * 0.3f), ImVec2(450 * m_Scale.x, 600 * m_Scale.y), &m_ConsoleFocused);
		}
		else 
		{
			m_FightConsole.Draw("fight console", "Type help to display a list of available commands", ImVec2(m_ScreenWidthWindows * 0.6f, m_ScreenHeightWindows * 0.3f), ImVec2(450 * m_Scale.x, 600 * m_Scale.y), &m_ConsoleFocused);
		}
	}

	void OnDestroy() override
	{
		if (s_Instance == this)
			s_Instance = nullptr;
	}
};


inline ConsoleManagerScript* ConsoleManagerScript::s_Instance = nullptr;

// Uncomment the code line below for your script to be registered by the Engine Script Manager
REGISTER_SCRIPT(ConsoleManagerScript);
