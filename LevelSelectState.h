#pragma once
#include "MainMenuState.h"

class LevelSelectState : public MainMenuState
{

public:
    LevelSelectState();

    virtual void execute() override;
    virtual void renderRdpq() override;

protected:

};