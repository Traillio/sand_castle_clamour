#pragma once
#include "LevelState.h"
#include "globals.h"
#include "SelectionBox.h"

class MainMenuState : public State
{
    enum Selection{START_GAME, INSTRUCTIONS, LEVEL_SELECT, HIGH_SCORES};

    public:
    MainMenuState();

    protected:

    virtual void execute() override;
    virtual void renderGl() override;
    virtual void renderRdpq() override;

    TexturedModel skybox;
    SelectionBox m_selectionBox;

    bool viewingControls = false;
};