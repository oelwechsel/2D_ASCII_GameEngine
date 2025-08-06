#pragma once
#include <Flux.h>

class TestConsole : public Flux::ImGuiConsole {
public:
    TestConsole();
    ~TestConsole();
    virtual void ExecCommand(const std::string& command) override;
    virtual void AutoComplete(const std::string& currentInput, std::vector<std::string>& suggestions) override;

private:
    std::unordered_set<std::string> Commands;
};
