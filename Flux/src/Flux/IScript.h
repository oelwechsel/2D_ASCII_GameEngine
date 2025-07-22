#pragma once

#include "Events/Event.h"

namespace Flux {

    class FLUX_API IScript
    {
    public:
        IScript();
        virtual ~IScript();

        virtual void Start() = 0;
        virtual void Update(float deltaTime) = 0;
        virtual void OnImGuiRender() {}
        virtual void OnEvent(Event& event);
    };

}
