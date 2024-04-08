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
			m_LightDirection(0.0f, 1.0f, 0.0f) 
		{
			m_AmbientIntensity = 0.1f;
			m_DiffuseIntensity = 0.5f;
		}

		glm::vec3 GetLightDirection() const { return m_LightDirection; }
		glm::vec3& GetLightDirection() { return m_LightDirection; }
		void SetLightDirection(const glm::vec3& _direction) { m_LightDirection = _direction; }

	protected:
		glm::vec3 m_LightDirection;
	};


	class PointLight : public Light
	{
	public:
		PointLight() : Light({"PointLight"}), constant(0.3f), linear(0.2f), exponent(0.1f)
		{
			m_AmbientIntensity = 0.0f;
			m_DiffuseIntensity = 1.0f;
		}

		PointLight(const ObjectInitializer& _objInit) : Light( _objInit ), constant(0.3f), linear(0.2f), exponent(0.1f)
		{
			m_AmbientIntensity = 0.0f;
			m_DiffuseIntensity = 1.0f;
		}

		float GetConstant() const { return constant; }
		float& GetConstant() { return constant; }
		void SetConstant(const float _constant) { constant = _constant; }

		float GetLinear() const { return linear; }
		float& GetLinear() { return linear; }
		void SetLinear(const float _linear) { linear = _linear; }

		float GetExponent() const { return exponent; }
		float& GetExponent() { return exponent; }
		void SetExponent(const float _exponent) { exponent = _exponent; }


	protected:
		float constant, linear, exponent;
		//glm::vec3 m_Attenuation;
		//float m_Range;
	};

	class SpotLight : public PointLight
	{
	public:
		SpotLight() : PointLight({"SpotLight"}), m_Direction(0.0f, -1.0f,0.0f), m_Edge(5), m_ProcessedEdge(0)
		{
			m_AmbientIntensity = 0.5f;
			m_DiffuseIntensity = 1.0f;
			m_ProcessedEdge = cosf(glm::radians(m_Edge));
		}


		void Update(float _deltaTime) override
		{
			// Update the processed edge
			m_ProcessedEdge = cosf(glm::radians(m_Edge));
		}

		glm::vec3 GetDirection() const { return m_Direction; }
		glm::vec3& GetDirection() { return m_Direction; }
		void SetDirection(const glm::vec3& _direction) { m_Direction = _direction; }

		float GetEdge() const { return m_Edge; }
		float& GetEdge() { return m_Edge; }

		float GetProcessedEdge() const { return m_ProcessedEdge; }
		float& GetProcessedEdge() { return m_ProcessedEdge; }
		private:
		glm::vec3 m_Direction;
		float m_Edge;
		float m_ProcessedEdge;
	};
}
