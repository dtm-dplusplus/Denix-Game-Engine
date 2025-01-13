#include "TransformComponent.h"
#include "Denix/Scene/SceneSubsystem.h"

namespace Denix
{
    TransformComponent::TransformComponent(): Component(ObjectInit("Transform Component")), m_Forward({0.0f, 0.0f, -1.0f}), m_Right({1.0f, 0.0f, 0.0f}), m_Up({0.0f, 1.0f, 0.0f}),
                                              m_Position(), m_Rotation(), m_Scale(),
                                              m_Model(1.0f),
                                              m_RotationMatrix(1.0f)
    {
    }

    void TransformComponent::BeginScene()
    {
        Component::BeginScene();

    }

    void TransformComponent::EndScene()
    {
        Component::EndScene();
    }

    void TransformComponent::Update(float _deltaTime)
    {
        glm::vec3 fwd;
        fwd.x = cos(glm::radians(m_Rotation.y)) * cos(glm::radians(m_Rotation.x));
        fwd.y = sin(glm::radians(m_Rotation.x));
        fwd.z = sin(glm::radians(m_Rotation.y)) * cos(glm::radians(m_Rotation.x));
        m_Forward = glm::normalize(fwd);

        m_Right = glm::normalize(glm::cross(m_Forward, {0.0f, 1.0f, 0.0f})); // World up
        m_Up = glm::normalize(glm::cross(m_Right, m_Forward));
        
        m_Model = glm::translate(glm::mat4(1.0f), m_Position);

        m_Model = glm::rotate(m_Model, glm::radians(m_Rotation.x), glm::vec3(1, 0, 0));
        m_Model = glm::rotate(m_Model, glm::radians(m_Rotation.y), glm::vec3(0, 1, 0));
        m_Model = glm::rotate(m_Model, glm::radians(m_Rotation.z), glm::vec3(0, 0, 1));
        
        // Scale Model by half to account for axis sign
        m_Model = glm::scale(m_Model, m_Scale / 2.0f);
    }

    void TransformComponent::UpdateRotationVectorFromMatrix()
    {
        // Update Rotation Vector
        m_Rotation.x = fmod(m_Rotation.x, 360.f);
        m_Rotation.y = fmod(m_Rotation.y, 360.f);
        m_Rotation.z = fmod(m_Rotation.z, 360.f);
    }
}
