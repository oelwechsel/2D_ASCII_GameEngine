#pragma once
#include <Flux.h>
#include "_internal/customizables/Consoles/TestConsole.h"

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

	TestConsole m_console;
	bool m_NormalConsoleOpen = true;
	bool m_FightConsoleOpen = false;
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
		s_Instance = this;
	}

	void Update(float deltaTime) override
	{

	}

	void OnImGuiRender() override
	{
		if (m_NormalConsoleOpen)
		{
			m_console.Draw("test", "welcome", ImVec2(300, 300), ImVec2(300, 300), &m_ConsoleFocused);
		}
	}

	void OnDestroy() override { s_Instance = nullptr; }
};


ConsoleManagerScript* ConsoleManagerScript::s_Instance = nullptr;

// Uncomment the code line below for your script to be registered by the Engine Script Manager
REGISTER_SCRIPT(ConsoleManagerScript);
