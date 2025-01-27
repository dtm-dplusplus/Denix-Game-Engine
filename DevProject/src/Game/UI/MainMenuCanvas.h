#pragma once
#include "Game/Canvas.h"

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


