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

	inline static MapRendererScript* s_Instance = nullptr;

	float m_timeAccumulator = 0.0f;
	float m_updateInterval = 1.0f / 10.0f;

	float m_deltaTime;
	
	//-------------------------------------------//
	//----------------Functions------------------//
	//-------------------------------------------//

public:
	
	void UpdateRenderTiles()
	{

		m_tiles = CreateRenderTiles(m_map, m_asciiDictionary, GameManagerScript::Instance().entities);
		m_needsUpdate = true;
	}

	static MapRendererScript* Get()
	{
		return s_Instance;
	}

	static MapRendererScript& Instance()
	{
		return *s_Instance;
	}

	const std::vector<std::string>& GetMap() const { return m_map; }
	
	bool MapRendererScript::IsBlockedTile(int x, int y) const
	{
		static const std::unordered_set<char> blockedTiles = { '#', '|', '-', '8', 'O', '=', '+' };

		if (y < 0 || y >= static_cast<int>(m_map.size()) || x < 0 || x >= static_cast<int>(m_map[y].size()))
			return true;

		char tile = m_map[y][x];
		return blockedTiles.count(tile) > 0;
	}

private:
	// define your private Functions here
	bool m_needsUpdate = true;

	//---------------------------------------------------------------//
	//----------------predefined override functions------------------//
	//---------------------------------------------------------------//

	void Start() override
	{
		s_Instance = this;

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
		m_deltaTime = deltaTime;
		m_timeAccumulator += deltaTime;

		if (m_timeAccumulator >= m_updateInterval)
		{
			m_tiles = CreateRenderTiles(m_map, m_asciiDictionary, GameManagerScript::Instance().entities);
			m_timeAccumulator = 0.0f;
		}

		m_texture = m_renderer.RenderToTexture(
			m_tiles, m_mapWidth, m_mapHeight,
			GameManagerScript::Instance().entities[0].x,
			GameManagerScript::Instance().entities[0].y);
	}

	void OnImGuiRender() override
	{
		// TODO: Add Camera and PLayer Dependencies!
		Flux::ImGuiWrapper::Begin("@venture", ImVec2(m_MapSize.x+15, m_MapSize.y+35), ImVec2(300, 300), ImGuiWindowFlags_NoCollapse);
		Flux::ImGuiWrapper::Image(m_texture, m_MapSize);

		double fps = 1.0 / m_deltaTime;
		Flux::ImGuiWrapper::Text("FPS: %.1f", fps);

		Flux::ImGuiWrapper::End();
	}

	void OnDestroy() override
	{
		s_Instance = nullptr;
	}
};

REGISTER_SCRIPT(MapRendererScript);
