#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Core.h"

#include "Object.h"
#include "Video/GL/VertexArray.h"
#include "Video/GL/VertexBuffer.h"
#include "Video/GL/GLShader.h"
#include "System/ShaderSubSystem.h"

namespace Denix
{
	/*	Base class for all object components */
	class Component: public Object
	{
	public:
		// Constructors
		Component(const ObjectInitializer& _objectInitializer = ObjectInitializer::Get()) : Object(_objectInitializer), m_ParentObjectID{0}
		{
			//DE_LOG(LogObject, Trace, "Created Component: {} without Parent Object", m_Name)
		}

		Component(const GLint _parentID, const ObjectInitializer& _objectInitializer = ObjectInitializer::Get()) : Object(_objectInitializer), m_ParentObjectID{ _parentID }
		{
			//DE_LOG(LogObject, Trace, "Created Component: {} with Parent Object ID: {}", m_Name, m_ParentObjectID)
		}

		// Destructors
		~Component() override = default;

	public: // Component Interface
		/** Get the parent object id */
		GLint GetParentObjectID() const { return m_ParentObjectID; }

		/** Set the parent object id */
		void SetParentObjectID(const GLint _id) { m_ParentObjectID = _id; }

	public: // Object Interface
		virtual void Initialize() {}
		virtual void Deinitialize() {}

		void BeginScene() override {}
		void EndScene() override {}
		void Update(float _deltaTime) override {}

	protected:
		/* Object this component is attacthed to */
		GLint m_ParentObjectID;
	};


	constexpr float SqureData[] = {
			-0.5f, 0.5f, 0.0f, // top left point
			0.5f, 0.5f, 0.0f, // top right point
			0.5f, -0.5f, 0.0f, // bottom right point
			0.5f, -0.5f, 0.0f, // bottom right point
			-0.5f, -0.5f, 0.0f, // bottom left point
			-0.5f, 0.5f, 0.0f // top left point
	};

	class MeshComponent : public Component
	{
	public:
		MeshComponent() : Component(ObjectInitializer("Mesh Component")) {}

		MeshComponent(const GLint _parentID) : Component(_parentID, ObjectInitializer("Mesh Component")) {}

		MeshComponent(const GLint _parentID, 
			const GLenum _target, const GLsizei _size, const void* _data, const GLuint _count, 
			const GLenum _type, const GLenum _drawType = GL_TRIANGLES) :
				Component(_parentID, ObjectInitializer("Mesh Component"))
		{
			m_VBO = MakeRef<VertexBuffer>(_target, sizeof(SqureData), SqureData, _count, _type,  _drawType);

			m_VAO = MakeRef<VertexArray>();
			m_VAO->GenVertexArray();
			m_VAO->Bind();

			// Bind the vbos & attribs
			m_VBO->Bind(m_VBO->GetTarget());
			m_VAO->AttribPtr(m_VBO->GetCount(), m_VBO->GetType());

			// Reset the state
			VertexBuffer::Unbind(GL_ARRAY_BUFFER);
			VertexArray::Unbind();
		}

		~MeshComponent() override =  default;

		void BindVAO() const { if(m_VAO) m_VAO->Bind(); }
		void BindVBO() const { if(m_VBO) m_VBO->Bind(m_VBO->GetTarget()); }	
	private:
		Ref<VertexArray> m_VAO;
		Ref<VertexBuffer> m_VBO;
	
		friend class RendererSubSystem;

	};



	class TransformComponent : public Component
	{
	public:
		// Constructors
		TransformComponent() : Component(ObjectInitializer("Transform Component")) {}
		TransformComponent(const GLint _parentID) : Component(_parentID, ObjectInitializer("Transform Component")) {}

		// Destructors
		~TransformComponent() override = default;

	
		void EndScene() override
		{
			m_Position = glm::vec3(0.f);
			m_Rotation = glm::vec3(0.f);
			m_Scale = glm::vec3(1.f);
		}

		// Getters
		glm::mat4 GetModel() const { return Model; }
		glm::mat4& GetModel() { return Model; }

		glm::vec3 GetPosition() const { return m_Position; }
		glm::vec3& GetPosition() { return m_Position; }

		glm::vec3 GetRotation() const { return m_Rotation; }
		glm::vec3& GetRotation() { return m_Rotation; }

		glm::vec3 GetScale() const { return m_Scale; }
		glm::vec3& GetScale() { return m_Scale; }

		// Setters
		void SetPosition(const glm::vec3& _position) { m_Position = _position; }
		void SetRotation(const glm::vec3& _rotation) { m_Rotation = _rotation; }
		void SetScale(const glm::vec3& _scale) { m_Scale = _scale; }
	
	protected:
		glm::vec3 m_Position = glm::vec3(0.f);
		glm::vec3 m_Rotation = glm::vec3(0.f);
		glm::vec3 m_Scale = glm::vec3(1.f);

		glm::mat4 Model = glm::mat4(1.f);
	};

	class PhysicsComponent : public Component
	{
	public:
		PhysicsComponent() : Component(ObjectInitializer("Physics Component")) 
		{
		}
		PhysicsComponent(const GLint _parentID) : Component(_parentID, ObjectInitializer("Physics Component")) {}


		// Destructors
		~PhysicsComponent() override = default;

		enum class StepMethod
		{
			Euler,
			RK2,
			RK4,
			Verlet
		};

	public:
		void Step(float _deltaTime)
		{
			// Step integration
			switch (m_StepMethod)
			{
			case StepMethod::Euler:
				StepEuler(_deltaTime);
				break;

			case StepMethod::RK2:
				StepRK2(_deltaTime);
				break;

			case StepMethod::RK4:
				StepRK4(_deltaTime);
				break;

			case StepMethod::Verlet:
				StepVerlet(_deltaTime);
				break;
			}
		}
		void StepEuler(float _deltaTime)
		{
			// Calculate the net force
			const glm::vec3 netF = m_Force - m_Drag * m_Velocity;

			// Calculate new velocity at time t + dt
			const glm::vec3 vNew = m_Velocity + (netF / m_Mass) * _deltaTime;

			// Calculate new displacement at time t + dt
			const glm::vec3 disp = m_TempPosition + vNew * _deltaTime;

			// Update the object's velocity and position
			m_Velocity = vNew;
			m_TempPosition = disp;
		}

		void StepRK2(float _deltaTime)
		{
			glm::vec3 netF;
			glm::vec3 accel;

			// Calculate k1
			netF = m_Force - m_Drag * m_Velocity;
			accel = netF / m_Mass;
			const glm::vec3 k1 = accel * _deltaTime;

			// Calculate k2
			netF = m_Force - m_Drag * (m_Velocity + k1);
			accel = netF / m_Mass;
			const glm::vec3 k2 = accel * _deltaTime;

			// Calculate new velocity at time t + dt
			m_Velocity += (k1 + k2) / 2.f;

			// Calculate new displacement at time t + dt
			m_TempPosition += m_Velocity * _deltaTime;
		}

		void StepRK4(float _deltaTime)
		{
		}

		void StepVerlet(float _deltaTime)
		{
		}

		void ToggleSimulation()
		{
			m_IsSimulated = !m_IsSimulated;

			if(m_IsSimulated)
			{
				DE_LOG(LogPhysics, Trace, "Physics simulation enabled")
			}
			else
			{
				DE_LOG(LogPhysics, Trace, "Physics simulation disabled")
			}
		}

		void ToggleGravity()
		{
			m_IsCustomGravity = !m_IsCustomGravity;

			if(m_IsCustomGravity)
			{
				DE_LOG(LogPhysics, Trace, "Custom Gravity enabled")

				// Get the scene gravity (future)
				// Just use the default gravity for now
				m_Gravity = glm::vec3(0.f, -9.81, 0.f);
			}
			else
			{
				DE_LOG(LogPhysics, Trace, "Custom Gravity disabled")
			}
		}
		// Getters
		bool IsSimulated() const { return m_IsSimulated; }
		bool& IsSimulated() { return m_IsSimulated; }

		void SetStepMethod(StepMethod _method) 
		{
			m_StepMethod = _method; 
		
		
			DE_LOG(LogPhysics, Trace, "Physics step method set to: {}", GetStepMethod(_method))
		}
		StepMethod GetStepMethod() const { return m_StepMethod; }
		std::string GetStepMethod(StepMethod _method) const
		{
			const static std::string s[]{ "Euler", "k2", "k4", "Verlet" };
			return s[static_cast<int>(m_StepMethod)];
		}
		bool IsCustomGravity() const { return m_IsCustomGravity; }
		bool& IsCustomGravity() { return m_IsCustomGravity; }

		glm::vec3 GetVelocity() const { return m_Velocity; }
		glm::vec3& GetVelocity() { return m_Velocity; }

		glm::vec3 GetAngularVelocity() const { return m_AngularVelocity; }
		glm::vec3& GetAngularVelocity() { return m_AngularVelocity; }

		glm::vec3 GetAcceleration() const { return m_Acceleration; }
		glm::vec3& GetAcceleration() { return m_Acceleration; }

		glm::vec3 GetForce() const { return m_Force; }
		glm::vec3& GetForce() { return m_Force; }

		float GetMass() const { return m_Mass; }
		float& GetMass() { return m_Mass; }

		glm::vec3 GetDrag() const { return m_Drag; }
		glm::vec3& GetDrag() { return m_Drag; }

		glm::vec3 GetGravity() const { return m_Gravity; }
		glm::vec3& GetGravity() { return m_Gravity; }

	private:
		/** Set to decide if the physics component should update simulation */
		bool m_IsSimulated = true;

		/** Set to decide if the physics component should use custom gravity
		 * If false, the physics component will use the scene gravity
		 */
		bool m_IsCustomGravity = false;

		/** Method used to step the physics simulation */
		StepMethod m_StepMethod = StepMethod::Euler;

	private:
		/** Mass of the object */
		float m_Mass = 1.0f;

		/** Gravitational forceacting on the object */
		glm::vec3 m_Gravity = glm::vec3(0.f, -9.81, 0.f);

		/** Drag force acting on the object */
		glm::vec3 m_Drag = glm::vec3(0.f);

		/** Velocity of the object */
		glm::vec3 m_Velocity = glm::vec3(0.f);

		/** Angular velocity of the object */
		glm::vec3 m_AngularVelocity = glm::vec3(0.f);

		/** Acceleration of the object */
		glm::vec3 m_Acceleration = glm::vec3(0.f);

		/** Force acting on the object */
		glm::vec3 m_Force = glm::vec3(0.f);

		/** Temporary position of the object
		 * Will be removed in the future
		 */
		glm::vec3 m_TempPosition = glm::vec3(0.f);

		friend class SceneSubSystem;
		friend class PhysicsSubSystem;
		friend class GameObject;
	};

	class RenderComponent : public Component
	{
	public:
		RenderComponent() : Component(ObjectInitializer("Render Component"))
		{
			if (!m_Shader)
			{
				if (const Ref<GLShader> shader = ShaderSubSystem::Get()->GetShader("DebugShader"))
				{
					m_Shader = shader;
					DE_LOG(LogRender, Trace, "RenderComponent: {} Shader set to DebugShader", m_Name)
				}
			}
		}

		RenderComponent(const GLint _parentID) : Component(_parentID, ObjectInitializer("Render Component"))
		{
			if (!m_Shader)
			{
				if (const Ref<GLShader> shader = ShaderSubSystem::Get()->GetShader("DebugShader"))
				{
					m_Shader = shader;
					DE_LOG(LogRender, Trace, "RenderComponent: {} Shader set to DebugShader", m_Name)
				}
			}
		}

		~RenderComponent() override = default;

		void Initialize() override
		{
		}

		void Deinitialize() override
		{
		}

		void BeginScene() override
		{
			
		}

		void EndScene() override{}

		void Update(float _deltaTime) override
		{

		}

	public:
		void BindShader() const { if (m_Shader) m_Shader->Bind(); }
		void SetShader(const Ref<GLShader>& _shader) { m_Shader = _shader; }
		Ref<GLShader> GetShader() const { return m_Shader; }

	public:
		glm::vec4 GetDebugColor() const { return m_DebugColor; }
		glm::vec4& GetDebugColor() { return m_DebugColor; }
		glm::vec4 SetDebugColor(const glm::vec4& _color) { return m_DebugColor = _color; }

	private:
		glm::vec4 m_DebugColor = glm::vec4(0.98f, 1.f, 1.f, 1.f);
		Ref<GLShader> m_Shader;

		friend class SceneSubSystem;
		friend class RendererSubSystem;
	};
}