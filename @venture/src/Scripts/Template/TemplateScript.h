#pragma once
#include <Flux.h>

class TemplateScript : public Flux::IScript
{
	//-------------------------------------------//
	//----------------Variables------------------//
	//-------------------------------------------//

private:
	// define your private Variables here
	std::string m_TemplatePrivateVariable;

public: 
	// define your public Variables here
	std::string m_TemplatePublicVariable;
	
	//-------------------------------------------//
	//----------------Functions------------------//
	//-------------------------------------------//

public: 
	// define your public Functions here
	void PublicTestFunction()
	{
		FX_INFO("Value of templatePublicVariable: {0}", m_TemplatePublicVariable);
	}

private:
	// define your private Functions here
	void PrivateTestFunction()
	{
		FX_INFO("Value of templatePrivateVariable: {0}", m_TemplatePrivateVariable);
	}

	//---------------------------------------------------------------//
	//----------------predefined override functions------------------//
	//---------------------------------------------------------------//

	void Start() override
	{
		FX_INFO("TestScript started");
		PublicTestFunction();
	}

	void Update(float _deltaTime) override
	{
		if (Flux::Input::IsKeyPressed(FX_KEY_TAB))
			FX_INFO("Tab pressed");
	}

	void OnImGuiRender() override
	{
		Flux::ImGuiWrapper::Begin("Test-Fenster", ImVec2(1000, 100), ImVec2(100, 100), ImGuiWindowFlags_AlwaysVerticalScrollbar);
		Flux::ImGuiWrapper::Text("Hello from TextWindowScript!");
		Flux::ImGuiWrapper::End();
	}

	void OnDestroy() override { FX_INFO("Only add when working with pointers or deleting scripts"); }
};


// Uncomment the code line below for your script to be registered by the Engine Script Manager
//REGISTER_SCRIPT(TemplateScript);
