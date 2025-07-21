#include "fxpch.h"   
#include "IScript.h"

namespace Flux {

	IScript::IScript() = default;
	IScript::~IScript() = default;

	void IScript::OnEvent(Event& event) {}
}
