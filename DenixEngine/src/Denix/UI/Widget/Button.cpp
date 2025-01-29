#include "Button.h"

#include "Denix/Asset/AssetSubsystem.h"
#include "Denix/Audio/AudioSubsystem.h"
#include "Denix/Scene/SceneSubsystem.h"
#include "Denix/UI/UISubsystem.h"

using namespace Denix;

Button::Button(): UIWidget({"Button"})
{
    m_ClassName = "Button";
    m_Model = AssetSubsystem::GetModel("Content\\Engine\\models\\SM_Plane.obj");
    m_HoverAudioClip = AssetSubsystem::GetAudioClip("Content\\Engine\\audio\\UI_Navigate.wav");
    m_SelectAudioClip = AssetSubsystem::GetAudioClip("Content\\Engine\\audio\\UI_Select.wav");
    m_Material->m_BaseColor = m_DefaultColor;
    m_Material->m_MultiplyBase = true;
    m_Transform.Rotation.x = 90.0f;
    m_Transform.Scale = {0.7f, 1.0f, 0.3f};
}

Button::Button(const ObjectInit& _objInit): UIWidget(_objInit)
{
    m_ClassName = "Button";
    m_Model = AssetSubsystem::GetModel("Content\\Engine\\models\\SM_Plane.obj");
    m_HoverAudioClip = AssetSubsystem::GetAudioClip("Content\\Engine\\audio\\UI_Navigate.wav");
    m_SelectAudioClip = AssetSubsystem::GetAudioClip("Content\\Engine\\audio\\UI_Select.wav");
    m_Material->m_BaseColor = m_DefaultColor;
    m_Transform.Rotation.x = 90.0f;
    m_Transform.Scale = {0.7f, 1.0f, 0.3f};
}

void Button::OnSelect()
{
    AudioSubsystem::PlayAudioClipSingle(m_SelectAudioClip.lock());
}


void Button::OnHoverBegin()
{
    m_Material->m_BaseColor = m_SelectedColor;
    m_Material->m_MultiplyBase = true;
    AudioSubsystem::PlayAudioClipSingle(m_HoverAudioClip.lock());
}

void Button::OnHoverEnd()
{
    m_Material->m_BaseColor = m_DefaultColor;
    m_Material->m_MultiplyBase = false;
}
