#pragma once
#include "Denix/Core.h"
#include "Denix/Core/Timer.h"
#include "Denix/Scene/Scene.h"

using namespace  Denix;

struct ActorGridSpawner
{
public:
    void SpawnGrid(const Ref<Scene>& _scene)
    {
        if (!_scene) return;

        // Adjust vector
        std::vector<Ref<Actor>>& _sceneObjects = _scene->GetSceneObjects();
       for (const auto& actor : _sceneObjects)  actor->Destroy();
        
        Timer spawnTime;
        spawnTime.Start();
        // Spawn cube grid
        for (int i = 0; i < GridSize; i++)
        {
            for (int j = 0; j < GridSize; j++)
            {
                DE_PROFILE(SpawnActor);
                _scene->SpawnActor<Cube>(glm::vec3(i * 2.5f, j * 2.5f, 0.0f));
                DE_PROFILE_END(SpawnActor);
            }
        }
        spawnTime.Stop();
        DE_LOG(Log, Warn, "Spawned {} Actors to Grid in: {} ms", GridSize * GridSize, spawnTime.GetDuration() * 1000.0f);
    }

    void SpawnGridDef(const Ref<Scene>& _scene, const int _count)
    {
        if (!_scene) return;

        // Adjust vector
        std::vector<Ref<Actor>>& _sceneObjects = _scene->GetSceneObjects();
        _sceneObjects.clear();
        
        // Spawn cube grid
        for (int i = 0; i < _count; i++)
        {
            for (int j = 0; j < _count; j++)
            {
                _scene->SpawnActor<Cube>(glm::vec3(i * 2.5f, j * 2.5f, 0.0f));
            }
        }
    }
    
    int GridSize = 10;
};
