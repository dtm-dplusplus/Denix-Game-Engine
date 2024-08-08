#include "Pipe.h"
#include <cstdlib> 
#include <ctime> 

namespace Denix
{
    float PipePair::MoveSpeed{0.8f};
    bool PipePair::CanMove{false};
    float PipePair::SeperationMax{2.0f};
    float PipePair::SeperationMin{1.0f};
    float PipePair::DestructionDistance{20.0f};
	float PipePair::HeightMin{ 0.0f };
	float PipePair::HeightMax{ 2.0f };
    bool PipePair::RandSeedInit{false};

    void PipePair::InitPipePair()
    {
		if (!RandSeedInit)
		{
			srand(time(0)); // seed the random number generator
			RandSeedInit = true;
		}

        Seperation = SeperationMin + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (SeperationMax - SeperationMin)));
		Height = HeightMin + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HeightMax - HeightMin)));

		m_TransformComponent->GetPosition().y = Height;
		const glm::vec3& position = m_TransformComponent->GetPosition();
        // Set Pipe Positions
        PipeTop->GetTransformComponent()->SetPosition({ position.x, Height + Seperation, position.z });
        PipeBottom->GetTransformComponent()->SetPosition({ position.x, Height - Seperation, position.z });

    }

    void PipePair::GameUpdate(float _deltaTime)
    {
        GameObject::GameUpdate(_deltaTime);

        if (!CanMove) return;

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