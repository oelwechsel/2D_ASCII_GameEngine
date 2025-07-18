#pragma once

#include "Core.h"
#include "Window.h"
#include "Flux/LayerStack.h"
#include "Flux/Events/Event.h"
#include "Flux/Events/ApplicationEvent.h"


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
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		
		LayerStack m_LayerStack;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}


