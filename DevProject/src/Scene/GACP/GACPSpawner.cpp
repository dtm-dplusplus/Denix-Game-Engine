#include "GACPSpawner.h"
#include "Denix/Core/Timer.h"
#include "Denix/Scene/Scene.h"
#include "Scene/GACP/GACPActor.h"

void Denix::GACPSpawner::SpawnGrid(const Ref<Scene>& _scene) const
{
    if (!_scene) return;

    // Destroy all actors
    for (const std::vector<Ref<Actor>>& _sceneObjects = _scene->GetSceneActors(); 
        const auto& actor : _sceneObjects) actor->Destroy();
        
    Timer spawnTime;
    spawnTime.Start();
    
    // Spawn Actor Grid
    for (int i = 0; i < GridSize; i++)
        for (int j = 0; j < GridSize; j++)
            _scene->SpawnActor<GACPActor>(glm::vec3((float)i * 2.5f, SpawnHeight, (float)j * 2.5f));

    spawnTime.Stop();
    DE_LOG(Log, Warn, "Spawned {} Actors to Grid in: {} ms", GridSize * GridSize, spawnTime.GetDurationMs())
}
