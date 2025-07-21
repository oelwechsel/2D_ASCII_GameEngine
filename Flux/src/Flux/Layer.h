#pragma once

#include "Flux/Core.h"
#include "Flux/Events/Event.h"

namespace Flux
{
	class FLUX_API Layer
	{
	public:
		Layer(const std::string& _name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& _event) {}

		inline const std::string& GetName() const { return m_DebugName; }

	protected:
		std::string m_DebugName;
	};
}
