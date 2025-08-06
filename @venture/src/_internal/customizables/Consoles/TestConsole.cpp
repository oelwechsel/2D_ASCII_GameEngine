#include "fxpch.h"
#include "TestConsole.h"


TestConsole::TestConsole()
{
	UseCustomStyle = true;
	BackgroundColor = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);
	DefaultTextColor = ImVec4(1.0f, 0.0, 0.0f, 1.0f);
}

TestConsole::~TestConsole()
{
	// Hier könnte man Ressourcen freigeben, falls nötig
}

void TestConsole::ExecCommand(const std::string& command)
{
	AddLog(LogLevel::Info, "# %s", command.c_str());

	if (command == "clear")
	{
		ClearLog();
	}
	else if (command == "help")
	{
		AddLog("Verfügbare Befehle:");
		AddLog("  help  - Zeigt diese Hilfe an");
		AddLog("  clear - Leert die Konsole");
		AddLog("  echo <text> - Gibt Text zurück");
	}
	else if (command.rfind("echo ", 0) == 0)
	{
		std::string text = command.substr(5);
		AddLog("%s", text.c_str());
	}
	else
	{
		AddLog(LogLevel::Warning, "Unbekannter Befehl: '%s'", command.c_str());
	}
}

void TestConsole::AutoComplete(const std::string& currentInput, std::vector<std::string>& suggestions)
{
	static const char* commands[] = { "help", "clear", "echo " };
	for (const char* cmd : commands)
	{
		if (currentInput.empty() || strncmp(cmd, currentInput.c_str(), currentInput.length()) == 0)
			suggestions.emplace_back(cmd);
	}
}

