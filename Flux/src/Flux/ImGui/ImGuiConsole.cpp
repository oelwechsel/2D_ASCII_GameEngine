#include "fxpch.h"
#include "ImGuiConsole.h"

namespace Flux
{
    ImGuiConsole::ImGuiConsole() {
        ClearLog();
        memset(InputBuf, 0, sizeof(InputBuf));
        HistoryPos = -1;
    }

    ImGuiConsole::~ImGuiConsole() {
        ClearLog();
    }

    void ImGuiConsole::ClearLog() {
        for (auto& item : Items)
            free(item);
        Items.clear();
    }

    void ImGuiConsole::AddItem(const std::string& msg) {
        Items.push_back(strdup(msg.c_str()));
        ScrollToBottom = true;
    }

    void ImGuiConsole::AddLog(const char* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        char buf[1024];
        vsnprintf(buf, sizeof(buf), fmt, args);
        buf[sizeof(buf) - 1] = 0;
        va_end(args);
        AddItem(buf);
    }

    void ImGuiConsole::AddLog(LogLevel level, const char* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        char buf[1024];
        vsnprintf(buf, sizeof(buf), fmt, args);
        buf[sizeof(buf) - 1] = 0;
        va_end(args);

        std::string prefix;
        switch (level) {
        case LogLevel::Info:    prefix = "[INFO] "; break;
        case LogLevel::Warning: prefix = "[WARN] "; break;
        case LogLevel::Error:   prefix = "[ERROR] "; break;
        }

        AddItem(prefix + buf);
    }

    void ImGuiConsole::WelcomeMessage(const char* _message) {
        if (!Initialized) 
        {
            AddLog(_message);
            Initialized = true;
        }
    }

    void ImGuiConsole::Draw(const char* title, const char* _welcomeMessage,
        const ImVec2& windowPos, const ImVec2& windowSize, bool* p_open)
    {

        WelcomeMessage(_welcomeMessage);

        // Setze Fensterposition und -größe nur, wenn gültig (>= 0)
        if (windowPos.x >= 0 && windowPos.y >= 0)
            ImGui::SetNextWindowPos(windowPos, ImGuiCond_FirstUseEver);
        if (windowSize.x >= 0 && windowSize.y >= 0)
            ImGui::SetNextWindowSize(windowSize, ImGuiCond_FirstUseEver);

        if (!ImGui::Begin(title, p_open, ImGuiWindowFlags_NoScrollbar)) {
            ImGui::End();
            return;
        }

        if (UseCustomStyle) {
            ImGui::PushStyleColor(ImGuiCol_WindowBg, BackgroundColor);
            ImGui::PushStyleColor(ImGuiCol_Text, DefaultTextColor);
        }

        ImGui::BeginChild("ScrollingRegion", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), false,
            ImGuiWindowFlags_HorizontalScrollbar);

        for (const auto& item : Items) {
            ImVec4 color;
            bool has_color = false;
            if (strstr(item, "[ERROR]")) {
                color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f);
                has_color = true;
            }
            else if (strstr(item, "[WARN]")) {
                color = ImVec4(1.0f, 1.0f, 0.4f, 1.0f);
                has_color = true;
            }

            if (has_color)
                ImGui::PushStyleColor(ImGuiCol_Text, color);

            ImGui::TextUnformatted(item);

            if (has_color)
                ImGui::PopStyleColor();
        }

        if (ScrollToBottom)
            ImGui::SetScrollHereY(1.0f);
        ScrollToBottom = false;

        ImGui::EndChild();

        ImGui::Separator();

        bool reclaim_focus = false;
        if (ImGui::InputText("Input", InputBuf, sizeof(InputBuf),
            ImGuiInputTextFlags_EnterReturnsTrue |
            ImGuiInputTextFlags_CallbackCompletion |
            ImGuiInputTextFlags_CallbackHistory,
            [](ImGuiInputTextCallbackData* data) -> int {
                ImGuiConsole* console = (ImGuiConsole*)data->UserData;

                if (data->EventFlag == ImGuiInputTextFlags_CallbackCompletion) {
                    std::vector<std::string> suggestions;
                    console->AutoComplete(data->Buf, suggestions);
                    if (!suggestions.empty()) {
                        strncpy(data->Buf, suggestions[0].c_str(), data->BufTextLen);
                        data->Buf[data->BufTextLen - 1] = '\0';
                        data->CursorPos = (int)strlen(data->Buf);
                        data->SelectionStart = data->SelectionEnd = data->CursorPos;
                    }
                }
                else if (data->EventFlag == ImGuiInputTextFlags_CallbackHistory) {
                    if (data->EventKey == ImGuiKey_UpArrow) {
                        if (console->HistoryPos == -1)
                            console->HistoryPos = static_cast<int>(console->History.size()) - 1;
                        else if (console->HistoryPos > 0)
                            console->HistoryPos--;
                    }
                    else if (data->EventKey == ImGuiKey_DownArrow) {
                        if (console->HistoryPos != -1)
                            if (++console->HistoryPos >= (int)console->History.size())
                                console->HistoryPos = -1;
                    }

                    if (console->HistoryPos >= 0) {
                        strncpy(data->Buf, console->History[console->HistoryPos], data->BufTextLen);
                        data->Buf[data->BufTextLen - 1] = '\0';
                    }
                    else {
                        data->Buf[0] = '\0';
                    }

                    data->BufDirty = true;
                }

                return 0;
            }, (void*)this)) {
            std::string cmd(InputBuf);
            ExecCommand(cmd);

            if (History.empty() || strcmp(History.back(), cmd.c_str()) != 0)
                History.push_back(strdup(cmd.c_str()));

            strcpy(InputBuf, "");
            reclaim_focus = true;
        }

        if (reclaim_focus)
            ImGui::SetKeyboardFocusHere(-1);

        if (UseCustomStyle)
            ImGui::PopStyleColor(2); 

        ImGui::End();
    }


    
}
