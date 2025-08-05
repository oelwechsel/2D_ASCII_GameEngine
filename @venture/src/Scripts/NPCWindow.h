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
	std::string m_name;

	float m_AnimationTime = 0.0f;
	int m_currentFrame = 0;

	const Flux::Entity* m_activeNPC = nullptr;

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
		m_AnimationTime = 0.5f; //TODO

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
	}

	void Update(float deltaTime) override
	{
		if (!m_showWindow || !m_activeNPC) return;

		float deltaSeconds = deltaTime / 1000.0f;
		m_AnimationTime += deltaSeconds;

		if (m_AnimationTime >= m_activeNPC->frameDuration)
		{
			m_AnimationTime = 0.0f;
			m_currentFrame = (m_currentFrame + 1) % m_activeNPC->layeredFrames.size();
		}
	}

	void OnImGuiRender() override
	{
		if (!m_showWindow || !m_activeNPC) return;

		ImGuiWindowFlags npcWindowFlags = ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_NoScrollWithMouse |
			ImGuiWindowFlags_NoCollapse;

		Flux::ImGuiWrapper::Begin(m_name.c_str(), m_windowSize, m_position, npcWindowFlags);

		const Flux::LayeredFrame& frame = m_activeNPC->layeredFrames[m_currentFrame];
		const float lineHeight = Flux::ImGuiWrapper::GetFontSize() + 1.0f;
		ImVec2 cursorPos = Flux::ImGuiWrapper::GetCursorScreenPos();

		ImU32 layerColors[] = {
			IM_COL32(255, 255, 255, 255),
			IM_COL32(255, 255, 255, 200),
			IM_COL32(255, 255, 255, 100),
			IM_COL32(255, 255, 255, 50)
		};

		float y = cursorPos.y;

		for (size_t layerIndex = 0; layerIndex < frame.layers.size(); ++layerIndex)
		{
			std::istringstream stream(frame.layers[layerIndex]);
			std::string line;

			while (std::getline(stream, line))
			{
				Flux::ImGuiWrapper::DrawTextAbsolute(ImVec2(cursorPos.x, y),
					layerColors[min(layerIndex, size_t(3))],
					line.c_str());
				y += lineHeight;
			}
		}

		Flux::ImGuiWrapper::End();
	}




	void OnDestroy() override {  }
};


REGISTER_SCRIPT(NPCWindow);
