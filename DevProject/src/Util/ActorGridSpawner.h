#pragma once
#include "Denix/Core.h"
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
        _sceneObjects.clear();
        _sceneObjects.shrink_to_fit();
        
        // Spawn cube grid
        for (int i = 0; i < GridSize; i++)
        {
            for (int j = 0; j < GridSize; j++)
            {
                _scene->SpawnGameObject<Cube>(glm::vec3(i * 2.5f, j * 2.5f, 0.0f));
            }
        }
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
                _scene->SpawnGameObject<Cube>(glm::vec3(i * 2.5f, j * 2.5f, 0.0f));
            }
        }
    }
    
    int GridSize = 10;
};
