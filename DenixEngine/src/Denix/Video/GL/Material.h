#pragma once

#include "Denix/Scene/Object.h"
#include "Denix/Video/GL/Texture.h"
#include "Denix/Video/GL/Shader.h"

namespace Denix
{
	struct BaseMatParam
	{
		BaseMatParam() : Color(glm::vec3(1.0f)), IsTexture(false), Texture(nullptr) {}
		BaseMatParam(const glm::vec3& _color) : Color(_color), IsTexture(false), Texture(nullptr) {}
		BaseMatParam(const Ref<Texture>& _texture) : Color(glm::vec3(1.0f)), IsTexture(true), Texture(_texture) {}

		glm::vec3 Color;
		bool IsTexture;
		Ref<Texture> Texture;
	};

	struct NormalMatParam
	{
		NormalMatParam() : IsTexture(false), Texture(nullptr) {}
		NormalMatParam(const Ref<Texture>& _texture) : IsTexture(true), Texture(_texture) {}

		bool IsTexture;
		Ref<Texture> Texture;
	};

	class Material: public Object
	{
	public:
		Material(const ObjectInitializer& _objInit = { "Material" });
		Material(Ref<Material> _other);

		Ref<Shader> GetShader() const { return m_Shader; }
		void SetShader(const Ref<Shader>& _shader) { m_Shader = _shader; }

		BaseMatParam GetBaseParam() const { return m_BaseParam; }
		BaseMatParam& GetBaseParam() { return m_BaseParam; }
		void SetBaseParam(const BaseMatParam& _param) { m_BaseParam = _param; }


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
		BaseMatParam m_BaseParam;

		NormalMatParam m_NormalParam;

		Ref<Shader> m_Shader;

		float m_SpecularIntensity = 0.5f;
		float m_SpecularPower = 4.0f;

		float AO;
		float Metallic;
		float Roughness;
		
		friend class RendererSubsystem;
	};


}
