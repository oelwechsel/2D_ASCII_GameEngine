#include "fxpch.h"
#include "ImGuiConsole.h"
#include <imgui_internal.h>
#include "../Log.h"

namespace Flux
{
	ImGuiConsole::ImGuiConsole() 
	{
		ClearLog();
		memset(InputBuf, 0, sizeof(InputBuf));
		m_HistoryPos = -1;
	}

	ImGuiConsole::~ImGuiConsole() 
	{
		ClearLog();
	}

	void ImGuiConsole::ClearLog() 
	{
		for (auto& item : m_Items)
			free(item);
		m_Items.clear();
		m_CustomColorMap.clear();
	}


	void ImGuiConsole::AddItem(const std::string& _msg) 
	{
		m_Items.push_back(strdup(_msg.c_str()));
		m_ScrollToBottom = true;
	}

	void ImGuiConsole::AddLog(const char* fmt, ...) 
	{
		va_list args;
		va_start(args, fmt);
		char buf[1024];
		vsnprintf(buf, sizeof(buf), fmt, args);
		buf[sizeof(buf) - 1] = 0;
		va_end(args);
		AddItem(buf);
	}

	void ImGuiConsole::AddLog(LogLevel _level, const char* fmt, ...) 
	{
		va_list args;
		va_start(args, fmt);
		char buf[1024];
		vsnprintf(buf, sizeof(buf), fmt, args);
		buf[sizeof(buf) - 1] = 0;
		va_end(args);

		std::string prefix;
		switch (_level) 
		{
		case LogLevel::e_Info:    prefix = "[INFO] "; break;
		case LogLevel::e_Warning: prefix = "[WARN] "; break;
		case LogLevel::e_Error:   prefix = "[ERROR] "; break;
		}

		AddItem(prefix + buf);
	}

	void ImGuiConsole::AddCustomLog(const std::string& _customLevel, const ImVec4& _color, const char* fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		char buf[1024];
		vsnprintf(buf, sizeof(buf), fmt, args);
		buf[sizeof(buf) - 1] = 0;
		va_end(args);

		std::string fullText = "[" + _customLevel + "] " + buf;

		char* storedText = strdup(fullText.c_str());
		m_Items.push_back(storedText);
		m_CustomColorMap[storedText] = _color;
		m_ScrollToBottom = true;
	}

	void ImGuiConsole::WelcomeMessage(const char* _message) 
	{
		if (!m_Initialized) {
			AddLog(_message);
			m_Initialized = true;
		}
	}

	void ImGuiConsole::Draw(const char* _title, const char* _welcomeMessage,
		const ImVec2& _windowPos, const ImVec2& _windowSize, bool* _out_hasFocus, bool* _p_open)
	{
		WelcomeMessage(_welcomeMessage);

		if (_windowPos.x >= 0 && _windowPos.y >= 0)
			ImGui::SetNextWindowPos(_windowPos, ImGuiCond_FirstUseEver);
		if (_windowSize.x >= 0 && _windowSize.y >= 0)
			ImGui::SetNextWindowSize(_windowSize, ImGuiCond_FirstUseEver);

		if (!ImGui::Begin(_title, _p_open, ImGuiWindowFlags_NoScrollbar)) 
		{
			ImGui::End();
			return;
		}

		if (_out_hasFocus)
			*_out_hasFocus = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);

		ImGui::BeginChild("ScrollingRegion", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), false,
			ImGuiWindowFlags_HorizontalScrollbar);

		for (const auto& item : m_Items) 
		{
			auto it = m_CustomColorMap.find(item);
			if (it != m_CustomColorMap.end()) 
			{
				ImGui::PushStyleColor(ImGuiCol_Text, it->second);
				ImGui::TextUnformatted(item);
				ImGui::PopStyleColor();
			}
			else 
			{
				ImVec4 color;
				bool has_color = false;
				if (strstr(item, "[ERROR]")) 
				{
					color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f);
					has_color = true;
				}
				else if (strstr(item, "[WARN]")) 
				{
					color = ImVec4(1.0f, 1.0f, 0.4f, 1.0f);
					has_color = true;
				}
				else if (strstr(item, "[INFO]")) 
				{
					color = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
					has_color = true;
				}

				if (has_color) 
				{
					ImGui::PushStyleColor(ImGuiCol_Text, color);
					ImGui::TextUnformatted(item);
					ImGui::PopStyleColor();
				}
				else 
				{
					ImGui::PushStyleColor(ImGuiCol_Text, m_DefaultTextColor);
					ImGui::TextUnformatted(item);
					ImGui::PopStyleColor();
				}
			}
		}

		if (m_ScrollToBottom)
			ImGui::SetScrollHereY(1.0f);
		m_ScrollToBottom = false;

		ImGui::EndChild();
		ImGui::Separator();

		if (m_UseCustomStyle)
			ImGui::PushStyleColor(ImGuiCol_Text, m_DefaultTextColor);

		bool reclaim_focus = false;
		if (m_InputEnabled && ImGui::InputText("Input", InputBuf, sizeof(InputBuf),
			ImGuiInputTextFlags_EnterReturnsTrue |
			ImGuiInputTextFlags_CallbackCompletion |
			ImGuiInputTextFlags_CallbackHistory,
			[](ImGuiInputTextCallbackData* data) -> int {
				ImGuiConsole* console = (ImGuiConsole*)data->UserData;

				switch (data->EventFlag) 
				{
				case ImGuiInputTextFlags_CallbackCompletion: 
				{
					std::vector<std::string> suggestions;
					console->AutoComplete(data->Buf, suggestions);
					if (!suggestions.empty()) 
					{
						ImStrncpy(data->Buf, suggestions[0].c_str(), data->BufSize);
						data->Buf[data->BufSize - 1] = '\0'; 
						data->CursorPos = (int)strlen(data->Buf);
						data->SelectionStart = data->SelectionEnd = data->CursorPos;

						data->BufTextLen = (int)strlen(data->Buf);  
						data->BufDirty = true;
					}
					break;
				}
				case ImGuiInputTextFlags_CallbackHistory: 
				{
					if (data->EventKey == ImGuiKey_UpArrow) 
					{
						if (console->m_HistoryPos == -1 && !console->m_History.empty())
							console->m_HistoryPos = (int)console->m_History.size() - 1;
						else if (console->m_HistoryPos > 0)
							console->m_HistoryPos--;
					}
					else if (data->EventKey == ImGuiKey_DownArrow) 
					{
						if (console->m_HistoryPos != -1) 
						{
							console->m_HistoryPos++;
							if (console->m_HistoryPos >= (int)console->m_History.size())
								console->m_HistoryPos = -1;
						}
					}

					if (console->m_HistoryPos >= 0 && console->m_HistoryPos < (int)console->m_History.size()) 
					{
						const std::string& historyItem = console->m_History[console->m_HistoryPos];
						ImStrncpy(data->Buf, historyItem.c_str(), data->BufSize);
					}
					else 
					{
						data->Buf[0] = '\0';
					}

					data->Buf[data->BufSize - 1] = '\0'; 
					data->CursorPos = (int)strlen(data->Buf);
					data->SelectionStart = data->SelectionEnd = data->CursorPos;

					data->BufTextLen = (int)strlen(data->Buf);  
					data->BufDirty = true;
					break;
				}
				}

				return 0;
			}, (void*)this)) 
		{

			std::string cmd(InputBuf);
			ExecCommand(cmd);

			if (m_History.empty() || m_History.back() != cmd)
				m_History.push_back(cmd);

			m_HistoryPos = -1;
			strcpy(InputBuf, "");
			reclaim_focus = true;
		}

		if (reclaim_focus)
			ImGui::SetKeyboardFocusHere(-1);

		if (m_UseCustomStyle)
			ImGui::PopStyleColor();

		ImGui::End();
	}

	void ImGuiConsole::RegisterCommand(const std::string& _name, const std::string& _description, CommandFunc _func) 
	{
		m_CommandMap[_name] = { _func, _description };
	}

	bool ImGuiConsole::ExecuteCommand(const std::string& _input, std::string& _outError) 
	{
		for (size_t i = _input.size(); i > 0; --i) {
			std::string cmd = _input.substr(0, i);
			auto it = m_CommandMap.find(cmd);
			if (it != m_CommandMap.end()) 
			{
				std::string args = (i < _input.size()) ? _input.substr(i + 1) : "";
				it->second.function(args);
				return true;
			}
		}

		_outError = "unknown command: " + _input + "\n" + "Enter the 'help' command for all available commands.";
		return false;
	}
}