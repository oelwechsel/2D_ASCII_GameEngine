#pragma once
#include <Flux.h>

class NPCWindowScript : public Flux::IScript
{
	inline static NPCWindowScript* s_Instance = nullptr;

	//-------------------------------------------//
	//----------------Variables------------------//
	//-------------------------------------------//

public:
	bool m_ShowWindow = false;
	ImVec2 m_WindowSize;

	ImVec2 m_Position;
	int m_ScreenWidthWindows = GetSystemMetrics(SM_CXSCREEN);
	int m_ScreenHeightWindows = GetSystemMetrics(SM_CYSCREEN);

	std::string m_Name;

	float m_AnimationTime = 0.0f;
	int m_CurrentFrame = 0;

	const Flux::Entity* m_ActiveNPC = nullptr;

	std::vector<ImU32> m_LayerColors = {
			IM_COL32(255, 255, 255, 255),
			IM_COL32(255, 255, 255, 200),
			IM_COL32(255, 255, 255, 100),
			IM_COL32(255, 255, 255, 32),
			IM_COL32(255, 255, 255, 32),
	};


	ImGuiWindowFlags m_WindowFlags = 
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoScrollWithMouse |
		ImGuiWindowFlags_NoCollapse;


	//-------------------------------------------//
	//----------------Functions------------------//
	//-------------------------------------------//

public:

	static NPCWindowScript* Get()
	{
		return s_Instance;
	}

	static NPCWindowScript& Instance()
	{
		return *s_Instance;
	}

	void ShowNPCWindow(const Flux::Entity& _entity)
	{
		m_ActiveNPC = &_entity;

		m_Name = _entity.m_AsciiName;
		m_WindowSize = _entity.m_WindowSize;

		m_CurrentFrame = 0;
		m_AnimationTime = 0.0f;

		m_ShowWindow = true;
	}

	void HideNPCWindow() {
		m_ShowWindow = false;
	}

private:

	//---------------------------------------------------------------//
	//----------------predefined override functions------------------//
	//---------------------------------------------------------------//

	void Start() override
	{
		s_Instance = this;

		m_Position.x = m_ScreenWidthWindows * 0.17f;
		m_Position.y = m_ScreenHeightWindows * 0.3f;
	}

	void Update(float _deltaTime) override
	{
		if (m_ShowWindow && m_ActiveNPC)
		{
			m_AnimationTime += _deltaTime;

			if (m_AnimationTime >= m_ActiveNPC->m_FrameDuration)
			{
				m_AnimationTime = 0.0f;
				m_CurrentFrame = (m_CurrentFrame + 1) % m_ActiveNPC->m_LayeredFrames.size();
			}
		}
		else
		{
			m_CurrentFrame = 0;
		}
	}

	void OnImGuiRender() override
	{
		if (!m_ShowWindow || !m_ActiveNPC)
			return;

		if (!GameManagerVariables::Instance().m_IsInFight)
		{
			Flux::ImGuiWrapper::Begin(m_Name.c_str(), m_WindowSize, m_Position, m_WindowFlags);

			auto* drawList = Flux::ImGuiWrapper::GetWindowDrawList();
			ImVec2 startPos = Flux::ImGuiWrapper::GetCursorScreenPos();

			const auto& frame = m_ActiveNPC->m_LayeredFrames[m_CurrentFrame];

			Flux::ImGuiWrapper::AnimateLayers(drawList, startPos, frame.layers, m_LayerColors);

			Flux::ImGuiWrapper::End();
		}
	}
	void OnDestroy() override { s_Instance = nullptr; }
};


REGISTER_SCRIPT(NPCWindowScript);
