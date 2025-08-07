#include "fxpch.h"
#include "ImGuiConsole.h"
#include <imgui_internal.h>
#include "../Log.h"

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
		if (!Initialized) {
			AddLog(_message);
			Initialized = true;
		}
	}

	void ImGuiConsole::Draw(const char* title, const char* _welcomeMessage,
		const ImVec2& windowPos, const ImVec2& windowSize, bool* out_hasFocus, bool* p_open)
	{
		WelcomeMessage(_welcomeMessage);

		if (windowPos.x >= 0 && windowPos.y >= 0)
			ImGui::SetNextWindowPos(windowPos, ImGuiCond_FirstUseEver);
		if (windowSize.x >= 0 && windowSize.y >= 0)
			ImGui::SetNextWindowSize(windowSize, ImGuiCond_FirstUseEver);

		if (!ImGui::Begin(title, p_open, ImGuiWindowFlags_NoScrollbar)) {
			ImGui::End();
			return;
		}

		if (out_hasFocus)
			*out_hasFocus = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);

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
			else if (strstr(item, "[INFO]")) {
				color = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);  // Oder DefaultTextColor, je nach Wunsch
				has_color = true;
			}

			if (has_color) {
				ImGui::PushStyleColor(ImGuiCol_Text, color);
				ImGui::TextUnformatted(item);
				ImGui::PopStyleColor();
			}
			else {
				// Kein spezieller Farb-Push, einfach DefaultTextColor direkt setzen:
				ImGui::PushStyleColor(ImGuiCol_Text, DefaultTextColor);
				ImGui::TextUnformatted(item);
				ImGui::PopStyleColor();
			}
		}

		if (ScrollToBottom)
			ImGui::SetScrollHereY(1.0f);
		ScrollToBottom = false;

		ImGui::EndChild();
		ImGui::Separator();

		if (UseCustomStyle)
			ImGui::PushStyleColor(ImGuiCol_Text, DefaultTextColor);

		bool reclaim_focus = false;
		if (InputEnabled && ImGui::InputText("Input", InputBuf, sizeof(InputBuf),
			ImGuiInputTextFlags_EnterReturnsTrue |
			ImGuiInputTextFlags_CallbackCompletion |
			ImGuiInputTextFlags_CallbackHistory,
			[](ImGuiInputTextCallbackData* data) -> int {
				ImGuiConsole* console = (ImGuiConsole*)data->UserData;

				switch (data->EventFlag) {
				case ImGuiInputTextFlags_CallbackCompletion: {
					std::vector<std::string> suggestions;
					console->AutoComplete(data->Buf, suggestions);
					if (!suggestions.empty()) {
						ImStrncpy(data->Buf, suggestions[0].c_str(), data->BufSize);
						data->Buf[data->BufSize - 1] = '\0'; // Sicherheit
						data->CursorPos = (int)strlen(data->Buf);
						data->SelectionStart = data->SelectionEnd = data->CursorPos;

						data->BufTextLen = (int)strlen(data->Buf);  // Wichtig!
						data->BufDirty = true;
					}
					break;
				}
				case ImGuiInputTextFlags_CallbackHistory: {
					if (data->EventKey == ImGuiKey_UpArrow) {
						if (console->HistoryPos == -1 && !console->History.empty())
							console->HistoryPos = (int)console->History.size() - 1;
						else if (console->HistoryPos > 0)
							console->HistoryPos--;
					}
					else if (data->EventKey == ImGuiKey_DownArrow) {
						if (console->HistoryPos != -1) {
							console->HistoryPos++;
							if (console->HistoryPos >= (int)console->History.size())
								console->HistoryPos = -1;
						}
					}

					if (console->HistoryPos >= 0 && console->HistoryPos < (int)console->History.size()) {
						const std::string& historyItem = console->History[console->HistoryPos];
						ImStrncpy(data->Buf, historyItem.c_str(), data->BufSize);
					}
					else {
						data->Buf[0] = '\0';
					}

					data->Buf[data->BufSize - 1] = '\0'; // Sicherheit
					data->CursorPos = (int)strlen(data->Buf);
					data->SelectionStart = data->SelectionEnd = data->CursorPos;

					data->BufTextLen = (int)strlen(data->Buf);  // Wichtig!
					data->BufDirty = true;
					break;
				}
				}

				return 0;
			}, (void*)this)) {

			std::string cmd(InputBuf);
			ExecCommand(cmd);

			if (History.empty() || History.back() != cmd)
				History.push_back(cmd);

			HistoryPos = -1; // Reset History position
			strcpy(InputBuf, "");
			reclaim_focus = true;
		}

		if (reclaim_focus)
			ImGui::SetKeyboardFocusHere(-1);

		if (UseCustomStyle)
			ImGui::PopStyleColor();

		ImGui::End();
	}

	void ImGuiConsole::RegisterCommand(const std::string& name, const std::string& description, CommandFunc func) {
		CommandMap[name] = { func, description };
	}

	bool ImGuiConsole::ExecuteCommand(const std::string& input, std::string& outError) {
		for (size_t i = input.size(); i > 0; --i) {
			std::string cmd = input.substr(0, i);
			auto it = CommandMap.find(cmd);
			if (it != CommandMap.end()) {
				std::string args = (i < input.size()) ? input.substr(i + 1) : "";
				it->second.function(args);
				return true;
			}
		}

		outError = "Unbekannter Befehl: " + input;
		return false;
	}




}