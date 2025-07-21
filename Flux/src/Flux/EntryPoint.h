#pragma once

#ifdef FX_PLATFORM_WINDOWS

extern Flux::Application* Flux::CreateApplication();

/*int main(int argc, char** argv) 
{
	Flux::Log::Init();
	FX_CORE_WARN("Initialized Log!");
	FX_INFO("Initialized Log!");

	auto app = Flux::CreateApplication();
	app->Run();
	delete app;
}*/


#endif
