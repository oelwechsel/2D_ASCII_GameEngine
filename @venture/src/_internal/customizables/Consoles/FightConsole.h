#pragma once
#include <Flux.h>



class FightConsole : public Flux::ImGuiConsole {
public:
    FightConsole();
    ~FightConsole();

    bool m_WaitingForPassword;
    std::string m_PasswordInput;
    std::string m_Password = "cGFzc3dvcnQ=";

    virtual void ExecCommand(const std::string& _command) override;
    virtual void AutoComplete(const std::string& _currentInput, std::vector<std::string>& _suggestions) override;
};
