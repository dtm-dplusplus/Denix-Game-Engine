#pragma once

#include "Denix/Scene/BaseObject.h"
#include "Denix/Scene/Object.h"
#include "Denix/Video/GL/Texture.h"
#include "Denix/Video/GL/Shader.h"


namespace Denix
{
	class Asset;

	
	class Material: public BaseObject
	{
	public:
		Material(const ObjectInitializer& _objInit = { "Material" });
		Material(const Ref<Material>& _other);
		Material(const Ref<Asset>& _asset);
		
		// Serialization
		void Serialize(YAML::Emitter& _out) override;
		void Deserialize(const YAML::Node& _in) override;
		YAML::Node Deserialize(const Ref<Asset>& _asset);
		// Getters and Setters 
		Ref<Shader> GetShader() const { return m_Shader; }
		void SetShader(const Ref<Shader>& _shader) { m_Shader = _shader; }

		// Albedo color or texture
		void SetBaseColor(const glm::vec3& _color) { m_BaseColor = _color; IsBaseTexture = false;}
		void SetBaseColor(const Ref<Texture>& _texture) { BaseTexture = _texture; IsBaseTexture = true; }
		Ref<Texture> GetBaseTexture() const { return BaseTexture; }
		Ref<Texture>& GetBaseTexture() { return BaseTexture; }
		glm::vec3 GetBaseColor() const { return m_BaseColor; }
		glm::vec3& GetBaseColor() { return m_BaseColor; }

		
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

		Ref<Asset> GetAsset() const { return m_Asset; }
		void SetAsset(const Ref<Asset>& _asset) { m_Asset = _asset; }
	private:
		// Base color or texture
		glm::vec3 m_BaseColor = glm::vec3(0.0f);
		Ref<Texture> BaseTexture;
		bool IsBaseTexture = false;
		
		Ref<Shader> m_Shader;
		Ref<Asset> m_Asset;

	private:
		float m_SpecularIntensity = 0.5f;
		float m_SpecularPower = 4.0f;

		float AO;
		float Metallic;
		float Roughness;
		
		friend class RendererSubsystem;
	};


}
