#pragma once

#include "Denix/Scene/Component.h"
#include "Denix/Video/GL/Model.h"

namespace Denix
{
	class
	ModelComponent final : public Component
	{
	public:
		ModelComponent();

		Ref<Model> GetModel() const { return m_Model; }
		Ref<Model>& GetModel() { return m_Model; }
		void SetModel(const Ref<Model>& _model) { m_Model = _model; }

	private:
		void Serialize(YAML::Emitter& _out) override;
		void Deserialize(const YAML::Node& _in) override;
		
		Ref<Model> m_Model;

		friend class Actor;
		friend class RendererSubsystem;
	};
}
