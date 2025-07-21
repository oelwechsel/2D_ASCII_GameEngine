#pragma once
#include <Flux.h>
#include "ScriptRegistry.h"

class ScriptManagerLayer : public Flux::Layer
{
    std::vector<std::unique_ptr<Flux::IScript>> m_ActiveScripts;

public:
    ScriptManagerLayer()
        : Layer("ScriptManager") {
    }

    void OnAttach() override
    {
        // Beispiel: Erstelle automatisch alle Scripts (oder nur bestimmte)
        //m_ActiveScripts.push_back(ScriptRegistry::Get().CreateScript("TestScript"));
        /*
        * 
        // weitere Scripts können hier geladen oder dynamisch instanziiert werden
        for (auto& script : m_ActiveScripts)
            script->Start();

        */

        for (const auto& [name, createFn] : ScriptRegistry::Get().GetAllScripts())
        {
            auto script = createFn();
            script->Start();
            m_ActiveScripts.push_back(std::move(script));
        }
    }

    void OnUpdate() override
    {
        float deltaTime = Flux::Application::Get().GetDeltaTime();
        for (auto& script : m_ActiveScripts)
            script->Update(deltaTime);
    }

    void OnImGuiRender() override
    {
        for (auto& script : m_ActiveScripts)
            script->OnImGuiRender();
    }
};
