#include <Flux.h>

class ExampleLayer : public Flux::Layer
{
public:
	ExampleLayer(): Layer("Example"){}

	void OnUpdate() override { }

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
