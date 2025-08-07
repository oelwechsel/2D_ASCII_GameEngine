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

        

        ImGuiConsole();
        virtual ~ImGuiConsole();

        virtual void Draw(const char* title, const char* _welcomeMessage,
            const ImVec2& windowPos = ImVec2(-1, -1),
            const ImVec2& windowSize = ImVec2(-1, -1),
            bool* p_open = nullptr);

        virtual void WelcomeMessage(const char* _message);
        virtual void ExecCommand(const std::string& command) {}
        virtual void AutoComplete(const std::string& currentInput, std::vector<std::string>& suggestions) {}

        void AddLog(const char* fmt, ...) IM_FMTARGS(2);
        void AddLog(LogLevel level, const char* fmt, ...) IM_FMTARGS(3);

        using CommandFunc = std::function<void(const std::string&)>;

        struct CommandEntry {
            CommandFunc function;
            std::string description;
        };
        std::unordered_map<std::string, CommandEntry> CommandMap;


        void RegisterCommand(const std::string& name, const std::string& description, CommandFunc func);
        bool ExecuteCommand(const std::string& input, std::string& outError);
        bool InputEnabled = true;

    protected:
        
        char InputBuf[512];
        std::vector<std::string> History;
        std::vector<char*> Items;
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
