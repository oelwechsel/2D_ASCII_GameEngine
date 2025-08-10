#pragma once
#include <Flux.h>

class TestConsole : public Flux::ImGuiConsole {
public:
    TestConsole();
    ~TestConsole();
    virtual void ExecCommand(const std::string& _command) override;
    virtual void AutoComplete(const std::string& _currentInput, std::vector<std::string>& _suggestions) override;
};
