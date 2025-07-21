#pragma once

#include "Flux/Layer.h"

#include "Flux/Events/ApplicationEvent.h"
#include "Flux/Events/KeyEvent.h"
#include "Flux/Events/MouseEvent.h"

namespace Flux
{
	class FLUX_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;
		
		void Begin();
		void End();

	private:
		float m_Time = 0.0f;


	};
}