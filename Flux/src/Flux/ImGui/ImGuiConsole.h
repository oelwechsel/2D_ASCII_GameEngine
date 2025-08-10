#pragma once
#include "Flux/Core.h"
#include "../vendor/imgui/imgui.h"
#include <functional>
#include <unordered_map>

namespace Flux
{
    class FLUX_API ImGuiConsole {
    public:
        enum class LogLevel { Info, Warning, Error };

        struct CustomColorEntry {
            std::string Text;
            ImVec4 Color;
        };

        ImGuiConsole();
        virtual ~ImGuiConsole();

        virtual void Draw(const char* title, const char* _welcomeMessage,
            const ImVec2& windowPos, const ImVec2& windowSize, bool* out_hasFocus = nullptr, bool* p_open = nullptr);

        virtual void WelcomeMessage(const char* _message);
        virtual void ExecCommand(const std::string& command) {}
        virtual void AutoComplete(const std::string& currentInput, std::vector<std::string>& suggestions) {}

        void AddLog(const char* fmt, ...) IM_FMTARGS(2);
        void AddLog(LogLevel level, const char* fmt, ...) IM_FMTARGS(3);
        void AddCustomLog(const std::string& customLevel, const ImVec4& color, const char* fmt, ...) IM_FMTARGS(4);

        using CommandFunc = std::function<void(const std::string&)>;

        struct CommandEntry {
            CommandFunc function;
            std::string description;
        };
        std::unordered_map<std::string, CommandEntry> CommandMap;

        std::vector<char*> Items;
        std::unordered_map<const char*, ImVec4> CustomColorMap;



        void RegisterCommand(const std::string& name, const std::string& description, CommandFunc func);
        bool ExecuteCommand(const std::string& input, std::string& outError);
        bool InputEnabled = true;

    protected:
        
        char InputBuf[512];
        std::vector<std::string> History;
                int HistoryPos = -1;
        bool ScrollToBottom = false;
        bool Initialized = false;
       

        ImVec2 WindowPos = ImVec2(-1, -1);
        ImVec2 WindowSize = ImVec2(-1, -1);

        bool UseCustomStyle = false;
        ImVec4 DefaultTextColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

        void ClearLog();

    private:
        void AddItem(const std::string& msg);
    };
}
