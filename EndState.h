#pragma once
#include "MainMenuState.h"
#include "TextBox.h"
#include "utils.h"

class EndState : public MainMenuState
{

public:
    EndState();
    
    virtual void execute() override;
    virtual void renderRdpq() override;
private:
    TextBox m_textBox;
    float m_score = 0.0f;
    sprite_t* m_cupSprite;
    wav64_t m_winSound;
    bool m_playedSound = false;
};