#pragma once

#include "Core.h"
#include "Window.h"
#include "Flux/LayerStack.h"
#include "Flux/Events/Event.h"
#include "Flux/Events/ApplicationEvent.h"

#include "Flux/ImGui/ImGuiLayer.h"
#include "Flux/ScriptManagerLayer.h"


namespace Flux
{
	class FLUX_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* _layer);
		void PushOverlay(Layer* _overlay);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
		float GetDeltaTime() const { return m_DeltaTime; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		ScriptManagerLayer* m_ScriptManagerLayer;
		bool m_Running = true;
		
		LayerStack m_LayerStack;
	private:
		static Application* s_Instance;
		float m_DeltaTime;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}


