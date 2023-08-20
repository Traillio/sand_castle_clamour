#include "IntroState.h"

IntroState::IntroState() :
m_logoModel(TexturedModel(modelMan::loadModel("rom:/garbled.model64"), "garbled"))
{
    textureMan::clearTextures();
    textureMan::loadTexture("rom:/garbled.sprite");

    camera.Zoom(-12.0f);
    m_logoModel.moveY(5.0f);
}

IntroState::~IntroState()
{
}

void IntroState::execute()
{
    camera.Transform();

    m_logoModel.rotateY(120.0f * global::elapsedSeconds);

    if(m_timeElapsed >= 3.0f)
    {
        nextState = new MainMenuState();
    }
    
    m_timeElapsed += global::elapsedSeconds;
}


void IntroState::renderGl()
{
    m_logoModel.draw();
}


void IntroState::renderRdpq()
{
    rdpq_font_begin(RGBA32(0x38, 0x38, 0x7F, 0xFF));
    rdpq_font_scale(0.5f, 0.5f);
    rdpq_font_position(80, 200);
    rdpq_font_print(global::font, "Created by: Traill");
    rdpq_font_end();
}