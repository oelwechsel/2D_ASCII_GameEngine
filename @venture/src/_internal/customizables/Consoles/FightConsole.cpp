#include "fxpch.h"
#include "FightConsole.h"
#include "Scripts/EnemyControllerScript.h"
#include "Scripts/GameManagerScript.h"


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
        EnemyControllerScript::Instance().e_BossEnemyFightState = EnemyControllerScript::EnemyState::TargetGetHit;

        if (EnemyControllerScript::Instance().m_BossEnemyHP <= 0) {
            if (EnemyControllerScript::Instance().m_BossEnemyPhase == 1) {
                EnemyControllerScript::Instance().m_BossEnemyPhase = 2;
                EnemyControllerScript::Instance().m_BossEnemyHP = 4;
                AddLog("HP restored to %d.", EnemyControllerScript::Instance().m_BossEnemyHP);
                AddLog("Seems like a rough enemy... Maybe you should seek some help");

            }
            else if (EnemyControllerScript::Instance().m_BossEnemyPhase == 2) {
                EnemyControllerScript::Instance().m_BossEnemyPhase = 4;
                AddLog("HP restored to %d.", EnemyControllerScript::Instance().m_BossEnemyHP);
                AddLog("Seems like a rough enemy... Maybe you should seek some help");

            }
        }
        });

    RegisterCommand("cd left", "Move to the LEFT platform", [this](const std::string&)
        {
            if (GameManagerScript::Instance().e_PlayerPlatform == GameManagerScript::Platform::Left)
            {
                AddLog("You keep standing on the LEFT platform.");
            }
            else 
            {
                GameManagerScript::Instance().e_PlayerPlatform = GameManagerScript::Platform::Left;
                AddLog("You moved to LEFT platform.");
            }

            if (EnemyControllerScript::Instance().e_BossEnemyFightState == EnemyControllerScript::EnemyState::WaitingForPlayer)
            {
                EnemyControllerScript::Instance().e_BossEnemyFightState = EnemyControllerScript::EnemyState::Attacking;
            }
        });

    RegisterCommand("cd right", "Move to the RIGHT platform", [this](const std::string&)
        {
            if (GameManagerScript::Instance().e_PlayerPlatform == GameManagerScript::Platform::Right)
            {
                AddLog("You keep standing on the RIGHT platform.");
            }
            else
            {
                GameManagerScript::Instance().e_PlayerPlatform = GameManagerScript::Platform::Right;
                AddLog("You moved to RIGHT platform.");
            }

            if (EnemyControllerScript::Instance().e_BossEnemyFightState == EnemyControllerScript::EnemyState::WaitingForPlayer)
            {
                EnemyControllerScript::Instance().e_BossEnemyFightState = EnemyControllerScript::EnemyState::Attacking;
            }
        });
}

FightConsole::~FightConsole() {}


void FightConsole::ExecCommand(const std::string& command)
{
    if (EnemyControllerScript::Instance().e_BossEnemyFightState != EnemyControllerScript::EnemyState::WaitingForPlayer)
    {
        AddLog("You cannot act right now!");
        return;
    }

    AddLog(LogLevel::Info, "$ %s", command.c_str());

    std::string error;
    if (!ExecuteCommand(command, error))
    {
        AddLog(LogLevel::Warning, "%s", error.c_str());
        return;
    }

}


void FightConsole::AutoComplete(const std::string& currentInput, std::vector<std::string>& suggestions)
{
    for (const auto& [cmd, _] : CommandMap)
    {
        if (cmd.find(currentInput) == 0)
            suggestions.push_back(cmd + " ");
    }
}


