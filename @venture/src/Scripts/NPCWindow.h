#pragma once
#include <Flux.h>

class NPCWindow : public Flux::IScript
{
	//-------------------------------------------//
	//----------------Variables------------------//
	//-------------------------------------------//

private:

	inline static NPCWindow* s_Instance = nullptr;

public:
	bool m_showWindow = false;
	ImVec2 m_windowSize;

	ImVec2 m_position;
	int m_screenWidthWindows = GetSystemMetrics(SM_CXSCREEN);
	int m_screenHeightWindows = GetSystemMetrics(SM_CYSCREEN);

	std::string m_name;

	float m_AnimationTime = 0.0f;
	int m_currentFrame = 0;

	const Flux::Entity* m_activeNPC = nullptr;

	std::vector<ImU32> m_layerColors = {
			IM_COL32(255, 255, 255, 255),
			IM_COL32(255, 255, 255, 200),
			IM_COL32(255, 255, 255, 100),
			IM_COL32(255, 255, 255, 32),
			IM_COL32(255, 255, 255, 32),
	};


	ImGuiWindowFlags m_windowFlags = 
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoScrollWithMouse |
		ImGuiWindowFlags_NoCollapse;


	//-------------------------------------------//
	//----------------Functions------------------//
	//-------------------------------------------//

public:

	static NPCWindow* Get()
	{
		return s_Instance;
	}

	static NPCWindow& Instance()
	{
		return *s_Instance;
	}

	void ShowNPCWindow(const Flux::Entity& _entity)
	{
		m_activeNPC = &_entity;

		m_name = _entity.ascii;
		m_windowSize = _entity.windowSize;

		m_currentFrame = 0;
		m_AnimationTime = 0.0f;

		m_showWindow = true;
	}

	void HideNPCWindow() {
		m_showWindow = false;
	}

private:

	//---------------------------------------------------------------//
	//----------------predefined override functions------------------//
	//---------------------------------------------------------------//

	void Start() override
	{
		s_Instance = this;

		m_position.x = m_screenWidthWindows * 0.15f;
		m_position.y = m_screenHeightWindows * 0.3f;
	}

	void Update(float deltaTime) override
	{
		if (m_showWindow && m_activeNPC)
		{
			m_AnimationTime += deltaTime;

			if (m_AnimationTime >= m_activeNPC->frameDuration)
			{
				m_AnimationTime = 0.0f;
				m_currentFrame = (m_currentFrame + 1) % m_activeNPC->layeredFrames.size();
			}
		}
		else
		{
			m_currentFrame = 0;
		}
	}


	void OnImGuiRender() override
	{
		if (!m_showWindow || !m_activeNPC)
			return;


		Flux::ImGuiWrapper::Begin(m_name.c_str(), m_windowSize, m_position, m_windowFlags);

		auto* drawList = Flux::ImGuiWrapper::GetWindowDrawList();
		ImVec2 startPos = Flux::ImGuiWrapper::GetCursorScreenPos();

		const auto& frame = m_activeNPC->layeredFrames[m_currentFrame];

		Flux::ImGuiWrapper::AnimateLayers(drawList, startPos, frame.layers, m_layerColors);

		Flux::ImGuiWrapper::End();

	}




	void OnDestroy() override {  }
};


REGISTER_SCRIPT(NPCWindow);
