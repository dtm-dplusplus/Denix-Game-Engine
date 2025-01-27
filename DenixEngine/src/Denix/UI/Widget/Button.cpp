#include "Button.h"

#include "Denix/Asset/AssetSubsystem.h"
#include "Denix/Audio/AudioSubsystem.h"
#include "Denix/Scene/SceneSubsystem.h"
#include "Denix/UI/UISubsystem.h"

Button::Button(): UIWidget({"Button"})
{
    m_ClassName = "Button";
    m_Model = AssetSubsystem::GetModel("Content\\Engine\\models\\SM_Plane.obj");
   // m_TransformComponent->GetScale().y = 0.01f;

    m_SelectAudioClip = AssetSubsystem::GetAudioClip("Content\\Engine\\audio\\UI_Select.wav");
  //  m_RenderComponent->m_IsUI = true;
    m_Material->m_BaseColor = m_DefaultColor;
   // m_RenderComponent->GetMaterial()->m_BaseColor = Button::m_DefaultColor;
}


void Button::BeginScene()
{
    UIWidget::BeginScene();
}

void Button::EndScene()
{
    m_SelectAudioClip.reset();

    UIWidget::EndScene();
}

void Button::OnSelect()
{
    if (m_SelectAudioClip)
    {
        AudioSubsystem::PlayAudioClipSingle(m_SelectAudioClip);
        DE_LOG(LogUI, Info, "Button Selected: {0}", GetName())
    }
}
