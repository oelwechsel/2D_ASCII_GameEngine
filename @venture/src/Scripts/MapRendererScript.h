#pragma once
#include <Flux.h>
#include "ASCIIBlockDictionary.h"
#include "internal/MapToRenderTiles.h"
#include "GameManagerScript.h"

class MapRendererScript : public Flux::IScript
{
	//-------------------------------------------//
	//----------------Variables------------------//
	//-------------------------------------------//

private:
	
	

	Flux::TileRenderer m_renderer;
	std::vector<std::string> m_map;

	ASCIIBlockDictionary m_asciiDictionary;
	std::vector<Flux::RenderTile> m_tiles;
	unsigned int m_texture;
	bool success;

	int m_tileSize = 14;
	int m_tileSetWidth = 224;
	int m_tileSetHeight = 84;

	int m_mapWidth = 80;
	int m_mapHeight = 40;

	ImVec2 m_MapSize;
	
	//-------------------------------------------//
	//----------------Functions------------------//
	//-------------------------------------------//

public:
	// define your public Functions here
	

private:
	// define your private Functions here
	

	//---------------------------------------------------------------//
	//----------------predefined override functions------------------//
	//---------------------------------------------------------------//

	void Start() override
	{
		m_asciiDictionary = ASCIIBlockDictionary();

		m_map = Flux::FileLoader::LoadTextFile("map.txt", &success);
		TextureID tex = Flux::FileLoader::LoadTileset("tileset.png", 14);
		if (tex == 0) 
		{
			FX_ERROR("Error while Loading Texture");
		}
		else 
		{
			FX_INFO("Texture loaded, TextureID: {0}", tex);
		}

		m_renderer.SetTileset(tex, m_tileSize, m_tileSetWidth, m_tileSetHeight); 
		m_tiles = CreateRenderTiles(m_map, m_asciiDictionary, GameManagerScript::Instance().entities);


		// TODO: REMOVE 1,1 WITH PLAYER COORDINATES
		// POTENTIAL ERROR! GAME MANAGER NOT INITIALIZED BEFORE USED HERE?!
		m_texture = m_renderer.RenderToTexture(m_tiles, m_mapWidth, m_mapHeight, GameManagerScript::Instance().entities[0].x, GameManagerScript::Instance().entities[0].y);

		m_MapSize = ImVec2(m_mapWidth * m_tileSize, m_mapHeight * m_tileSize);
	}

	void Update(float deltaTime) override
	{
	}

	void OnImGuiRender() override
	{
		// TODO: Add Camera and PLayer Dependencies!
		Flux::ImGuiWrapper::Begin("@venture", ImVec2(m_MapSize.x+15, m_MapSize.y+35), ImVec2(300, 300), ImGuiWindowFlags_NoCollapse);
		Flux::ImGuiWrapper::Image(m_texture, m_MapSize);
		Flux::ImGuiWrapper::End();
	}
};

REGISTER_SCRIPT(MapRendererScript);
