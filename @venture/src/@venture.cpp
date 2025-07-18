#include <Flux.h>

class ExampleLayer : public Flux::Layer
{
public:
	ExampleLayer(): Layer("Example"){}

	void OnUpdate() override { FX_INFO("ExampleLayer::Update"); }

	void OnEvent(Flux::Event& _event) override { FX_TRACE("{0}", _event); }
};

class ATventure : public Flux::Application
{
public:
	ATventure()
	{
		PushLayer(new ExampleLayer());
	}
	~ATventure()
	{

	}
};

Flux::Application* Flux::CreateApplication()
{
	return new ATventure();
}
