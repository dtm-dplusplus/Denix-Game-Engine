#pragma once
#include <GL/glew.h>

namespace Denix
{
	class Material
	{
	public:
		Material(): m_SpecularIntensity(0), m_SpecularPower(0) {}
		Material(const float _specIntensity, const float _specPower) : m_SpecularIntensity(_specIntensity), m_SpecularPower(_specPower) {}

		void UseMaterial(const GLint _specIntensityLocation, const GLint _specPowerLocation)
		{
			glUniform1f(_specIntensityLocation, m_SpecularIntensity);
			glUniform1f(_specPowerLocation, m_SpecularPower);
		}

		float GetSpecularPower() const { return m_SpecularPower; }
		float& GetSpecularPower() { return m_SpecularPower; }
		void SetSpecularPower(const float _power) { m_SpecularPower = _power; }

		float GetSpecularIntensity() const { return m_SpecularIntensity; }
		float& GetSpecularIntensity() { return m_SpecularIntensity; }
		void SetSpecularIntensity(const float _intensity) { m_SpecularIntensity = _intensity; }

	private:
		float m_SpecularIntensity;
		float m_SpecularPower;
	};


}
