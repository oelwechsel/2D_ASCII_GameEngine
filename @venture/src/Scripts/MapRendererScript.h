#pragma once
#include <Flux.h>
#include "_internal/MapToRenderTiles.h"
#include "_internal/customizables/Dictionary/ASCIIBlockDictionary.h"

#include "GameManagerVariables.h"

class MapRendererScript : public Flux::IScript
{
	static MapRendererScript* s_Instance;

	inline static std::unordered_set<char> m_BlockedTiles =
	{ '#', '|', '-', '8', '=', '+', 'f', 'B', 'P', '0', '(', '~' };

	//-------------------------------------------//
	//----------------Variables------------------//
	//-------------------------------------------//

private:
	
	Flux::TileRenderer m_Renderer;
	std::vector<std::string> m_Map;

	std::vector<Flux::RenderTile> m_Tiles;
	unsigned int m_Texture;
	bool m_Success;

	int m_TileSize = 14;
	int m_TileSetWidth = 224;
	int m_TileSetHeight = 84;

	int m_MapWidth = 80;
	int m_MapHeight = 40;

	ImVec2 m_MapSize;

	float m_TimeAccumulator = 0.0f;
	float m_UpdateInterval = 1.0f / 10.0f;

	float m_DeltaTime;

	int m_CameraWidth = 30; 
	int m_CameraHeight = 20; 


	ImVec2 m_Position;
	int m_ScreenWidthWindows = GetSystemMetrics(SM_CXSCREEN);
	int m_ScreenHeightWindows = GetSystemMetrics(SM_CYSCREEN);

	ImVec2 m_Scale;
	
	//-------------------------------------------//
	//----------------Functions------------------//
	//-------------------------------------------//

public:
	
	void UpdateRenderTiles()
	{
		m_Tiles = CreateRenderTiles(m_Map, ASCIIBlockDictionary::Instance(), GameManagerScript::Instance().m_Entities);
		m_NeedsUpdate = true;
	}

	static MapRendererScript* Get()
	{
		return s_Instance;
	}

	static MapRendererScript& Instance()
	{
		return *s_Instance;
	}

	const std::vector<std::string>& GetMap() const { return m_Map; }
	
	bool MapRendererScript::IsTileBlocked(int x, int y) const
	{
		if (y < 0 || y >= static_cast<int>(m_Map.size()) || x < 0 || x >= static_cast<int>(m_Map[y].size()))
			return true;

		char tile = m_Map[y][x];
		return m_BlockedTiles.count(tile) > 0;
	}

private:
	// define your private Functions here
	bool m_NeedsUpdate = true;

	//---------------------------------------------------------------//
	//----------------predefined override functions------------------//
	//---------------------------------------------------------------//

	void Start() override
	{
		if (s_Instance && s_Instance != this)
		{
			FX_WARN("GameManagerScript: Instance already exists. Ignoring this one.");
			return;
		}

		s_Instance = this;

		m_Map = Flux::FileLoader::LoadTextFile("map.txt", &m_Success);
		TextureID tex = Flux::FileLoader::LoadTileset("tileset.png", 14);
		if (tex == 0) 
		{
			FX_ERROR("Error while Loading Texture");
		}
		else 
		{
			FX_INFO("Texture loaded, TextureID: {0}", tex);
		}

		m_Renderer.SetTileset(tex, m_TileSize, m_TileSetWidth, m_TileSetHeight); 
		m_Tiles = CreateRenderTiles(m_Map, ASCIIBlockDictionary::Instance(), GameManagerScript::Instance().m_Entities);

		m_Texture = m_Renderer.RenderToTexture(m_Tiles, m_MapWidth, m_MapHeight, GameManagerScript::Instance().m_Entities[0].m_xPos, GameManagerScript::Instance().m_Entities[0].m_yPos);
		m_MapSize = ImVec2(m_MapWidth * m_TileSize, m_MapHeight * m_TileSize);

		m_Position.x = m_ScreenWidthWindows * 0.30f;
		m_Position.y = m_ScreenHeightWindows * 0.3f;
	}

	void Update(float _deltaTime) override
	{
		m_DeltaTime = _deltaTime;
		m_TimeAccumulator += _deltaTime;

		if (m_TimeAccumulator >= m_UpdateInterval)
		{
			m_Tiles = CreateRenderTiles(m_Map, ASCIIBlockDictionary::Instance(), GameManagerScript::Instance().m_Entities);
			m_TimeAccumulator = 0.0f;
		}

		int playerX = GameManagerScript::Instance().m_Entities[0].m_xPos;
		int playerY = GameManagerScript::Instance().m_Entities[0].m_yPos;

		int startX = playerX - m_CameraWidth / 2;
		int startY = playerY - m_CameraHeight / 2;

		std::vector<Flux::RenderTile> visibleTiles;
		for (const auto& tile : m_Tiles)
		{
			if (tile.x >= startX && tile.x < startX + m_CameraWidth &&
				tile.y >= startY && tile.y < startY + m_CameraHeight)
			{
				visibleTiles.push_back(tile);
			}
		}

		m_Texture = m_Renderer.RenderToTexture(
			visibleTiles,
			m_CameraWidth,
			m_CameraHeight,
			startX,
			startY
		);

		m_MapSize = ImVec2(m_CameraWidth * m_TileSize, m_CameraHeight * m_TileSize);
	}


	void OnImGuiRender() override
	{
		if (!GameManagerVariables::Instance().m_IsInFight)
		{
			Flux::ImGuiWrapper::Begin("@venture", ImVec2(m_MapSize.x * 2 , m_MapSize.y * 2 ), m_Position, ImGuiWindowFlags_NoCollapse);

			ImVec2 availSize = Flux::ImGuiWrapper::GetContentRegionAvail();

			float scaleX = availSize.x / m_MapSize.x;
			float scaleY = availSize.y / m_MapSize.y;
			float rawScale = min(scaleX, scaleY);

			float snappedScale = std::floor(rawScale * 4.0f) / 4.0f;
			static float currentScale = 2.0f;
			if (std::abs(snappedScale - currentScale) > 0.01f)
				currentScale = snappedScale;

			ImVec2 scaledMapSize = ImVec2(m_MapSize.x * currentScale, m_MapSize.y * currentScale);

			Flux::ImGuiWrapper::Image(m_Texture, scaledMapSize);

			Flux::ImGuiWrapper::End();
		}
	}


	void OnDestroy() override
	{
		if (s_Instance == this)
			s_Instance = nullptr;
	}
};

inline MapRendererScript* MapRendererScript::s_Instance = nullptr;

REGISTER_SCRIPT(MapRendererScript);
