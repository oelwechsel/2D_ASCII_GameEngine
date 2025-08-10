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


void TestConsole::ExecCommand(const std::string& command)
{
    AddLog(LogLevel::e_Info, "$ %s", command.c_str());

    std::string error;
    if (!ExecuteCommand(command, error))
        AddLog(LogLevel::e_Warning, "%s", error.c_str());
}

void TestConsole::AutoComplete(const std::string& currentInput, std::vector<std::string>& suggestions)
{
    for (const auto& [cmd, _] : m_CommandMap)
    {
        if (cmd.find(currentInput) == 0)
            suggestions.push_back(cmd + " ");
    }
}

