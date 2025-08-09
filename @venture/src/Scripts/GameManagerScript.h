#pragma once
#include <Flux.h>

#include <filesystem>
#include <iostream>
#include <fstream>

namespace fs = std::filesystem;

class GameManagerScript : public Flux::IScript
{
public:
    static GameManagerScript* s_Instance;

    GameManagerScript() = default;

    std::vector<Flux::Entity> entities;

    bool m_playerIsInteracting = false;

    bool m_isInFight = false;


    enum class Platform { Left, Right };
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
            { "equals",    "welcome_message.txt",    "Oh man, now we've really done it \n*sweating profusely*\n" },
            { "exclamation_mark", "welcome_message.txt", "*sob* *sob* *sob*\n" },
            { "question_mark", "welcome_message.txt", "Wanna learn something good, kid ? \nAt some point you will need SUDO\n" },
            { "and", "welcome_message.txt", "Everyone is gone... I will be gone too\n" },
            { "percent", "welcome_message.txt", "Pssssssssssst look at what I got\n" } 
        };

        for (const auto& [folderName, fileName, fileContent] : folders) {
            fs::path folderPath = homePath / folderName;
            fs::create_directories(folderPath);

            fs::path filePath = folderPath / fileName;

            std::ofstream out(filePath);
            if (out.is_open()) {
                out << fileContent;
                out.close();
            }
        }

        fs::path current = homePath / "percent" / "secret";
        std::vector<std::string> layers = {
            "superSecret",
            "superSuperSecret",
            "reallyASecret",
            "PleaseDontTellRootTypaSecret",
            "KeepOut!"
        };

        for (const auto& layer : layers) {
            current /= layer;
            fs::create_directories(current);
        }

        fs::path secretFile = current / "password.txt";
        std::ofstream secretOut(secretFile);
        if (secretOut.is_open()) {
            secretOut << "cGFzc3dvcnQ=\n";  
            secretOut.close();
        }
    }

    std::vector<std::string> LoadDialogueLinesForSymbol(const std::string& folderName)
    {
        fs::path homePath = fs::current_path() / "home";
        fs::path filePath = homePath / folderName / "welcome_message.txt";

        bool success = false;
        auto lines = Flux::FileLoader::LoadTextFile(filePath.string(), &success);

        if (!success)
        {
            FX_CORE_WARN("Could not load dialogue lines for folder '{}'", folderName);
            return {};
        }

        return lines;
    }


    void Start() override
    {
        if (s_Instance && s_Instance != this)
        {
            // Es gibt bereits eine gültige Instanz – wir ignorieren diese neue
            FX_WARN("GameManagerScript: Instance already exists. Ignoring this one.");
            return;
        }

        s_Instance = this;

        CreateGameFolderStructure();

        // Rest des Codes
        Flux::Entity player(20, 14, '@', "@", true);
        entities.push_back(player);

        Flux::Entity questionNPC(33, 32, '?', "?", 0.2f, ImVec2(180, 370), false);
        questionNPC.layeredFrames = Flux::FileLoader::LoadAsciiFrames("QUEST_Animation.txt");
        questionNPC.dialogueLines = LoadDialogueLinesForSymbol("question_mark");
        entities.push_back(questionNPC);

        Flux::Entity andNPC(11, 9, '&', "&", 0.2f, ImVec2(240, 350), false);
        andNPC.layeredFrames = Flux::FileLoader::LoadAsciiFrames("AND_Animation.txt");
        andNPC.dialogueLines = LoadDialogueLinesForSymbol("and");
        entities.push_back(andNPC);

        Flux::Entity percentNPC(7, 23, '%', "%", 0.2f, ImVec2(300, 450), false);
        percentNPC.layeredFrames = Flux::FileLoader::LoadAsciiFrames("PERC_Animation.txt");
        percentNPC.dialogueLines = LoadDialogueLinesForSymbol("percent");
        entities.push_back(percentNPC);

        Flux::Entity equalsNPC(20, 20, 'a', "=", 0.2f, ImVec2(230, 350), false);
        equalsNPC.layeredFrames = Flux::FileLoader::LoadAsciiFrames("EQUA_Animation.txt");
        equalsNPC.dialogueLines = LoadDialogueLinesForSymbol("equals");
        entities.push_back(equalsNPC);

        Flux::Entity exclamationNPC(18, 14, '!', "!", 0.3f, ImVec2(170, 390), false);
        exclamationNPC.layeredFrames = Flux::FileLoader::LoadAsciiFrames("EXCL_Animation.txt");
        exclamationNPC.dialogueLines = LoadDialogueLinesForSymbol("exclamation_mark");
        entities.push_back(exclamationNPC);
    }

    void Update(float deltaTime) override {}

    void OnDestroy() override
    {
        if (s_Instance == this)
            s_Instance = nullptr;
    }
};

inline GameManagerScript* GameManagerScript::s_Instance = nullptr;

REGISTER_SCRIPT(GameManagerScript);
