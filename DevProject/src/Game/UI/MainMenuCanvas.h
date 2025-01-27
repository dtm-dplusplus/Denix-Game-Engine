#pragma once
#include "Denix/UI/Widget/Button.h"
#include "Denix/UI/Widget/Canvas.h"

using namespace Denix;


class PlayButton : public Button
{
    void OnSelect() override;
};

class QuitButton : public Button
{
    void OnSelect() override;
};


class MainMenuCanvas: public Canvas
{
public:
    void BeginScene() override;
};


