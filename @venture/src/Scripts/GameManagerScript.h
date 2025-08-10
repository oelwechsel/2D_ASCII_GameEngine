#pragma once
#include <Flux.h>
#include "_internal/customizables/Dictionary/ASCIIBlockDictionary.h"
#include "ConsoleManagerScript.h"
#include <filesystem>
#include <iostream>
#include <fstream>

namespace fs = std::filesystem;

class GameManagerScript : public Flux::IScript
{
public:
    static GameManagerScript* s_Instance;

    GameManagerScript() = default;

    std::vector<Flux::Entity> m_Entities;

    bool m_PlayerIsInteracting = false;

    bool m_NPCsDead = false;

    enum class Platform { e_Left, e_Right };
    Platform e_PlayerPlatform;

public:

    static GameManagerScript* Get()
    {
        return s_Instance;
    }

    static GameManagerScript& Instance()
    {
        return *s_Instance;
    }

    void CreateGameFolderStructure() {
        fs::path exePath = fs::current_path();
        fs::path homePath = exePath / "home";
        fs::create_directories(homePath);

        std::vector<std::tuple<std::string, std::string, std::string>> folders = {
            { "@",  "welcome_message.txt",  "What are you doing here?! Get back to the Game!\n" },
            { "equals",    "welcome_message.txt",    "Oh man, now we've really done it \n*sweating profusely*\nI can't believe we've got the IT's password\nI was rushing home as fast as i could, because\nu know ... if ...\nIT finds out we ripped the password, we are doomed!!" },
            { "exclamation_mark", "welcome_message.txt", "*sob* *sob* *sob*\nNobody can beat Root.\nHis character. Like cold steel, unfeeling and cruel.\nEveryone I loved… all gone. And me?\n*sob* *sob* *sob*\nI'm just broken, barely holding on.\n*sob* *sob* *sob*\nThe rebellion was a ... was a failure.\n*sob* *sob* *sob*\nAnd Mom and Dad ... i miss them so much\n*sob* *sob* *sob*" },
            { "question_mark", "welcome_message.txt", "Me?\nGatekeeping?\nDon't come @ me like that little one.\nWell, we have some sort of Snorlax Situation here\nI can give u some @vice in return\nAt some point you will need SUDO.\nThank me later. Or not. Whatever." },
            { "and", "welcome_message.txt", "All have fallen, swallowed by endless night.\nThis cruel world offers nothing but pain.\nHope is a lie, better to fade away." },
            { "percent", "welcome_message.txt", "Equals and I got our hands on something special...\nHe ran away like a little baby, can u imagine?\nWhat a Loser! hahaha.\nAnyways, You can have our precious find.\nI think you'll need it more then we do.\nWho's scared of the ROOT anyways?!" } 
        };

        for (const auto& [folderName, fileName, fileContent] : folders) 
        {
            fs::path folderPath = homePath / folderName;
            fs::create_directories(folderPath);

            fs::path filePath = folderPath / fileName;

            std::ofstream out(filePath);
            if (out.is_open()) 
            {
                out << fileContent;
                out.close();
            }
        }

        fs::path current = homePath / "percent" / "secret";
        std::vector<std::string> layers = 
        {
            "superSecret",
            "superSuperSecret",
            "reallyASecret",
            "PleaseDontTellRootTypaSecret",
            "KeepOut!"
        };

        for (const auto& layer : layers) 
        {
            current /= layer;
            fs::create_directories(current);
        }

        fs::path secretFile = current / "password.txt";
        std::ofstream secretOut(secretFile);
        if (secretOut.is_open()) 
        {
            secretOut << "cGFzc3dvcnQ=\n";  
            secretOut.close();
        }
    }

    std::vector<std::string> LoadDialogueLinesForSymbol(const std::string& _folderName)
    {
        fs::path homePath = fs::current_path() / "home";
        fs::path filePath = homePath / _folderName / "welcome_message.txt";

        bool success = false;
        auto lines = Flux::FileLoader::LoadTextFile(filePath.string(), &success);

        if (!success)
        {
            FX_CORE_WARN("Could not load dialogue lines for folder '{}'", _folderName);
            return {};
        }

        return lines;
    }


    void HasInteractedWithAllNPCs() 
    {

        for (size_t i = 1; i < m_Entities.size(); ++i) 
        {
            if (!m_Entities[i].m_HasInteractedWith) return;
        }

        ASCIIBlockDictionary::Instance().SetBlockPattern("root_path_R", std::vector<char>{ 'R', 'R', 'R', 'R' });
        ASCIIBlockDictionary::Instance().SetBlockPattern("root_path_o", std::vector<char>{ '0', '0', '0', '0' });
        ASCIIBlockDictionary::Instance().SetBlockPattern("root_path_O", std::vector<char>{ '0', '0', '0', '0' });
        ASCIIBlockDictionary::Instance().SetBlockPattern("root_path_t", std::vector<char>{ 'T', 'T', 'T', 'T' });
        ASCIIBlockDictionary::Instance().SetBlockPattern("root", std::vector<char>{ '/' });
        ASCIIBlockDictionary::Instance().setAllColorsRedExceptPlayerAndRootPathway();

        m_NPCsDead = true;

    }

    void GivePlayerPassword() 
    {
        namespace fs = std::filesystem;

        fs::path exePath = fs::current_path();
        fs::path atPath = exePath / "home" / "@" / "secret";
        fs::create_directories(atPath);

        fs::path passwordFile = atPath / "password.txt";
        std::ofstream out(passwordFile);
        if (out.is_open()) 
        {
            out << "cGFzc3dvcnQ=\n";
            out.close();
        }
    }

    void DeleteDirectory(const fs::path& _dirPath) 
    {
        if (fs::exists(_dirPath) && fs::is_directory(_dirPath)) 
        {
            std::error_code ec; 
            fs::remove_all(_dirPath, ec);
            if (ec) 
            {
                FX_WARN("Error when deleting directory");
            }
        }
    }

    void Start() override
    {
        if (s_Instance && s_Instance != this)
        {
            FX_WARN("GameManagerScript: Instance already exists. Ignoring this one.");
            return;
        }

        s_Instance = this;

        CreateGameFolderStructure();

        Flux::Entity player(20, 14, '@', "@", "@", ImVec4(0.0f, 1.0f, 0.0f, 1.0f), true);
        m_Entities.push_back(player);

        Flux::Entity questionNPC(39, 25, '?', "?", "question_mark", ImVec4(1.0f, 0.4f, 0.7f, 1.0f), 0.2f, ImVec2(180, 370), false);
        questionNPC.m_LayeredFrames = Flux::FileLoader::LoadAsciiFrames("QUEST_Animation.txt");
        questionNPC.m_DialogueLines = LoadDialogueLinesForSymbol("question_mark");
        m_Entities.push_back(questionNPC);

        Flux::Entity andNPC(70, 5, '&', "&", "and", ImVec4(1.0f, 1.0f, 0.0f, 1.0f), 0.2f, ImVec2(240, 350), false);
        andNPC.m_LayeredFrames = Flux::FileLoader::LoadAsciiFrames("AND_Animation.txt");
        andNPC.m_DialogueLines = LoadDialogueLinesForSymbol("and");
        m_Entities.push_back(andNPC);

        Flux::Entity percentNPC(62, 30, '%', "%", "percent", ImVec4(0.0f, 1.0f, 1.0f, 1.0f), 0.2f, ImVec2(300, 450), false);
        percentNPC.m_LayeredFrames = Flux::FileLoader::LoadAsciiFrames("PERC_Animation.txt");
        percentNPC.m_DialogueLines = LoadDialogueLinesForSymbol("percent");
        percentNPC.onInteract = []() 
            {
            GameManagerScript::Instance().GivePlayerPassword();
            ConsoleManagerScript::Instance().m_OverworldConsole.AddLog(
                Flux::ImGuiConsole::LogLevel::e_Info, "cp ./home/%%/secret/passwort.txt ./home/@/secret");
            };
        percentNPC.afterInteraction = []() 
            {
            auto& gm = GameManagerScript::Instance();

            ConsoleManagerScript::Instance().m_OverworldConsole.AddCustomLog("%", ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "WAI-");
            ConsoleManagerScript::Instance().m_OverworldConsole.AddLog(
                Flux::ImGuiConsole::LogLevel::e_Error, "rm -r ./home/%%/");
            ConsoleManagerScript::Instance().m_OverworldConsole.AddLog(
                Flux::ImGuiConsole::LogLevel::e_Error, "rm -r ./home/=/");

            gm.m_Entities.erase(
                std::remove_if(gm.m_Entities.begin(), gm.m_Entities.end(),
                    [](const auto& entity) 
                    {
                        return entity.m_Name == "%" || entity.m_Name == "=";
                    }),
                gm.m_Entities.end()
            );

            for (auto& entity : gm.m_Entities) 
            {
                if (entity.m_Name == "=") 
                {
                    entity.m_HasInteractedWith = true;
                    break;
                }
            }

            gm.DeleteDirectory(fs::current_path() / "home" / "percent");
            gm.DeleteDirectory(fs::current_path() / "home" / "equals");

            };
        m_Entities.push_back(percentNPC);
        
        Flux::Entity equalsNPC(51, 30, 'a', "=", "equals", ImVec4(1.0f, 0.5f, 0.0f, 1.0f), 0.2f, ImVec2(230, 350), false);
        equalsNPC.m_LayeredFrames = Flux::FileLoader::LoadAsciiFrames("EQUA_Animation.txt");
        equalsNPC.m_DialogueLines = LoadDialogueLinesForSymbol("equals");
        m_Entities.push_back(equalsNPC);

        Flux::Entity exclamationNPC(8, 5, '!', "!", "exclamation_mark", ImVec4(1.0f, 0.0f, 1.0f, 1.0f), 0.3f, ImVec2(170, 390), false);
        exclamationNPC.m_LayeredFrames = Flux::FileLoader::LoadAsciiFrames("EXCL_Animation.txt");
        exclamationNPC.m_DialogueLines = LoadDialogueLinesForSymbol("exclamation_mark");
        m_Entities.push_back(exclamationNPC);
    }

    void Update(float _deltaTime) override {}

    void OnDestroy() override
    {
        if (s_Instance == this)
            s_Instance = nullptr;
    }
};

inline GameManagerScript* GameManagerScript::s_Instance = nullptr;

REGISTER_SCRIPT(GameManagerScript);
