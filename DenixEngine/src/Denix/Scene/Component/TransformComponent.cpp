#include "TransformComponent.h"
#include "Denix/Scene/SceneSubsystem.h"

namespace Denix
{
    void TransformComponent::BeginScene()
    {
        Component::BeginScene();

        RegisterComponent();
    }

    void TransformComponent::EndScene()
    {
        UnregisterComponent();
    
        Component::EndScene();
    }

    void TransformComponent::RegisterComponent()
    {
        Component::RegisterComponent();

        /*if (SceneSubsystem* sceneSystem = SceneSubsystem::Get())
        {
            sceneSystem->RegisterTransformComponent(shared_from_this());
        }*/
    }

    void TransformComponent::UnregisterComponent()
    {
        /*if (SceneSubsystem* sceneSystem = SceneSubsystem::Get())
        {
            sceneSystem->UnregisterTransformComponent(shared_from_this());
        }*/
    }

    void TransformComponent::Update(float _deltaTime, const Ref<Counter>& _waitCounter)
    {
        m_Model = glm::translate(glm::mat4(1.0f), m_Position);

        m_Model = glm::rotate(m_Model, glm::radians(m_Rotation.x), glm::vec3(1, 0, 0));
        m_Model = glm::rotate(m_Model, glm::radians(m_Rotation.y), glm::vec3(0, 1, 0));
        m_Model = glm::rotate(m_Model, glm::radians(m_Rotation.z), glm::vec3(0, 0, 1));
        
        /*glm::mat4 rotationMatrix = glm::mat4(m_RotationMatrix);
        m_Model *= rotationMatrix;*/

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
