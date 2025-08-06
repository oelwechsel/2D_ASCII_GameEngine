#include "fxpch.h"
#include "TestConsole.h"

TestConsole::TestConsole()
{
    UseCustomStyle = true;
    DefaultTextColor = ImVec4(0.4f, 1.0, 0.4f, 0.5f);

    RegisterCommand("help", [this](const std::string&) {
        AddLog("Verfügbare Befehle:");
        for (const auto& cmd : CommandMap)
            AddLog("  %s", cmd.first.c_str());
        });


    RegisterCommand("echo", [this](const std::string& args) {
        AddLog("%s", args.c_str());
        });


    RegisterCommand("clear", [this](const std::string&) {
        ClearLog();
        });
}

TestConsole::~TestConsole() {}


void TestConsole::ExecCommand(const std::string& command)
{
    AddLog(LogLevel::Info, "# %s", command.c_str());

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

