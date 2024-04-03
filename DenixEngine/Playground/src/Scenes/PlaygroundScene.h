#pragma once

#include <filesystem>

#include "Denix/Scene/Scene.h"

namespace Denix
{
	
	class Light : public GameObject
	{
	public:

		Light(): GameObject({ "Light" }),
			m_LightColor(glm::vec3(1.0f)),
			m_AmbientIntensity(0.5f),
			m_LightDirection(0.0f, -1.0f, 0.0f),
			m_DiffuseIntensity(0.0f) {}
		

		void UseLight()
		{
			m_RenderComponent->GetShader()->Bind();

			glUniform3f(GetRenderComponent()->GetShader()->GetUniform("u_DirLight.Direction"), m_LightDirection.x, m_LightDirection.y, m_LightDirection.z);
			glUniform1f(GetRenderComponent()->GetShader()->GetUniform("u_DirLight.DiffuseIntensity"), m_DiffuseIntensity);

			glUniform3f(GetRenderComponent()->GetShader()->GetUniform("u_DirLight.Albedo"), m_LightColor.r, m_LightColor.g, m_LightColor.b);
			glUniform1f(GetRenderComponent()->GetShader()->GetUniform("u_DirLight.AmbientIntensity"), m_AmbientIntensity);
			GLShader::Unbind();
		}

		glm::vec3 GetLightColor() const { return m_LightColor; }

		glm::vec3 m_LightColor;
		GLfloat m_AmbientIntensity;

		glm::vec3 m_LightDirection;
		GLfloat m_DiffuseIntensity;
	};
	class PlaygroundScene final : public Scene
	{
	public:
		PlaygroundScene(const ObjectInitializer& _objInit = ObjectInitializer("PlaygroundScene")) {}

		bool Load() override;
		void Update(float _deltaTime) override;

		Ref<Cube> Brick;
		Ref<Light> LightSource;
	};
}
