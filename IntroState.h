#pragma once
#include "MainMenuState.h"
#include "globals.h"

class IntroState : public State
{
public:
    IntroState();
    ~IntroState();
    virtual void execute() override;
    virtual void renderGl() override;
    virtual void renderRdpq() override;

private:
    float m_timeElapsed = 0;

    TexturedModel m_logoModel;
};

