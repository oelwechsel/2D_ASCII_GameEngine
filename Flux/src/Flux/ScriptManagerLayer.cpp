#include "fxpch.h"
#include "ScriptManagerLayer.h"
#include "Flux/Log.h"

namespace Flux
{
    ScriptManagerLayer::ScriptManagerLayer()
        : Layer("ScriptManager")
    {
        FX_CORE_INFO("ScriptManagerLayer constructed.");
    }

    void ScriptManagerLayer::OnAttach()
    {
        FX_CORE_INFO("ScriptManagerLayer: OnAttach called. Loading scripts...");

        for (const auto& [name, createFn] : ScriptRegistry::Get().GetAllScripts())
        {
            if (!createFn)
            {
                FX_CORE_WARN("Script '{0}' has no valid creation function.", name);
                continue;
            }

            std::unique_ptr<IScript> script;

            try
            {
                script = createFn();
            }
            catch (const std::exception& e)
            {
                FX_CORE_ERROR("Exception while creating script '{0}': {1}", name, e.what());
                continue;
            }
            catch (...)
            {
                FX_CORE_ERROR("Unknown error while creating script '{0}'", name);
                continue;
            }

            if (!script)
            {
                FX_CORE_ERROR("Script '{0}' creation returned nullptr.", name);
                continue;
            }

            try
            {
                FX_CORE_INFO("Starting script: {0}", name);
                script->Start();
            }
            catch (const std::exception& e)
            {
                FX_CORE_ERROR("Exception in Start() of script '{0}': {1}", name, e.what());
                continue;
            }
            catch (...)
            {
                FX_CORE_ERROR("Unknown exception in Start() of script '{0}'", name);
                continue;
            }

            m_ActiveScripts.push_back(std::move(script));
        }

        if (m_ActiveScripts.empty())
        {
            FX_CORE_WARN("No scripts were started. Check ScriptRegistry.");
        }
    }

    void ScriptManagerLayer::OnUpdate(float deltaTime)
    {
        for (auto& script : m_ActiveScripts)
        {
            try
            {
                script->Update(deltaTime);
            }
            catch (const std::exception& e)
            {
                FX_CORE_ERROR("Exception in Update(): {0}", e.what());
            }
            catch (...)
            {
                FX_CORE_ERROR("Unknown exception in Update()");
            }
        }
    }

    void ScriptManagerLayer::OnImGuiRender()
    {
        for (auto& script : m_ActiveScripts)
        {
            try
            {
                script->OnImGuiRender();
            }
            catch (const std::exception& e)
            {
                FX_CORE_ERROR("Exception in OnImGuiRender(): {0}", e.what());
            }
            catch (...)
            {
                FX_CORE_ERROR("Unknown exception in OnImGuiRender()");
            }
        }
    }
}
