#pragma once

#include <fxpch.h>

#include <Flux/Core.h>
#include <Flux/Events/Event.h>
#include <GLFW/glfw3.h>

namespace Flux 
{
	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& _title = "Flux Engine",
			unsigned int _width = 1280,
			unsigned int _height = 720)
			: Title(_title), Width(_width), Height(_height) 
		{
		}
	};

	class FLUX_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window();

		void OnUpdate();
		
		unsigned int GetWidth() const { return m_Data.Width;}
		unsigned int GetHeight() const { return m_Data.Height; }

		//Window attributes
		void SetEventCallback(const EventCallbackFn& _callback) { m_Data.EventCallback = _callback; }
		void SetVSync(bool _enabled);
		bool IsVSync() const;

		static std::unique_ptr<Window> Create(const WindowProps& _props = WindowProps());

	private:
		void Init(const WindowProps& _props);
		void Shutdown();

	private:
		GLFWwindow* m_Window;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
}

