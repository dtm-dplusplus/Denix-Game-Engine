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

		virtual void UploadUniforms() {}

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

	protected:
		BaseMatParam m_BaseParam;

		NormalMatParam m_NormalParam;

		Ref<Shader> m_Shader;

		float m_SpecularIntensity = 0.5f;
		float m_SpecularPower = 4.0f;

		friend class RendererSubsystem;
	};

	class DefaultMaterial : public Material
	{
	public:
		DefaultMaterial(const ObjectInitializer& _objInit = { "DefaultMaterial" }) : Material(_objInit)
		{
		}

		void UploadUniforms() override;
	};

}
