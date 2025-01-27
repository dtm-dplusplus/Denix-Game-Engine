#include "Button.h"

#include "Denix/Asset/AssetSubsystem.h"
#include "Denix/Audio/AudioSubsystem.h"
#include "Denix/Scene/SceneSubsystem.h"
#include "Denix/UI/UISubsystem.h"

Button::Button()
{
    m_SelectAudioClip = AssetSubsystem::GetAudioClip("Content\\Engine\\audio\\UI_Select.wav");
    m_RenderComponent->m_IsUI = true;
    m_RenderComponent->GetMaterial()->m_BaseColor = Button::defaultColor;
}

Button::~Button() = default;


void Button::BeginScene()
{
    Plane::BeginScene();

    UISubsystem::GetInstance()->m_UIWidgets.push_back(GetRef<Button>());
}

void Button::EndScene()
{
    std::erase(UISubsystem::GetInstance()->m_UIWidgets, GetRef<Button>());
    m_SelectAudioClip.reset();

    Plane::EndScene();
}

void Button::OnSelect()
{
    if (m_SelectAudioClip)
    {
        AudioSubsystem::PlayAudioClipSingle(m_SelectAudioClip);
        DE_LOG(LogUI, Info, "Button Selected: {0}", GetName())
    }
}
