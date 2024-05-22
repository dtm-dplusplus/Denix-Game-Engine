#include "Pipe.h"

namespace Denix
{
    float PipePair::MoveSpeed{0.8f};
    float PipePair::SeperationMax{3.0f};
    float PipePair::SeperationMin{0.5f};
    float PipePair::DestructionDistance{20.0f};

    void PipePair::InitPipePair()
    {
        // Generate Random Seperation
        Seperation = 2.0f;

        // Set Pipe Positions
        const glm::vec3& pairPosition = m_TransformComponent->GetPosition();
        PipeTop->GetTransformComponent()->SetPosition(
            { pairPosition.x, pairPosition.y + Seperation / 2.0f, pairPosition.z });

        PipeBottom->GetTransformComponent()->SetPosition(
            { pairPosition.x, pairPosition.y -Seperation / 2.0f, pairPosition.z });
    }

    void PipePair::GameUpdate(float _deltaTime)
    {
        GameObject::GameUpdate(_deltaTime);

        // Update Pipe Position
        glm::vec3& position = m_TransformComponent->GetPosition();
        position.x -= MoveSpeed * _deltaTime;
        PipeTop->GetTransformComponent()->GetPosition().x = position.x;
        PipeBottom->GetTransformComponent()->GetPosition().x = position.x;

        // Cleanup when out of screen
        if (m_TransformComponent->GetPosition().x <= -DestructionDistance)
        {
            PipeTop->Destroy();
            PipeBottom->Destroy();
            Destroy();
        }
    }
}