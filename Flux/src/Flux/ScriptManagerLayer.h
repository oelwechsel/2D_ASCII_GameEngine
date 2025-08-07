#pragma once

#include "ScriptRegistry.h"
#include "Flux/Layer.h"
#include "Flux/IScript.h"

namespace Flux
{
    class FLUX_API ScriptManagerLayer : public Layer
    {
    public:
        ScriptManagerLayer();
        ~ScriptManagerLayer() override = default;

        ScriptManagerLayer(const ScriptManagerLayer&) = delete;
        ScriptManagerLayer& operator=(const ScriptManagerLayer&) = delete;

        ScriptManagerLayer(ScriptManagerLayer&&) = default;
        ScriptManagerLayer& operator=(ScriptManagerLayer&&) = default;

        void OnAttach() override;
        void OnUpdate(float deltaTime) override;
        void OnImGuiRender() override;

    private:
        std::vector<std::unique_ptr<IScript>> m_ActiveScripts;
    };
}
