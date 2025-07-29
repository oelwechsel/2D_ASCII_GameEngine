#include "fxpch.h"
#include "Application.h"


#include "Flux/Log.h"
#include <GL/glew.h>

#include "imgui.h"
//#include "backends/imgui_impl_glfw.h"
//#include "backends/imgui_impl_opengl3.h"

#include "Input.h"

namespace Flux 
{
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;


	static float GetTime()
	{
		return (float)glfwGetTime();
	}

	float lastFrameTime = GetTime();

	Application::Application()
	{
		FX_CORE_ASSERT(s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_ScriptManagerLayer = new ScriptManagerLayer();
		PushLayer(m_ScriptManagerLayer);

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

	}

	Application::~Application()
	{
	}

	void Application::PushLayer(Layer* _layer)
	{
		m_LayerStack.PushLayer(_layer);
		_layer->OnAttach();
	}

	void Application::PushOverlay(Layer* _overlay)
	{
		m_LayerStack.PushOverlay(_overlay);
		_overlay->OnAttach();
	}

	void Application::OnEvent(Event& _e)
	{
		EventDispatcher dispatcher(_e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(_e);
			if (_e.IsHandled())
				break;
		}
	}


	void Application::Run() 
	{
		while (m_Running)
		{

			//Delta Time
			float time = GetTime();
			m_DeltaTime = time - lastFrameTime;
			lastFrameTime = time;

			glClearColor(0, 0, 0, 0);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate(m_DeltaTime);

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();  //Render all ImGuiWindows
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
}
