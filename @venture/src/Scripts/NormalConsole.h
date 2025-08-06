#pragma once
#include <Flux.h>
#include "_internal/customizables/Consoles/TestConsole.h"

class NormalConsole : public Flux::IScript
{
	//-------------------------------------------//
	//----------------Variables------------------//
	//-------------------------------------------//

private:

	static NormalConsole* s_Instance;

public:

	//-------------------------------------------//
	//----------------Functions------------------//
	//-------------------------------------------//

	TestConsole m_console;

public:
	static NormalConsole* Get()
	{
		return s_Instance;
	}

	static NormalConsole& Instance()
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
		m_console.Draw("test", "welcome", ImVec2(300, 300), ImVec2(300, 300));
	}

	void OnDestroy() override { s_Instance = nullptr; }
};


NormalConsole* NormalConsole::s_Instance = nullptr;

// Uncomment the code line below for your script to be registered by the Engine Script Manager
REGISTER_SCRIPT(NormalConsole);
