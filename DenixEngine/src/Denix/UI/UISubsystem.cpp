#include "UISubsystem.h"

#include "Denix/Core/File/FileSubsystem.h"
#include "Denix/Video/RendererSubsystem.h"
#include "Denix/UI/Widget/Canvas.h"
#include "Widget/Button.h"

namespace Denix
{
	void UISubsystem::Initialize()
	{
		Subsystem::Initialize();
		DE_LOG(LogUI, Warn, "UI Subsystem Initializing")
		FT_Error error;

		error = FT_Init_FreeType(&m_FtLibrary); /* initialize library */
		if (error)
		{
			DE_LOG(LogRender, Error, "Failed to initialize FreeType Library")
		}

		std::string fontFile = FileSubsystem::FormatPath("Content\\Engine\\fonts\\arial.ttf");

		error = FT_New_Face(UISubsystem::m_FtLibrary, fontFile.c_str(), 0, &m_Face); /* create face object */
		if (error == FT_Err_Unknown_File_Format)
		{
			DE_LOG(LogRender, Error, "Font format not supported")
		}
		else if (error)
		{
			DE_LOG(LogRender, Error, "Failed to load font")
		}
		
		DE_LOG(LogUI, Info, "UI Subsystem Initialized")
	}

	void UISubsystem::Deinitialize()
	{
		DE_LOG(LogUI, Trace, "UI Subsystem Deinitializing")

		FT_Done_Face(m_Face);
		FT_Done_FreeType(m_FtLibrary);
		Subsystem::Deinitialize();
		DE_LOG(LogUI, Trace, "UI Subsystem Deinitialized")
	}
	void UISubsystem::Update(float _deltaTime, const Ref<Counter>& _waitCounter)
	{
		Subsystem::Update(_deltaTime, _waitCounter);

		//////////
		for (const auto& canvas : m_Widgets)
        {
			if (!canvas) continue;
			if (!canvas->m_IsActive) continue;

			canvas->Update(_deltaTime, _waitCounter);
        }
	}

	void UISubsystem::RenderUISubmission()
	{
		// @TODO These should be dynamicly adjusted in the future
		m_Projection = glm::ortho(-1.6f, 1.6f, -0.9f, 0.9f, 0.01f, 100.0f);
		m_View = glm::lookAt(glm::vec3(0.0f, 0.0f, 15.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		RendererSubsystem::SubmitCamera({m_Projection, m_View});
		
		for (const auto& widget : m_Widgets)
			for (const auto& child : widget->m_Buttons)
				if (child->m_IsDisplayed) RendererSubsystem::RenderObject({child->m_Material,	child->m_Model.lock(),child->m_ModelMatrix});
					
	}
}
