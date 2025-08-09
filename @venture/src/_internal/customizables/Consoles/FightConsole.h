#pragma once
#include <Flux.h>



class FightConsole : public Flux::ImGuiConsole {
public:
    FightConsole();
    ~FightConsole();

    bool m_WaitingForPassword;
    std::string m_PasswordInput;
    std::string m_Password = "cGFzc3dvcnQ=";

    virtual void ExecCommand(const std::string& command) override;
    virtual void AutoComplete(const std::string& currentInput, std::vector<std::string>& suggestions) override;
};
