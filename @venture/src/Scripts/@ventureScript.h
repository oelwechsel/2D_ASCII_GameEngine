#pragma once
#include <Flux.h>
#include "ASCIIBlockDictionary.h"
#include "MapToRenderTiles.h"

class AtventureScript : public Flux::IScript
{
public:

	bool success;

	ASCIIBlockDictionary m_asciiDictionary;
	std::vector<std::string> m_map;

	std::vector<Flux::RenderTile> m_tiles;
	unsigned int m_texture;

	//Flux::TileRenderer m_renderer; // TileRenderer cpp does not compile, Direct member access forces the compiler to know the definition of the constructor and destructor of TileRenderer already when compiling the client

	void Start() override
	{

		TextureID tex = Flux::FileLoader::LoadTileset("tileset.png", 14);
		if (tex == 0) {
			std::cerr << "Fehler beim Laden" << std::endl;
		}
		else {
			std::cout << "Textur geladen, ID: " << tex << std::endl;
		}

		//m_renderer.SetTileset(tex, 14, 112, 112); // TileRenderer cpp does not compile 

		m_asciiDictionary = ASCIIBlockDictionary();
		m_map = Flux::FileLoader::LoadTextFile("map.txt", &success);

		/*if (success)
		{
			for (const auto& line : lines)
				std::cout << line << "\n";
		}*/

		m_tiles = CreateRenderTiles(m_map, m_asciiDictionary);

		//m_texture = m_renderer.RenderToTexture(m_tiles, 40, 81, 10, 10); // TileRenderer cpp does not compile 

	}

	void Update(float deltaTime) override
	{
		if (Flux::Input::IsKeyPressed(FX_KEY_TAB))
			FX_INFO("Tab pressed");
	}

	void OnImGuiRender() override
	{
		Flux::ImGuiWrapper::Begin("@venture", ImVec2(1000, 100), ImVec2(100, 100), ImGuiWindowFlags_AlwaysVerticalScrollbar);
		//Flux::ImGuiWrapper::Text("Hello from @venture!");
		
		/*ImVec2 size = ImVec2(300, 300);
		ImGui::Image(m_texture, size); */ // ImGui Image Wrapper has some errors with the converting of the texture

		Flux::ImGuiWrapper::End();
	}
};


REGISTER_SCRIPT(AtventureScript);
