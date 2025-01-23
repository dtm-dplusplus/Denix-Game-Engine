#pragma once
#include "Denix/Scene/Scene.h"

inline static int CharSize = 25;
inline static glm::vec2 Position = {50.0f, 100.0f};
inline static glm::vec3 Color = {1.0f, 1.0f, 1.0f};
inline static std::vector<std::string> Text = { "Hello ", "Hello 2" };
inline static const int WIDTH = 400;
inline static const int HEIGHT = 200;
inline static GLuint textureID[2] = { 0, 0 };
namespace Denix
{
    class UIScene: public Scene
    {
    public:
        UIScene() = default;

        void BeginScene() override;
        void Update(float _deltaTime) override;

        unsigned int CreateTextBox(std::string _text, glm::vec2 _position, glm::vec2 _size, glm::vec3 _color);
    };
}
