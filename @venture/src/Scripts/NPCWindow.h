#pragma once
#include <Flux.h>

class NPCWindow : public Flux::IScript
{
	//-------------------------------------------//
	//----------------Variables------------------//
	//-------------------------------------------//

private:

public:
	bool m_showWindow = false;
	ImVec2 m_windowSize;
	ImVec2 m_position;

	//-------------------------------------------//
	//----------------Functions------------------//
	//-------------------------------------------//

public:

private:

	//---------------------------------------------------------------//
	//----------------predefined override functions------------------//
	//---------------------------------------------------------------//

	void Start() override
	{
	}

	void Update(float deltaTime) override
	{
	}

	void OnImGuiRender() override
	{
		if (m_showWindow) {
			Flux::ImGuiWrapper::Begin("Test-Fenster", m_windowSize, m_position, ImGuiWindowFlags_AlwaysVerticalScrollbar);
			Flux::ImGuiWrapper::Text("Hello from TextWindowScript!");
			Flux::ImGuiWrapper::End();
		}
	}

	void ShowNPCWindow() {
		m_showWindow = true;
	}

	void HideNPCWindow() {
		m_showWindow = false;
	}

	void OnDestroy() override {  }
};


REGISTER_SCRIPT(NPCWindow);
