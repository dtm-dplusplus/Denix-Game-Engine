#pragma once

#include "Denix/Scene/Object.h"
#include "Denix/Video/GL/Texture.h"
#include "Denix/Video/GL/Shader.h"

namespace Denix
{
	class Material: public Object
	{
	public:
		Material(const ObjectInitializer& _objInit = { "Material" });
		Material(Ref<Material> _other);

		Ref<Shader> GetShader() const { return m_Shader; }
		void SetShader(const Ref<Shader>& _shader) { m_Shader = _shader; }

		// Albedo color or texture
		void SetBaseColor(const glm::vec3& _color) { BaseColor = _color; IsBaseTexture = false;}
		void SetBaseColor(const Ref<Texture>& _texture) { BaseTexture = _texture; IsBaseTexture = true; }
		Ref<Texture> GetBaseTexture() const { return BaseTexture; }
		Ref<Texture>& GetBaseTexture() { return BaseTexture; }
		glm::vec3 GetBaseColor() const { return BaseColor; }
		glm::vec3& GetBaseColor() { return BaseColor; }

		
		bool CheckBaseType() { IsBaseTexture = IsValid(BaseTexture); return IsBaseTexture;}

		/**
		 *  @brief Get the base type of the material
		 * @return true if the base is a texture, false if it is a color
		 */
		bool IsBaseATexture() const { return IsBaseTexture; }

		float GetSpecularPower() const { return m_SpecularPower; }
		float& GetSpecularPower() { return m_SpecularPower; }
		void SetSpecularPower(const float _power) { m_SpecularPower = _power; }

		float GetSpecularIntensity() const { return m_SpecularIntensity; }
		float& GetSpecularIntensity() { return m_SpecularIntensity; }
		void SetSpecularIntensity(const float _intensity) { m_SpecularIntensity = _intensity; }

		float GetAO() const { return AO; }
		float& GetAO() { return AO; }
		void SetAO(const float _ao) { AO = _ao; }

		float GetMetallic() const { return Metallic; }
		float& GetMetallic() { return Metallic; }
		void SetMetallic(const float _metallic) { Metallic = _metallic; }

		float GetRoughness() const { return Roughness; }
		float& GetRoughness() { return Roughness; }
		void SetRoughness(const float _roughness) { Roughness = _roughness; }
		
	private:
		// Base color or texture
		glm::vec3 BaseColor = glm::vec3(0.0f);
		Ref<Texture> BaseTexture;
		bool IsBaseTexture = false;
		
		Ref<Shader> m_Shader;

		float m_SpecularIntensity = 0.5f;
		float m_SpecularPower = 4.0f;

		float AO;
		float Metallic;
		float Roughness;
		
		friend class RendererSubsystem;
	};


}
