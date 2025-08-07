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
		if (s_Instance)
		{
			FX_CORE_ERROR("Application instance already exists!");
			throw std::runtime_error("Application already exists!");
		}
		s_Instance = this;

		try
		{
			m_Window = std::unique_ptr<Window>(Window::Create());
		}
		catch (const std::exception& e)
		{
			FX_CORE_ERROR("Window creation failed: {0}", e.what());
			throw;
		}

		FX_CORE_INFO("Window successfully created.");

		if (!m_Window)
		{
			FX_CORE_CRITICAL("Window is nullptr after creation.");
			throw std::runtime_error("Window creation failed: nullptr returned.");
		}

		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		// Script Manager
		m_ScriptManagerLayer = new(std::nothrow) ScriptManagerLayer();
		if (!m_ScriptManagerLayer)
		{
			FX_CORE_CRITICAL("Failed to allocate ScriptManagerLayer.");
			throw std::bad_alloc();
		}
		PushLayer(m_ScriptManagerLayer);
		FX_CORE_INFO("ScriptManagerLayer initialized and attached.");

		// ImGui Layer
		m_ImGuiLayer = new(std::nothrow) ImGuiLayer();
		if (!m_ImGuiLayer)
		{
			FX_CORE_CRITICAL("Failed to allocate ImGuiLayer.");
			throw std::bad_alloc();
		}
		PushOverlay(m_ImGuiLayer);
		FX_CORE_INFO("ImGuiLayer initialized and attached.");
	}


	Application::~Application()
	{
	}

	void Application::PushLayer(Layer* _layer)
	{
		if (!_layer)
		{
			FX_CORE_WARN("Tried to push nullptr as Layer.");
			return;
		}
		m_LayerStack.PushLayer(_layer);
		_layer->OnAttach();
		FX_CORE_INFO("Layer pushed: {0}", typeid(*_layer).name());
	}


	void Application::PushOverlay(Layer* _overlay)
	{
		m_LayerStack.PushOverlay(_overlay);
		_overlay->OnAttach();
	}

	void Application::OnEvent(Event& _e)
	{
		FX_CORE_TRACE("Event received: {0}", _e.ToString());

		EventDispatcher dispatcher(_e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			Layer* layer = *--it;
			if (!layer)
			{
				FX_CORE_WARN("Nullptr layer in stack during event dispatch.");
				continue;
			}

			layer->OnEvent(_e);
			if (_e.IsHandled())
			{
				FX_CORE_TRACE("Event handled by layer: {0}", typeid(*layer).name());
				break;
			}
		}
	}



	void Application::Run()
	{
		FX_CORE_INFO("Application loop started.");

		while (m_Running)
		{
			float time = GetTime();
			m_DeltaTime = time - lastFrameTime;
			lastFrameTime = time;

			glClearColor(0, 0, 0, 0);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : m_LayerStack)
			{
				if (!layer)
				{
					FX_CORE_WARN("Nullptr layer in update loop.");
					continue;
				}

				layer->OnUpdate(m_DeltaTime);
			}

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
			{
				if (!layer) continue;
				layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}

		FX_CORE_INFO("Application loop exited cleanly.");
	}


	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		FX_CORE_INFO("WindowCloseEvent received. Closing application.");
		m_Running = false;
		return true;
	}


	void Application::CloseApplication()
	{
		FX_CORE_INFO("Application manually closed via CloseApplication().");
		m_Running = false;
	}

}
