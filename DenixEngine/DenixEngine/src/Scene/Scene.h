#pragma once

#include <string>

// Super Basic Scene class so I can start creating and testing features
// Will be intergrated into ECS later on
class Scene
{
public:
	Scene() =  default;
	Scene(std::string _name) : SceneName(std::move(_name)) {}
	virtual ~Scene() = default;

	virtual void Update() {}
	virtual void Draw() {}

protected:
	std::string SceneName;
};



