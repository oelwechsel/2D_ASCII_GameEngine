#include "fxpch.h"
#include "TestConsole.h"

TestConsole::TestConsole()
{
    UseCustomStyle = true;
    DefaultTextColor = ImVec4(0.4f, 1.0, 0.4f, 0.8f);

    RegisterCommand("help", "Zeigt alle Befehle an", [this](const std::string&) {
        AddLog("=== Verfügbare Befehle ===");
        for (const auto& [cmd, entry] : CommandMap)
            AddLog("  %-12s - %s", cmd.c_str(), entry.description.c_str());
        AddLog("==========================");
        });

    RegisterCommand("clear", "Konsole leeren", [this](const std::string&) {
        ClearLog();
        });
}

TestConsole::~TestConsole() {}


void TestConsole::ExecCommand(const std::string& command)
{
    AddLog(LogLevel::Info, "$ %s", command.c_str());

    std::string error;
    if (!ExecuteCommand(command, error))
        AddLog(LogLevel::Warning, "%s", error.c_str());
}

void TestConsole::AutoComplete(const std::string& currentInput, std::vector<std::string>& suggestions)
{
    for (const auto& [cmd, _] : CommandMap)
    {
        if (cmd.find(currentInput) == 0)
            suggestions.push_back(cmd + " ");
    }
}

