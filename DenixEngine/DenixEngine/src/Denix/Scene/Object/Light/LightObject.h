#pragma once

#include "Denix/Core.h"
#include "Denix/Scene/GameObject.h"

namespace Denix
{
	class Light : public GameObject
	{
	public:
		Light(const ObjectInitializer& _objInit = {"Light"}):
			GameObject(_objInit),
			m_LightColor(1.0f, 1.0f, 1.0f),
			m_AmbientIntensity(0.1f),
			m_DiffuseIntensity(1.0f)
		{
			m_MeshComponent->CreateMesh((GLfloat*)CubeData, (unsigned int*)CubeIndices, 40, 36);
			m_TransformComponent->SetScale(glm::vec3(0.5f));

			m_RenderComponent->SetAffectsLighting(false);
			m_RenderComponent->GetTexture()->m_BaseColor = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
		}

		glm::vec3 GetLightColor() const { return m_LightColor; }
		glm::vec3& GetLightColor() { return m_LightColor; }
		void SetLightColor(const glm::vec3& _color) { m_LightColor = _color; }

		float GetAmbientIntensity() const { return m_AmbientIntensity; }
		float& GetAmbientIntensity() { return m_AmbientIntensity; }
		void SetAmbientIntensity(const float _intensity) { m_AmbientIntensity = _intensity; }

		float GetDiffuseIntensity() const { return m_DiffuseIntensity; }
		float& GetDiffuseIntensity() { return m_DiffuseIntensity; }
		void SetDiffuseIntensity(const float _intensity) { m_DiffuseIntensity = _intensity; }

		// Constraints to test in ImGui
		glm::vec2 m_AmbientConstraints = { 0.0f, 1.0f };
		glm::vec2 m_DiffuseConstraints = { 0.0f, 3.0f };

	protected:
		glm::vec3 m_LightColor;

		float m_AmbientIntensity;

		float m_DiffuseIntensity;
	};

	class DirectionalLight : public Light
	{
	public:

		DirectionalLight() : Light({ "DirectionalLight" }),
			m_LightDirection(0.0f, -1.0f, 0.0f)
		{

			m_MeshComponent->CreateMesh((GLfloat*)CubeData, (unsigned int*)CubeIndices, 40, 36);
			m_TransformComponent->SetScale(glm::vec3(0.5f));

			m_RenderComponent->SetAffectsLighting(false);
			m_RenderComponent->GetTexture()->m_BaseColor = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
		}


		void UseLight(GLint _u_directon, GLint _u_diffIntensity, GLint _u_color, GLint _u_ambientIntensity)
		{
			m_RenderComponent->GetShader()->Bind();
			const glm::vec3& position = m_TransformComponent->GetPosition();
			glUniform3f(GetRenderComponent()->GetShader()->GetUniform("u_DirLight.Direction"), m_LightDirection.x, m_LightDirection.y, m_LightDirection.z);
			glUniform1f(GetRenderComponent()->GetShader()->GetUniform("u_DirLight.DiffuseIntensity"), m_DiffuseIntensity);
			glUniform3f(GetRenderComponent()->GetShader()->GetUniform("u_DirLight.Color"), m_LightColor.r, m_LightColor.g, m_LightColor.b);
			glUniform1f(GetRenderComponent()->GetShader()->GetUniform("u_DirLight.AmbientIntensity"), m_AmbientIntensity);

			//glUniform3f(_u_directon, position.x, position.y, position.z);
			//glUniform1f(_u_diffIntensity, m_DiffuseIntensity);
			//glUniform3f(_u_color, m_LightColor.r, m_LightColor.g, m_LightColor.b);
			//glUniform1f(_u_ambientIntensity, m_AmbientIntensity);

			GLShader::Unbind();
		}

		glm::vec3 GetLightDirection() const { return m_LightDirection; }
		glm::vec3& GetLightDirection() { return m_LightDirection; }
		void SetLightDirection(const glm::vec3& _direction) { m_LightDirection = _direction; }

	protected:
		glm::vec3 m_LightDirection;
	};
}
