#include "fxpch.h"
#include "Input.h"

#include "Flux/Application.h"
#include <GLFW/glfw3.h>



namespace Flux
{
    Input* Input::s_Instance = new Input();

    bool Flux::Input::IsKeyPressed(int _keycode)
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        auto state = glfwGetKey(window, _keycode);

        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    inline bool Flux::Input::IsMouseButtonPressed(int _button)
    {

        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        auto state = glfwGetMouseButton(window, _button);

        return state == GLFW_PRESS;
    }

    inline std::pair<float, float> Flux::Input::GetMousePosition()
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        return { (float)xpos, (float)ypos };
    }

    inline bool Flux::Input::GetMouseX()
    {
        // only in C++ 17
        auto [x, y] = GetMousePosition();
        return x;
    }

    inline bool Flux::Input::GetMouseY()
    {
        // only in C++ 17
        auto [x, y] = GetMousePosition();
        return y;
    }
}


