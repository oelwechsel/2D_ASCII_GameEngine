#pragma once
#include <Flux.h>

class AtventureScript : public Flux::IScript
{
public:

	bool success;
	std::vector<std::string> lines;

	void Start() override
	{
		const char* image = "testbild.png"; 
		int width, height, channels;

		unsigned char* data = Flux::FileLoader::LoadImageFromFile(image, &width, &height, &channels, 0);

		FX_INFO("Trying to load image: {0}", image);

		if (data)
		{
			FX_INFO("Image loaded successfully!");
			Flux::FileLoader::FreeImage(data);
		}

		lines = Flux::FileLoader::LoadTextFileLines("map.txt", &success);

		/*if (success)
		{
			for (const auto& line : lines)
				std::cout << line << "\n";
		}*/

	}

	void Update(float deltaTime) override
	{
		if (Flux::Input::IsKeyPressed(FX_KEY_TAB))
			FX_INFO("Tab pressed");
	}

	void OnImGuiRender() override
	{
		Flux::ImGuiWrapper::Begin("@venture", ImVec2(1000, 100), ImVec2(100, 100), ImGuiWindowFlags_AlwaysVerticalScrollbar);
		if (success)
		{
			Flux::ImGuiWrapper::Text(lines);
		}
		//Flux::ImGuiWrapper::Text("Hello from @venture!");
		Flux::ImGuiWrapper::End();
	}
};


REGISTER_SCRIPT(AtventureScript);
