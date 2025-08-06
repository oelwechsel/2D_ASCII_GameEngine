#pragma once
#include "Flux/Core.h"
#include "../vendor/imgui/imgui.h"

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
        virtual void ExecCommand(const std::string& command) = 0;
        virtual void AutoComplete(const std::string& currentInput, std::vector<std::string>& suggestions) {}

    protected:
        char InputBuf[512];
        std::vector<char*> History;
        std::vector<char*> Items;
        int HistoryPos = -1; // -1: neue Eingabe, 0..n: History-Eintrag
        bool ScrollToBottom = false;
        bool Initialized = false;

        ImVec2 WindowPos = ImVec2(-1, -1); // -1: nicht setzen
        ImVec2 WindowSize = ImVec2(-1, -1); // -1: nicht setzen

        bool UseCustomStyle = false;
        ImVec4 BackgroundColor = ImVec4(0.1f, 0.1f, 0.1f, 1.0f); // Standard-Dunkelgrau
        ImVec4 DefaultTextColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // Weiﬂ

        void AddLog(const char* fmt, ...) IM_FMTARGS(2);
        void AddLog(LogLevel level, const char* fmt, ...) IM_FMTARGS(3);
        void ClearLog();

    private:
        void AddItem(const std::string& msg);
    };
}

