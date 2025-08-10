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


	int m_screenWidthWindows = GetSystemMetrics(SM_CXSCREEN);
	int m_screenHeightWindows = GetSystemMetrics(SM_CYSCREEN);

	ImVec2 m_scale;
public:

	//-------------------------------------------//
	//----------------Functions------------------//
	//-------------------------------------------//

	TestConsole m_overworldConsole;
	FightConsole m_fightConsole;
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
		m_scale = Flux::ImGuiWrapper::GetScale();
		if (!GameManagerVariables::Instance().m_isInFight)
		{
			m_overworldConsole.Draw("console", "User @ connected", ImVec2(m_screenWidthWindows * 0.6f, m_screenHeightWindows * 0.3f), ImVec2(450 * m_scale.x, 600 * m_scale.y), &m_ConsoleFocused);
		}
		else 
		{
			m_fightConsole.Draw("fight console", "Type ‘help’ to display a list of available commands", ImVec2(m_screenWidthWindows * 0.6f, m_screenHeightWindows * 0.3f), ImVec2(450 * m_scale.x, 600 * m_scale.y), &m_ConsoleFocused);
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
