#include <Flux.h>

class ExampleLayer : public Flux::Layer
{
public:
	ExampleLayer(): Layer("Example"){}

	void OnUpdate() override 
	{ 
		if (Flux::Input::IsKeyPressed(FX_KEY_TAB))
			FX_INFO("Tab pressed");
	}

	void OnEvent(Flux::Event& _event) override { FX_TRACE("{0}", _event); }
};

class ATventure : public Flux::Application
{
public:
	ATventure()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new Flux::ImGuiLayer());
	}
	~ATventure()
	{

	}
};

Flux::Application* Flux::CreateApplication()
{
	return new ATventure();
}
