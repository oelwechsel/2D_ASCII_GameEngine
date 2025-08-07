#include "fxpch.h"
#include "FightConsole.h"
#include "Scripts/PlayerControllerScript.h"
#include "Scripts/EnemyControllerScript.h"


FightConsole::FightConsole()
{
    UseCustomStyle = true;
    DefaultTextColor = ImVec4(1.0f, 0.4f, 0.4f, 0.8f);

    RegisterCommand("help", "Shows all available commands", [this](const std::string&) {
        AddLog("=== Available commands ===");
        for (const auto& [cmd, entry] : CommandMap)
            AddLog("  %-12s - %s", cmd.c_str(), entry.description.c_str());
        AddLog("==========================");
        });

    RegisterCommand("clear", "Clear console", [this](const std::string&) {
        ClearLog();
        });

    RegisterCommand("attack", "Attack the ROOT", [this](const std::string&) {

        EnemyControllerScript::Instance().m_BossEnemyHP--;
        AddLog("You attack the ROOT!");
        AddLog("ROOT HP: %d", EnemyControllerScript::Instance().m_BossEnemyHP);

        if (EnemyControllerScript::Instance().m_BossEnemyHP <= 0) {
            if (EnemyControllerScript::Instance().m_BossEnemyPhase == 1) {
                EnemyControllerScript::Instance().m_BossEnemyPhase = 2;
                EnemyControllerScript::Instance().m_BossEnemyHP = 4;
                EnemyControllerScript::Instance().e_BossEnemyFightState = EnemyControllerScript::EnemyState::Neutral;
                AddLog("HP restored to %d.", EnemyControllerScript::Instance().m_BossEnemyHP);
                AddLog("Seems like a rough enemy... Maybe you should seek some help");

                //EnemyCharge();  // Start Phase 2 mit laden
            }
            else if (EnemyControllerScript::Instance().m_BossEnemyPhase == 2) {
                EnemyControllerScript::Instance().m_BossEnemyPhase = 4;
                EnemyControllerScript::Instance().e_BossEnemyFightState = EnemyControllerScript::EnemyState::Neutral;
                AddLog("HP restored to %d.", EnemyControllerScript::Instance().m_BossEnemyHP);
                AddLog("Seems like a rough enemy... Maybe you should seek some help");

                //EnemyCharge();  // Gegner lädt wieder auf
            }
            //justReset = true;  // Markiere, dass gerade Reset war
        }
        });

    RegisterCommand("cd left", "Move to the LEFT platform", [this](const std::string&)
        {
            if (PlayerControllerScript::Instance().e_PlayerPlatform == PlayerControllerScript::Platform::Left)
            {
                AddLog("You keep standing on the LEFT platform.");
            }
            else 
            {
                PlayerControllerScript::Instance().e_PlayerPlatform = PlayerControllerScript::Platform::Left;
                AddLog("You moved to LEFT platform.");
            }
        });

    RegisterCommand("cd right", "Move to the RIGHT platform", [this](const std::string&)
        {
            if (PlayerControllerScript::Instance().e_PlayerPlatform == PlayerControllerScript::Platform::Right)
            {
                AddLog("You keep standing on the RIGHT platform.");
            }
            else
            {
                PlayerControllerScript::Instance().e_PlayerPlatform = PlayerControllerScript::Platform::Right;
                AddLog("You moved to RIGHT platform.");
            }
        });
}

FightConsole::~FightConsole() {}


void FightConsole::ExecCommand(const std::string& command)
{
    AddLog(LogLevel::Info, "$ %s", command.c_str());

    std::string error;
    if (!ExecuteCommand(command, error))
        AddLog(LogLevel::Warning, "%s", error.c_str());
}

void FightConsole::AutoComplete(const std::string& currentInput, std::vector<std::string>& suggestions)
{
    for (const auto& [cmd, _] : CommandMap)
    {
        if (cmd.find(currentInput) == 0)
            suggestions.push_back(cmd + " ");
    }
}


