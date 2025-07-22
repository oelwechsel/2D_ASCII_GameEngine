#pragma once
#include "ScriptRegistry.h"
#include "Flux/Layer.h"
#include "Flux/IScript.h"

namespace Flux
{

    class FLUX_API ScriptManagerLayer : public Layer
    {
        std::vector<std::unique_ptr<IScript>> m_ActiveScripts;

    public:
        ScriptManagerLayer()
            : Layer("ScriptManager") {
        }

        ScriptManagerLayer(const ScriptManagerLayer&) = delete;
        ScriptManagerLayer& operator=(const ScriptManagerLayer&) = delete;

        ScriptManagerLayer(ScriptManagerLayer&&) = default;
        ScriptManagerLayer& operator=(ScriptManagerLayer&&) = default;

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

        void OnUpdate(float _deltaTime) override
        {
            for (auto& script : m_ActiveScripts)
                script->Update(_deltaTime);
        }

        void OnImGuiRender() override
        {
            for (auto& script : m_ActiveScripts)
                script->OnImGuiRender();
        }
    };

}
