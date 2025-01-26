#include "GEPActor.h"

#include "Denix/Asset/AssetSubsystem.h"

GEPActor::GEPActor()
{
}

void GEPActor::OnCollisionEnter(const Denix::Ref<Actor>& _other, const glm::vec3& _normal, const glm::vec3& _point)
{
    Cube::OnCollisionEnter(_other, _normal, _point);

    DE_LOG(LogDevProject, Trace, "Collision Enter {}", _other->GetName());
}

void GEPActor::OnCollisionExit(const Denix::Ref<Actor>& _other, const glm::vec3& _normal, const glm::vec3& _point)
{
    Cube::OnCollisionExit(_other, _normal, _point);

    DE_LOG(LogDevProject, Trace, "Collision Exit {}", _other->GetName());
}
