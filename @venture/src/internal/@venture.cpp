
#include "fxpch.h"
#include <Flux.h>

#ifdef __has_include
#  if __has_include("generated/ScriptIncludes.gen.h")
#    include "generated/ScriptIncludes.gen.h"
#  endif
#endif


class ATventure : public Flux::Application
{
public:
	ATventure()
	{}
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