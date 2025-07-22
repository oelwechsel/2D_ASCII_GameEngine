#pragma once

#include <Flux.h>

class TestScript : public Flux::IScript 
{
public: 
	void Start() override
	{
		FX_INFO("TestScript started");
	}

	void Update(float deltaTime) override
	{
		if (Flux::Input::IsKeyPressed(FX_KEY_TAB))
			FX_INFO("Tab pressed");
	}

	void OnImGuiRender() override
	{


		Flux::ImGuiWrapper::Begin("Test-Fenster",ImVec2(1000,100), ImVec2(100, 100), ImGuiWindowFlags_AlwaysVerticalScrollbar);
		Flux::ImGuiWrapper::Text("Hello from TextWindowScript!");
		Flux::ImGuiWrapper::End();
	}
};


REGISTER_SCRIPT(TestScript);
