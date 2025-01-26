#include "AudioComponent.h"

void Denix::AudioComponent::Serialize(YAML::Emitter& _out)
{
    Component::Serialize(_out);
}

void Denix::AudioComponent::Deserialize(const YAML::Node& _in)
{
    Component::Deserialize(_in);
}
