#pragma once
#include "MainMenuState.h"


class InstructionState : public State
{
public:
    InstructionState();
    ~InstructionState();

    virtual void execute() override;
    virtual void renderGl() override;
    virtual void renderRdpq() override; 

protected:
    sprite_t* sprite;
    TexturedModel skybox;
};