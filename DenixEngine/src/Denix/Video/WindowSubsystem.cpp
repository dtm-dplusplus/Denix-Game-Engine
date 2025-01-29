#include "WindowSubsystem.h"

#include "Denix/Editor/EditorSubsystem.h"
#include "Denix/Profile/ProfileSubsystem.h"
#include "Denix/Scene/SceneSubsystem.h"
#include "Denix/UI/UISubsystem.h"

namespace Denix
{
    void WindowSubsystem::ToggleFullscreen()
    {
        s_Instance->m_Window->ToggleFullscreen();
    }

    void WindowSubsystem::NewFrame()
    {
        DE_PROFILE(Clear Frame Buffer)
        EditorSubsystem::NewFrame();
        m_Window->ClearBuffer();
        // @TODO: Move Viewport Ownership to window
        if (Ref<CameraComponent> camera = SceneSubsystem::GetActiveScene()->GetActiveCamera()->GetComponent<
            CameraComponent>())
            camera->GetViewport()->m_FrameBuffer->Bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        DE_PROFILE_END(Clear Frame Buffer)
    }

    void WindowSubsystem::PresentFrame()
    {
        DE_PROFILE(Draw Viewport)
        FrameBuffer::Unbind();
        // @TODO: Move Viewport Ownership to window
        if (Ref<CameraComponent> camera = SceneSubsystem::GetActiveScene()->GetActiveCamera()->GetComponent<
            CameraComponent>())
            camera->GetViewport()->DrawViewport();
        EditorSubsystem::RenderUI(); // Swap buffers and render UI
        m_Window->SwapBuffers();
        EditorSubsystem::PresentFrame();
        DE_PROFILE_END(Draw Viewport)
    }

    void WindowSubsystem::Initialize()
    {
        Subsystem::Initialize();
        DE_LOG(LogWindow, Warn, "Initializing Window Subsystem")

        SDL_GLWindow::m_GLMajorVersion = 3;
        SDL_GLWindow::m_GLMinorVersion = 0;
        SDL_GLWindow::m_GLDepthSize = 24;
        SDL_GLWindow::m_GLStencilSize = 8;
        SDL_GLWindow::m_GLDoubleBuffer = 1;


        //Create main window
        m_Window = MakeRef<SDL_GLWindow>();
        if (!m_Window->m_IsOpen)
        {
            const std::string error = "Failed to create main window";
            DE_LOG(LogWindow, Critical, error)
            throw std::runtime_error(error.c_str());
        }

        // Init Glew
        if (glewInit() != GLEW_OK)
        {
            DE_LOG(LogWindow, Critical, "glewInit(): failed")
            return;
        }
        DE_LOG(LogWindow, Trace, "glewInit(): success")

        DE_LOG(LogWindow, Info, "Window Subsystem Initialized")
    }

    void WindowSubsystem::Deinitialize()
    {
        DE_LOG(LogWindow, Trace, "Window Subsystem Deinitializing")
        m_Window.reset();
        Subsystem::Deinitialize();
        DE_LOG(LogWindow, Trace, "Window Subsystem Deinitialized")
    }
}
