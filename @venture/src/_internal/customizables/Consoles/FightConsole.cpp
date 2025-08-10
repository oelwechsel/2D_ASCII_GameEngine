#include "fxpch.h"
#include "FightConsole.h"
#include "Scripts/EnemyControllerScript.h"
#include "Scripts/GameManagerScript.h"


FightConsole::FightConsole()
{
    m_UseCustomStyle = true;
    m_DefaultTextColor = ImVec4(1.0f, 0.4f, 0.4f, 0.8f);

    m_WaitingForPassword = false;

    RegisterCommand("help", "Shows all available commands", [this](const std::string&) {
        AddLog("=== Available commands ===");
        for (const auto& [cmd, entry] : m_CommandMap)
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
        EnemyControllerScript::Instance().e_BossEnemyFightState = EnemyControllerScript::EnemyState::e_TargetGetHit;

        if (EnemyControllerScript::Instance().m_BossEnemyHP <= 0) {
            if (EnemyControllerScript::Instance().m_BossEnemyPhase == 1) {
                EnemyControllerScript::Instance().m_BossEnemyPhase = 2;
                EnemyControllerScript::Instance().m_BossEnemyHP = 4;
                AddLog("HP restored to %d.", EnemyControllerScript::Instance().m_BossEnemyHP);
                AddLog("Seems like a rough enemy... Maybe you should seek some help");

                // register sudo remove when in phase 2
                RegisterCommand("sudo remove", "Try to remove the ROOT (password required)", [this](const std::string&) {
                    if (m_WaitingForPassword) {
                        AddLog("Password input already in progress.");
                        return;
                    }
                    m_WaitingForPassword = true;
                    m_PasswordInput.clear();
                    AddLog("Enter password:");
                    });

            }
            else if (EnemyControllerScript::Instance().m_BossEnemyPhase == 2) {
                EnemyControllerScript::Instance().m_BossEnemyPhase = 4;
                EnemyControllerScript::Instance().m_BossEnemyHP = 4;
                AddLog("HP restored to %d.", EnemyControllerScript::Instance().m_BossEnemyHP);
                AddLog("Seems like a rough enemy... Maybe you should seek some help");
            }
        }
        });

    RegisterCommand("cd left", "Move to the LEFT platform", [this](const std::string&)
        {
            if (GameManagerScript::Instance().e_PlayerPlatform == GameManagerScript::Platform::e_Left)
                AddLog("You keep standing on the LEFT platform.");
            else
            {
                GameManagerScript::Instance().e_PlayerPlatform = GameManagerScript::Platform::e_Left;
                AddLog("You moved to LEFT platform.");
            }

            if (EnemyControllerScript::Instance().e_BossEnemyFightState == EnemyControllerScript::EnemyState::e_WaitingForPlayer)
                EnemyControllerScript::Instance().e_BossEnemyFightState = EnemyControllerScript::EnemyState::e_Attacking;
        });

    RegisterCommand("cd right", "Move to the RIGHT platform", [this](const std::string&)
        {
            if (GameManagerScript::Instance().e_PlayerPlatform == GameManagerScript::Platform::e_Right)
                AddLog("You keep standing on the RIGHT platform.");
            else
            {
                GameManagerScript::Instance().e_PlayerPlatform = GameManagerScript::Platform::e_Right;
                AddLog("You moved to RIGHT platform.");
            }

            if (EnemyControllerScript::Instance().e_BossEnemyFightState == EnemyControllerScript::EnemyState::e_WaitingForPlayer)
                EnemyControllerScript::Instance().e_BossEnemyFightState = EnemyControllerScript::EnemyState::e_Attacking;
        });

    m_WaitingForPassword = false;
    m_PasswordInput = "";
}

FightConsole::~FightConsole() {}


void FightConsole::ExecCommand(const std::string& _command)
{
    if (m_WaitingForPassword)
    {
        if (_command == m_Password)
        {
            AddLog("Password correct!");
            AddLog("...");
            AddLog("...");
            AddLog("User @ disconnected");
            AddLog("Now executing as ROOT");
            EnemyControllerScript::Instance().m_BossEnemyHP = 0;
            EnemyControllerScript::Instance().e_BossEnemyFightState = EnemyControllerScript::EnemyState::e_TargetGetHit;
            EnemyControllerScript::Instance().e_BossEnemyFightState = EnemyControllerScript::EnemyState::e_CutsceneEnd;
        }
        else
        {
            AddLog("Wrong password! Nothing happens.");
        }

        m_WaitingForPassword = false;
        m_PasswordInput.clear();
        return;
    }

    if (EnemyControllerScript::Instance().e_BossEnemyFightState != EnemyControllerScript::EnemyState::e_WaitingForPlayer)
    {
        AddLog("You cannot act right now!");
        return;
    }

    AddLog(LogLevel::e_Info, "$ %s", _command.c_str());

    std::string error;
    if (!ExecuteCommand(_command, error))
    {
        AddLog(LogLevel::e_Warning, "%s", error.c_str());
        return;
    }
}



void FightConsole::AutoComplete(const std::string& _currentInput, std::vector<std::string>& _suggestions)
{
    for (const auto& [cmd, _] : m_CommandMap)
    {
        if (cmd.find(_currentInput) == 0)
            _suggestions.push_back(cmd + " ");
    }
}


