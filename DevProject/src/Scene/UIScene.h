#pragma once
#include "Denix/Scene/Scene.h"

inline static int CharSize = 25;
inline static glm::vec2 Position = {0.0f, 240.0f};
inline static std::string Text = "Hello World";
inline unsigned int textureID;

namespace Denix
{
    class UIScene: public Scene
    {
    public:
        UIScene() = default;

        void BeginScene() override;
        void Update(float _deltaTime) override;
        
    };
}
