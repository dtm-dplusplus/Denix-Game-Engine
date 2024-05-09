#pragma once

#include "Denix/Scene/Object.h"
#include "Denix/Video/GL/Texture.h"

namespace Denix
{
	struct BaseColor
	{
		BaseColor() : Color(glm::vec3(1.0f)), IsTexture(false), Texture(nullptr) {}
		BaseColor(const glm::vec3& _color) : Color(_color), IsTexture(false), Texture(nullptr) {}
		BaseColor(const Ref<Texture>& _texture) : Color(glm::vec3(1.0f)), IsTexture(true), Texture(_texture) {}

		glm::vec3 Color;
		bool IsTexture;
		Ref<Texture> Texture;
	};
	class Material: public Object
	{
	public:
		Material(const std::string& _name = "Material") : Object({ _name }) {}

		//glm::vec3 GetBaseColor() const { return m_BaseColor.Color; }
		glm::vec3& GetBaseColor() { return m_BaseColor.Color; }
		void SetBaseColor(const glm::vec3& _color) { m_BaseColor.Color = _color; }

		float GetSpecularPower() const { return m_SpecularPower; }
		float& GetSpecularPower() { return m_SpecularPower; }
		void SetSpecularPower(const float _power) { m_SpecularPower = _power; }

		float GetSpecularIntensity() const { return m_SpecularIntensity; }
		float& GetSpecularIntensity() { return m_SpecularIntensity; }
		void SetSpecularIntensity(const float _intensity) { m_SpecularIntensity = _intensity; }

	private:
		BaseColor m_BaseColor;
		float m_SpecularIntensity = 0.5f;
		float m_SpecularPower = 4.0f;
	};


}
