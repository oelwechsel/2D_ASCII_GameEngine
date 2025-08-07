#pragma once
#include <Flux.h>

class CloseAllWindowsScript : public Flux::IScript
{

private:

	ImVec2 m_position;
	int m_screenWidthWindows = GetSystemMetrics(SM_CXSCREEN);
	int m_screenHeightWindows = GetSystemMetrics(SM_CYSCREEN);

	unsigned int m_MDHLogo;


	ImVec2 m_scale;

private:

	void Start() override
	{
		m_position.x = m_screenWidthWindows * 0.6f;
		m_position.y = m_screenHeightWindows * 0.2f;

		m_MDHLogo = Flux::FileLoader::LoadTextureFromFile("MDH_Logo.png");
	}

	void Update(float deltaTime) override
	{
		
	}

	void OnImGuiRender() override
	{
		m_scale = Flux::ImGuiWrapper::GetScale();
		Flux::ImGuiWrapper::Begin("Window Control", ImVec2(650 * m_scale.x, 300 * m_scale.y), m_position, ImGuiWindowFlags_NoCollapse);
		if (Flux::ImGuiWrapper::Button("Close All Windows")) {
			Flux::Application::Get().CloseApplication();
		}
		Flux::ImGuiWrapper::Image((ImTextureID)m_MDHLogo, ImVec2(559, 222));
		Flux::ImGuiWrapper::End();
	}

	void OnDestroy() override {  }
};


REGISTER_SCRIPT(CloseAllWindowsScript);
