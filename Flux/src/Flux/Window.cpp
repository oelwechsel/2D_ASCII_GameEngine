#include "fxpch.h"
#include "Window.h"
#include "Log.h" 
#include "Flux/Events/ApplicationEvent.h"
#include "Flux/Events/MouseEvent.h"
#include "Flux/Events/KeyEvent.h"
#include "GL/glew.h"
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>


namespace Flux {

	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int _error, const char* _description)
	{
		FX_CORE_ERROR("GLFW ERROR ({0}): {1}", _error, _description);
	}

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
			int success = glfwInit();
			FX_CORE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);

			s_GLFWInitialized = true;
		}

		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
		glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
		glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

		m_Window = glfwCreateWindow((int)_props.Width, (int)_props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);

		GLenum err = glewInit();
		FX_CORE_ASSERT(err != GLEW_OK, "Could not initialize GLEW!")

			glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* _window, int _width, int _height)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(_window);

				data.Width = _width;
				data.Height = _height;

				WindowResizeEvent event(_width, _height);
				data.EventCallback(event);
			});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* _window)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(_window);

				WindowCloseEvent event;
				data.EventCallback(event);
			});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* _window, int _key, int _scancode, int _action, int _mods)
			{ 
				// placeholder
			});

		glfwSetCharCallback(m_Window, [](GLFWwindow* _window, unsigned int _keycode)
			{
				// placeholder
			});


		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* _window, int _button, int _action, int _mods)
			{
				ImGui_ImplGlfw_MouseButtonCallback(_window, _button, _action, _mods);

				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(_window);

				switch (_action)
				{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(_button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(_button);
					data.EventCallback(event);
					break;
				}
				}
			});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* _window, double _xOffset, double _yOffset)
			{
				ImGui_ImplGlfw_ScrollCallback(_window, _xOffset, _yOffset);

				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(_window);
				MouseScrolledEvent event((float)_xOffset, (float)_yOffset);
				data.EventCallback(event);
			});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* _window, double _xPos, double _yPos)
			{
				ImGuiIO& io = ImGui::GetIO();
				io.MousePos = ImVec2((float)_xPos, (float)_yPos); 
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(_window);
				MouseMovedEvent event((float)_xPos, (float)_yPos);
				data.EventCallback(event);
			});

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