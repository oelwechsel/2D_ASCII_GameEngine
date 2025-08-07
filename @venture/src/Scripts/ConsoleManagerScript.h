#pragma once
#include <Flux.h>
#include "_internal/customizables/Consoles/TestConsole.h"
#include "_internal/customizables/Consoles/FightConsole.h"
#include "GameManagerScript.h"

class ConsoleManagerScript : public Flux::IScript
{
	//-------------------------------------------//
	//----------------Variables------------------//
	//-------------------------------------------//

private:

	static ConsoleManagerScript* s_Instance;

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
		if (!GameManagerScript::Instance().m_isInFight)
		{
			m_overworldConsole.Draw("console", "welcome", ImVec2(300, 300), ImVec2(300, 300), &m_ConsoleFocused);
		}
		else 
		{
			m_fightConsole.Draw("fight console", "welcome", ImVec2(500, 500), ImVec2(500, 500), &m_ConsoleFocused);
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
