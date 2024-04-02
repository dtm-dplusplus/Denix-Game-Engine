#pragma once

#include <filesystem>

#include "Denix/Scene/Scene.h"

namespace Denix
{
	
	class Light : public GameObject
	{
	public:

		Light(const glm::vec3& _color, GLfloat _ambientIntensity, const ObjectInitializer& _objInit = ObjectInitializer("Light")):
			GameObject(_objInit), m_LightColor(_color), m_AmbientIntensity(_ambientIntensity)
		{}

		Light(const ObjectInitializer& _objInit = ObjectInitializer("Light")):
			GameObject(_objInit), m_LightColor(glm::vec3(1.0f)), m_AmbientIntensity(0.5f)
		{}

		void UseLight()
		{
			m_RenderComponent->GetShader()->Bind();
			glUniform3f(GetRenderComponent()->GetShader()->GetUniform("u_DirLight.Albedo"), m_LightColor.r, m_LightColor.g, m_LightColor.b);
			glUniform1f(GetRenderComponent()->GetShader()->GetUniform("u_DirLight.AmbientIntensity"), m_AmbientIntensity);
			GLShader::Unbind();
		}

		glm::vec3 GetLightColor() const { return m_LightColor; }

		glm::vec3 m_LightColor;
		GLfloat m_AmbientIntensity;
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
