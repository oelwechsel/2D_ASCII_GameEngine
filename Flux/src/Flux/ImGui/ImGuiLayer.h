#pragma once

#include "Flux/Layer.h"

namespace Flux
{
	class FLUX_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate() override;
		virtual void OnEvent(Event& e) override;

	private:
		float m_Time;


	};
}