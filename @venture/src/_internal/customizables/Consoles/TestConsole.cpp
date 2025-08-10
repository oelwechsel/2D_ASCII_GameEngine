#include "fxpch.h"
#include "TestConsole.h"

TestConsole::TestConsole()
{
    m_UseCustomStyle = true;
    m_DefaultTextColor = ImVec4(0.4f, 1.0, 0.4f, 0.8f);

    RegisterCommand("help", "Shows all available commands", [this](const std::string&) {
        AddLog("=== Available commands ===");
        for (const auto& [cmd, entry] : m_CommandMap)
            AddLog("  %-12s - %s", cmd.c_str(), entry.description.c_str());
        AddLog("==========================");
        });

    RegisterCommand("clear", "Clear console", [this](const std::string&) {
        ClearLog();
        });
}

TestConsole::~TestConsole() {}


void TestConsole::ExecCommand(const std::string& _command)
{
    AddLog(LogLevel::e_Info, "$ %s", _command.c_str());

    std::string error;
    if (!ExecuteCommand(_command, error))
        AddLog(LogLevel::e_Warning, "%s", error.c_str());
}

void TestConsole::AutoComplete(const std::string& _currentInput, std::vector<std::string>& _suggestions)
{
    for (const auto& [cmd, _] : m_CommandMap)
    {
        if (cmd.find(_currentInput) == 0)
            _suggestions.push_back(cmd + " ");
    }
}

