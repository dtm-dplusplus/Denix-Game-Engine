#pragma once
#include <glm/vec3.hpp>
#include "yaml-cpp/yaml.h"

namespace YAML {
    template<>
    struct convert<glm::vec3> {
        static Node encode(const glm::vec3& rhs) {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            return node;
        }

        static bool decode(const ::YAML::Node& node, glm::vec3& rhs) {
            if(!node.IsSequence() || node.size() != 3) {
                return false;
            }

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            return true;
        }

        
    };
}

static YAML::Node EmitVec3(const glm::vec3& vec) {
    YAML::Node node;
    node.push_back(vec.x);
    node.push_back(vec.y);
    node.push_back(vec.z);
    return node;
}