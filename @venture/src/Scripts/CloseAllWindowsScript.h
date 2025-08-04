#pragma once
#include <Flux.h>

class CloseAllWindowsScript : public Flux::IScript
{

private:

	void Start() override
	{
		
	}

	void Update(float deltaTime) override
	{
		
	}

	void OnImGuiRender() override
	{
		Flux::ImGuiWrapper::Begin("Window Control", ImVec2(100, 100), ImVec2(100, 100), ImGuiWindowFlags_NoCollapse);
		if (Flux::ImGuiWrapper::Button("Close All Windows")) {
			Flux::Application::Get().CloseApplication();
		}
		Flux::ImGuiWrapper::End();
	}

	void OnDestroy() override {  }
};


REGISTER_SCRIPT(CloseAllWindowsScript);
