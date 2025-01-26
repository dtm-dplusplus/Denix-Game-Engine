#include "Button.h"

#include "Denix/Asset/AssetSubsystem.h"
#include "Denix/Audio/AudioSubsystem.h"
#include "Denix/Scene/SceneSubsystem.h"

Denix::Button::Button()
{
   m_SelectAudioClip = AssetSubsystem::GetAudioClip("Content\\Engine\\audio\\UI_Select.wav");
    m_RenderComponent->m_IsUI = true;
    m_RenderComponent->GetMaterial()->m_BaseColor = Button::defaultColor;
}

Denix::Button::~Button()
{
    m_SelectAudioClip.reset();
}

void Denix::Button::OnSelect()
{
    if (m_SelectAudioClip)
    {
        AudioSubsystem::PlayAudioClipSingle(m_SelectAudioClip);
        DE_LOG(LogUI, Info, "Button Selected: {0}", GetName())
    }

    //SceneSubsystem::RequestOpenScene(MakeRef<Asset>(AssetInit("Content\\Scene\\AudioScene.asset")));
}
