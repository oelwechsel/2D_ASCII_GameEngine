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

	TextureID tex2;

	Flux::TileRenderer m_renderer; // TileRenderer cpp does not compile, Direct member access forces the compiler to know the definition of the constructor and destructor of TileRenderer already when compiling the client
	//std::unique_ptr<Flux::TileRenderer> m_renderer;


	void Start() override
	{


		tex2 = Flux::FileLoader::LoadTileset("testbild.png", 14);
		if (tex2 == 0) {
			std::cerr << "Fehler beim Laden" << std::endl;
		}
		else {
			std::cout << "Textur geladen, ID: " << tex2 << std::endl;
		}














		TextureID tex = Flux::FileLoader::LoadTileset("tileset.png", 14);
		if (tex == 0) {
			std::cerr << "Fehler beim Laden" << std::endl;
		}
		else {
			std::cout << "Textur geladen, ID: " << tex << std::endl;
		}

		//m_renderer = std::make_unique<Flux::TileRenderer>();
		m_renderer.SetTileset(tex, 14, 224, 84); // TileRenderer cpp does not compile 

		m_asciiDictionary = ASCIIBlockDictionary();
		m_map = Flux::FileLoader::LoadTextFile("map.txt", &success);

		/*if (success)
		{
			for (const auto& line : lines)
				std::cout << line << "\n";
		}*/

		m_tiles = CreateRenderTiles(m_map, m_asciiDictionary);


		std::cout << "Tile count: " << m_tiles.size() << std::endl;
	}

	void Update(float deltaTime) override
	{
		if (Flux::Input::IsKeyPressed(FX_KEY_W))
			FX_INFO("W pressed");


		auto [x, y] = Flux::Input::GetMousePosition();
		FX_CORE_TRACE("{0}, {1}", x, y);


		m_texture = m_renderer.RenderToTexture(m_tiles, 80, 40, x, y); // TileRenderer cpp does not compile 
		FX_INFO(m_texture);
	}

	void OnImGuiRender() override
	{


		Flux::ImGuiWrapper::Begin("lol", ImVec2(350, 350), ImVec2(600, 600), ImGuiWindowFlags_AlwaysVerticalScrollbar);

		ImVec2 size1 = ImVec2(300, 300);
		Flux::ImGuiWrapper::Image(tex2, size1);

		Flux::ImGuiWrapper::End();



		
		Flux::ImGuiWrapper::Begin("@venture", ImVec2(1000, 500), ImVec2(300, 300), ImGuiWindowFlags_AlwaysVerticalScrollbar);
		//Flux::ImGuiWrapper::Text("Hello from @venture!");
		
		ImVec2 size = ImVec2(80*14, 40*14);

		if (m_texture != 0)
			Flux::ImGuiWrapper::Image(m_texture, size);
		else
			Flux::ImGuiWrapper::Text("Texture render failed!");


		Flux::ImGuiWrapper::End();
		
	}
	
};


REGISTER_SCRIPT(AtventureScript);
