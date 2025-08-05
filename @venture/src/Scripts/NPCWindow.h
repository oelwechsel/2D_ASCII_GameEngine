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
		m_name = _entity.ascii;
		m_windowSize = _entity.windowSize;

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
	}

	void OnImGuiRender() override
	{
		if (m_showWindow) {
			Flux::ImGuiWrapper::Begin(m_name.c_str(), m_windowSize, ImVec2(200,200), ImGuiWindowFlags_AlwaysVerticalScrollbar);
			Flux::ImGuiWrapper::Text("Hello from NPCWindow!");
			Flux::ImGuiWrapper::End();
		}
	}


	void OnDestroy() override {  }
};


REGISTER_SCRIPT(NPCWindow);
