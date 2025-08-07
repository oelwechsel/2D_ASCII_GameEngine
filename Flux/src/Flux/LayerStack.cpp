#include "fxpch.h"
#include "LayerStack.h"
#include "Flux/Log.h"

namespace Flux
{
	LayerStack::LayerStack()
	{
		FX_CORE_TRACE("LayerStack created.");
	}

	LayerStack::~LayerStack()
	{
		FX_CORE_TRACE("LayerStack destroyed. Releasing layers...");

		for (Layer* layer : m_Layers)
		{
			if (layer)
			{
				FX_CORE_INFO("Deleting layer: {0}", typeid(*layer).name());
				delete layer;
			}
			else
			{
				FX_CORE_WARN("Nullptr found in LayerStack during destruction.");
			}
		}

		m_Layers.clear();
	}

	void LayerStack::PushLayer(Layer* _layer)
	{
		if (!_layer)
		{
			FX_CORE_WARN("Tried to push nullptr as Layer.");
			return;
		}

		if (std::find(m_Layers.begin(), m_Layers.end(), _layer) != m_Layers.end())
		{
			FX_CORE_WARN("Tried to push a layer that already exists: {0}", typeid(*_layer).name());
			return;
		}

		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, _layer);
		m_LayerInsertIndex++;

		FX_CORE_INFO("Layer pushed: {0}", typeid(*_layer).name());
	}

	void LayerStack::PushOverlay(Layer* _overlay)
	{
		if (!_overlay)
		{
			FX_CORE_WARN("Tried to push nullptr as Overlay.");
			return;
		}

		if (std::find(m_Layers.begin(), m_Layers.end(), _overlay) != m_Layers.end())
		{
			FX_CORE_WARN("Tried to push an overlay that already exists: {0}", typeid(*_overlay).name());
			return;
		}

		m_Layers.emplace_back(_overlay);

		FX_CORE_INFO("Overlay pushed: {0}", typeid(*_overlay).name());
	}

	void LayerStack::PopLayer(Layer* _layer)
	{
		if (!_layer)
		{
			FX_CORE_WARN("Tried to pop nullptr as Layer.");
			return;
		}

		auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerInsertIndex, _layer);
		if (it != m_Layers.begin() + m_LayerInsertIndex)
		{
			m_Layers.erase(it);
			m_LayerInsertIndex--;

			FX_CORE_INFO("Layer popped: {0}", typeid(*_layer).name());
		}
		else
		{
			FX_CORE_WARN("Layer not found during PopLayer: {0}", typeid(*_layer).name());
		}
	}

	void LayerStack::PopOverlay(Layer* _overlay)
	{
		if (!_overlay)
		{
			FX_CORE_WARN("Tried to pop nullptr as Overlay.");
			return;
		}

		auto it = std::find(m_Layers.begin() + m_LayerInsertIndex, m_Layers.end(), _overlay);
		if (it != m_Layers.end())
		{
			m_Layers.erase(it);

			FX_CORE_INFO("Overlay popped: {0}", typeid(*_overlay).name());
		}
		else
		{
			FX_CORE_WARN("Overlay not found during PopOverlay: {0}", typeid(*_overlay).name());
		}
	}
}
