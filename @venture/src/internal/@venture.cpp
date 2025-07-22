
#include "fxpch.h"
#include <Flux.h>
#include "../TestScript.h" //damit es überhaupt kompiliert

/*class ExampleLayer : public Flux::Layer
{
public:
	ExampleLayer(): Layer("Example"){}

	void OnUpdate() override 
	{ 
		if (Flux::Input::IsKeyPressed(FX_KEY_TAB))
			FX_INFO("Tab pressed");
	}

	void OnEvent(Flux::Event& _event) override { FX_TRACE("{0}", _event); }
};*/

class ATventure : public Flux::Application
{
public:
	ATventure()
	{
		//PushLayer(new ScriptManagerLayer());
	}
	~ATventure() override = default;
};

Flux::Application* Flux::CreateApplication()
{
	return new ATventure();
}

int main(int argc, char** argv) {
	Flux::Log::Init();
	FX_CORE_WARN("Initialized Log!");
	FX_INFO("Initialized Log!");

	auto app = Flux::CreateApplication();
	app->Run();
	delete app;
}
