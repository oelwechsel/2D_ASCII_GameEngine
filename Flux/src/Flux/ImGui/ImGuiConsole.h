#pragma once
#include "Flux/Core.h"
#include "../vendor/imgui/imgui.h"
#include <functional>
#include <unordered_map>

namespace Flux
{
    class FLUX_API ImGuiConsole {
    public:
        enum class LogLevel { e_Info, e_Warning, e_Error };

        struct CustomColorEntry {
            std::string Text;
            ImVec4 Color;
        };

        ImGuiConsole();
        virtual ~ImGuiConsole();

        virtual void Draw(const char* _title, const char* _welcomeMessage,
            const ImVec2& _windowPos, const ImVec2& _windowSize, bool* _out_hasFocus = nullptr, bool* _p_open = nullptr);

        virtual void WelcomeMessage(const char* _message);
        virtual void ExecCommand(const std::string& _command) {}
        virtual void AutoComplete(const std::string& _currentInput, std::vector<std::string>& _suggestions) {}

        void AddLog(const char* fmt, ...) IM_FMTARGS(2);
        void AddLog(LogLevel _level, const char* fmt, ...) IM_FMTARGS(3);
        void AddCustomLog(const std::string& _customLevel, const ImVec4& _color, const char* fmt, ...) IM_FMTARGS(4);

        using CommandFunc = std::function<void(const std::string&)>;

        struct CommandEntry {
            CommandFunc function;
            std::string description;
        };
        std::unordered_map<std::string, CommandEntry> m_CommandMap;

        std::vector<char*> m_Items;
        std::unordered_map<const char*, ImVec4> m_CustomColorMap;



        void RegisterCommand(const std::string& _name, const std::string& _description, CommandFunc _func);
        bool ExecuteCommand(const std::string& _input, std::string& _outError);
        bool m_InputEnabled = true;

    protected:
        
        char InputBuf[512];
        std::vector<std::string> m_History;
                int m_HistoryPos = -1;
        bool m_ScrollToBottom = false;
        bool m_Initialized = false;
       

        ImVec2 m_WindowPos = ImVec2(-1, -1);
        ImVec2 m_WindowSize = ImVec2(-1, -1);

        bool m_UseCustomStyle = false;
        ImVec4 m_DefaultTextColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

        void ClearLog();

    private:
        void AddItem(const std::string& _msg);
    };
}
