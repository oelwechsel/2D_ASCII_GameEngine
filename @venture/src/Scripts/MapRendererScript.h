#pragma once
#include <Flux.h>
#include "_internal/customizables/Dictionary/ASCIIBlockDictionary.h"
#include "_internal/MapToRenderTiles.h"
#include "GameManagerScript.h"

class MapRendererScript : public Flux::IScript
{
	//-------------------------------------------//
	//----------------Variables------------------//
	//-------------------------------------------//

private:
	
	Flux::TileRenderer m_renderer;
	std::vector<std::string> m_map;

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

	int m_cameraWidth = 30; 
	int m_cameraHeight = 20; 


	ImVec2 m_position;
	int m_screenWidthWindows = GetSystemMetrics(SM_CXSCREEN);
	int m_screenHeightWindows = GetSystemMetrics(SM_CYSCREEN);
	
	//-------------------------------------------//
	//----------------Functions------------------//
	//-------------------------------------------//

public:
	
	void UpdateRenderTiles()
	{

		m_tiles = CreateRenderTiles(m_map, ASCIIBlockDictionary::Instance(), GameManagerScript::Instance().entities);
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
	
	bool MapRendererScript::IsTileBlocked(int x, int y) const
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
		m_tiles = CreateRenderTiles(m_map, ASCIIBlockDictionary::Instance(), GameManagerScript::Instance().entities);


		
		// POTENTIAL ERROR! GAME MANAGER NOT INITIALIZED BEFORE USED HERE?!
		m_texture = m_renderer.RenderToTexture(m_tiles, m_mapWidth, m_mapHeight, GameManagerScript::Instance().entities[0].x, GameManagerScript::Instance().entities[0].y);

		m_MapSize = ImVec2(m_mapWidth * m_tileSize, m_mapHeight * m_tileSize);


		m_position.x = m_screenWidthWindows * 0.25f;
		m_position.y = m_screenHeightWindows * 0.3f;
	}

	void Update(float deltaTime) override
	{
		m_deltaTime = deltaTime;
		m_timeAccumulator += deltaTime;

		if (m_timeAccumulator >= m_updateInterval)
		{
			m_tiles = CreateRenderTiles(m_map, ASCIIBlockDictionary::Instance(), GameManagerScript::Instance().entities);
			m_timeAccumulator = 0.0f;
		}

		int playerX = GameManagerScript::Instance().entities[0].x;
		int playerY = GameManagerScript::Instance().entities[0].y;

		int startX = playerX - m_cameraWidth / 2;
		int startY = playerY - m_cameraHeight / 2;

		std::vector<Flux::RenderTile> visibleTiles;
		for (const auto& tile : m_tiles)
		{
			if (tile.x >= startX && tile.x < startX + m_cameraWidth &&
				tile.y >= startY && tile.y < startY + m_cameraHeight)
			{
				visibleTiles.push_back(tile);
			}
		}

		m_texture = m_renderer.RenderToTexture(
			visibleTiles,
			m_cameraWidth,
			m_cameraHeight,
			startX,
			startY
		);

		m_MapSize = ImVec2(m_cameraWidth * m_tileSize, m_cameraHeight * m_tileSize);
	}


	void OnImGuiRender() override
	{
		// TODO: Add Camera and PLayer Dependencies!
		Flux::ImGuiWrapper::Begin("@venture", ImVec2(m_MapSize.x+15, m_MapSize.y+35), m_position, ImGuiWindowFlags_NoCollapse);
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
