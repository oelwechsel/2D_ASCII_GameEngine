#include "fxpch.h"
#include "Window.h"
#include "Log.h" 

namespace Flux {

	static bool s_GLFWInitialized = false;

	std::unique_ptr<Window> Window::Create(const WindowProps& _props)
	{
		std::unique_ptr<Window> window = std::make_unique<Window>();
		window->Init(_props);
		return window;
	}

	Window::~Window()
	{
		Shutdown();
	}

	void Window::Init(const WindowProps& _props)
	{
		m_Data.Title = _props.Title;
		m_Data.Width = _props.Width;
		m_Data.Height = _props.Height;

		FX_CORE_INFO("Creating Window {0} ({1}, {0})", _props.Title, _props.Width, _props.Height);

		if (!s_GLFWInitialized) 
		{
			//TODO: glfwTerminate on system shutdown
			int success = glfwInit();
			FX_CORE_ASSERT(success, "Could not initialize GLFW!");

			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow((int)_props.Width, (int)_props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);
	}

	void Window::Shutdown()
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}

	void Window::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void Window::SetVSync(bool _enabled)
	{
		if (_enabled)
			glfwSwapInterval(1);
		else 
			glfwSwapInterval(0);

		m_Data.VSync = _enabled;
	}

	bool Window::IsVSync() const
	{
		return m_Data.VSync;
	}
}