#pragma once
#include <Flux.h>

class CloseAllWindowsScript : public Flux::IScript
{

private:

	ImVec2 m_Position;
	int m_ScreenWidthWindows = GetSystemMetrics(SM_CXSCREEN);
	int m_ScreenHeightWindows = GetSystemMetrics(SM_CYSCREEN);

	unsigned int m_MDHLogo;

	std::vector<std::string> m_Credits = { "Credits", "Three Pebbles", "Programming: Leonhard Thiel, Seraphina Lange", "Art : Gabrielle Sibucao", "Supervising lecturers : Dominik Mieth, Michael Coldewey" };

	ImVec2 m_Scale;

private:

	void Start() override
	{
		m_Position.x = m_ScreenWidthWindows * 0.75f;
		m_Position.y = m_ScreenHeightWindows * 0.0f;

		m_MDHLogo = Flux::FileLoader::LoadTextureFromFile("MDH_Logo.png");
	}

	void Update(float _deltaTime) override {}

	void OnImGuiRender() override
	{
		m_Scale = Flux::ImGuiWrapper::GetScale();
		Flux::ImGuiWrapper::Begin("Window Control", ImVec2(650 * m_Scale.x, 400 * m_Scale.y), m_Position, ImGuiWindowFlags_NoCollapse);
		if (Flux::ImGuiWrapper::Button("Close All Windows")) {
			Flux::Application::Get().CloseApplication();
		}
		Flux::ImGuiWrapper::Image((ImTextureID)m_MDHLogo, ImVec2(559, 222));
		Flux::ImGuiWrapper::Text(m_Credits);
		Flux::ImGuiWrapper::End();
	}

	void OnDestroy() override {  }
};


REGISTER_SCRIPT(CloseAllWindowsScript);
