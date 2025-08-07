
#include "fxpch.h"
#include <Flux.h>

#ifdef __has_include
#  if __has_include("_generated/ScriptIncludes.gen.h")
#    include "_generated/ScriptIncludes.gen.h"
#  endif
#endif


class NewAtventure : public Flux::Application
{
public:
	NewAtventure()
	{}
	~NewAtventure() override = default;
};


Flux::Application* Flux::CreateApplication()
{
	return new NewAtventure();
}

int main(int argc, char** argv) 
{
	Flux::Log::Init();
	FX_CORE_WARN("Initialized Log!");
	FX_INFO("Initialized Log!");

	auto app = Flux::CreateApplication();
	Flux::ImGuiWrapper::LoadCustomFont("src/res/SourceCodePro-Regular.ttf", 16.0f, "ˉ");
	app->Run();
	delete app;
}

