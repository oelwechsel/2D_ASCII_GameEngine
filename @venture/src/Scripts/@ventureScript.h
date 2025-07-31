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

		 /*bool ok;
		auto frames = Flux::FileLoader::LoadAsciiFrames("EQUA_Animation.txt", &ok);
		if (!ok) return;

		for (size_t frameIndex = 0; frameIndex < frames.size(); ++frameIndex)
		{
			std::cout << "=== FRAME " << frameIndex << " ===\n";

			const auto& frame = frames[frameIndex];
			for (size_t layerIndex = 0; layerIndex < frame.size(); ++layerIndex)
			{
				std::cout << "--- LAYER " << layerIndex << " ---\n";

				const auto& layer = frame[layerIndex];
				for (const auto& line : layer)
				{
					std::cout << line << "\n";
				}

				std::cout << "\n";
			}

			std::cout << "\n";
		}*/

		TextureID tex = Flux::FileLoader::LoadTileset("tileset.png", 14);
		if (tex == 0) {
			std::cerr << "Fehler beim Laden" << std::endl;
		}
		else {
			std::cout << "Textur geladen, ID: " << tex << std::endl;
		}

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
