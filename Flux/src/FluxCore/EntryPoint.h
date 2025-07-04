#pragma once

#ifdef FX_PLATFORM_WINDOWS

extern Flux::Application* Flux::CreateApplication();

int main(int argc, char** argv) 
{
	std::cout << "test" << std::endl;
	auto app = Flux::CreateApplication();
	app->Run();
	delete app;
}

#endif
