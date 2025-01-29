#pragma once

#include <glm/fwd.hpp>
#include <glm/vec3.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace Denix
{
    struct Transform
    {
        Transform(): Position(glm::vec3(0.0f)), Rotation(glm::vec3(0.0f)), Scale(glm::vec3(1.0f))
        {
        }

        Transform(const glm::vec3& _position, const glm::vec3& _rotation, const glm::vec3& _scale)
            : Position(_position), Rotation(_rotation), Scale(_scale)
        {
        }

        glm::vec3 Position;
        glm::vec3 Rotation;
        glm::vec3 Scale;
    };

    inline void CalculateModel(const Transform& m_Transform, glm::mat4& m_Model)
    {
        // Position
        m_Model = translate(glm::mat4(1.0f), m_Transform.Position);

        // Rotation
        m_Model = rotate(m_Model, glm::radians(m_Transform.Rotation.x), glm::vec3(1, 0, 0));
        m_Model = rotate(m_Model, glm::radians(m_Transform.Rotation.y), glm::vec3(0, 1, 0));
        m_Model = rotate(m_Model, glm::radians(m_Transform.Rotation.z), glm::vec3(0, 0, 1));

        // Scale - half to account for +ve & -ve scale
        m_Model = scale(m_Model, m_Transform.Scale / 2.0f);
    }
}
