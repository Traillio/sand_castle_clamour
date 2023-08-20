#pragma once
#include "MainMenuState.h"
#include "TextBox.h"


class StartGameState : public MainMenuState
{
    
public:
    StartGameState();
    virtual void execute() override;
    virtual void renderRdpq() override;
private:
    TextBox m_textBox;

};