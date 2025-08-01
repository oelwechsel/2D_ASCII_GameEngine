#include "fxpch.h"   
#include "IScript.h"

namespace Flux {

	IScript::IScript() = default;

    IScript::~IScript()
    {
        OnDestroy();
    }

	void IScript::OnEvent(Event& event) {}

    void IScript::OnDestroy() {}
}
