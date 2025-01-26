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

inline void Vec3ToYAML(YAML::Emitter& _out, const glm::vec3& rhs)
{
    _out << YAML::Key << "x" << YAML::Value << rhs.x;
    _out << YAML::Key << "y" << YAML::Value << rhs.y;
    _out << YAML::Key << "z" << YAML::Value << rhs.z;
}

inline glm::vec3 YAMLtoVec3(const YAML::Node& node)
{
   //if(!node.IsSequence() || node.size() != 3) {
   //    return {0.0f, 0.0f, 0.0f};
   //}
    if (!node["x"].IsDefined() || !node["y"].IsDefined() || !node["z"].IsDefined()) {
        return {0.0f, 0.0f, 0.0f};
    }
    glm::vec3 vec;
    vec.x = node["x"].as<float>();
    vec.y = node["y"].as<float>();
    vec.z = node["z"].as<float>();
    return vec;
}

static YAML::Node EmitVec3(const glm::vec3& vec) {
    YAML::Node node;
    node["x"] = vec.x;
    node["y"] = vec.y;
    node["z"] = vec.z;
    /*node.push_back(vec.x);
    node.push_back(vec.y);
    node.push_back(vec.z);*/
    return node;
}
