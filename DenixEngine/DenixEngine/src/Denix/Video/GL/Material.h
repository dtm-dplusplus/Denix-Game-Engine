#pragma once

#include <glm/vec3.hpp>

#include "Denix/Scene/Object.h"

namespace Denix
{
	class Material: public Object
	{
	public:
		Material(const std::string& _name = "Material") : Object({ _name }) {}

		glm::vec3 GetBaseColor() const { return m_BaseColor; }
		glm::vec3& GetBaseColor() { return m_BaseColor; }
		void SetBaseColor(const glm::vec3& _color) { m_BaseColor = _color; }

		float GetSpecularPower() const { return m_SpecularPower; }
		float& GetSpecularPower() { return m_SpecularPower; }
		void SetSpecularPower(const float _power) { m_SpecularPower = _power; }

		float GetSpecularIntensity() const { return m_SpecularIntensity; }
		float& GetSpecularIntensity() { return m_SpecularIntensity; }
		void SetSpecularIntensity(const float _intensity) { m_SpecularIntensity = _intensity; }

	private:
		glm::vec3 m_BaseColor = glm::vec3(1.0f);
		float m_SpecularIntensity = 0.5f;
		float m_SpecularPower = 4.0f;
	};


}
