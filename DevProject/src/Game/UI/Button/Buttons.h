#pragma once
#include "Denix/UI/Widget/Button.h"

class PlayButton : public Denix::Button
{
public:
    PlayButton();
    void OnSelect() override;
};

class RestartButton : public Denix::Button
{
public:
    RestartButton();
    void OnSelect() override;
};

class MainMenuButton : public Denix::Button
{
public:
    MainMenuButton();
    void OnSelect() override;
};

class QuitButton : public Denix::Button
{
public:
    QuitButton();
    
    void OnSelect() override;
};