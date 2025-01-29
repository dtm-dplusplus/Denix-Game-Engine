#pragma once

#include <ft2build.h>
#include <glm/mat4x4.hpp>


#include FT_FREETYPE_H

#include "Denix/Core/Subsystem.h"

#include "UIWidget.h"


namespace Denix
{
    class Canvas;
    class Button;

    class UISubsystem : public Subsystem<UISubsystem>
    {
    public:
        UISubsystem() = default;
        ~UISubsystem() override = default;

        UISubsystem(const UISubsystem& _other) = delete;
        UISubsystem(UISubsystem&& _other) noexcept = delete;
        UISubsystem& operator=(const UISubsystem& _other) = delete;
        UISubsystem& operator=(UISubsystem&& _other) noexcept = delete;

        inline static FT_Library m_FtLibrary;
        inline static FT_Face m_Face;

        std::vector<Ref<Canvas>> m_Widgets;
        glm::mat4 m_Projection;
        glm::mat4 m_View;

    private:
        void RenderUISubmission();

        void Initialize() override;

        void Deinitialize() override;

        void Update(float _deltaTime, const Ref<Counter>& _waitCounter) override;


        friend class Engine;
        friend class EditorSubsystem;
        friend class WindowSubsystem;
    };
}
