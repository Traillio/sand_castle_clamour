#include "IntroState.h"

IntroState::IntroState() :
m_logoModel(TexturedModel(modelMan::loadModel("rom:/n64_textured.model64"), "n64_c"))
{
    textureMan::clearTextures();
    textureMan::loadTexture("rom:/n64_c.sprite");

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

    if(m_timeElapsed >= 1.0f)
    {
        nextState = new MainMenuState();
    }
    
    m_timeElapsed += global::elapsedSeconds;
}


void IntroState::renderGl()
{
    m_logoModel.draw();


    // glEnable(GL_BLEND);
    // glEnable(GL_COLOR_MATERIAL);
    // glColor4b(255, 0, 0, 25);
    // for(auto& block: emptyBlocks)
    // {
    //     block.draw();
    // }
    // glColor4b(0, 0, 0, 255);
    // glDisable(GL_COLOR_MATERIAL);
    // glDisable(GL_BLEND);

    
    
}


void IntroState::renderRdpq()
{
    rdpq_font_begin(RGBA32(0x38, 0x38, 0x7F, 0xFF));
    rdpq_font_scale(0.5f, 0.5f);
    rdpq_font_position(80, 200);
    rdpq_font_print(m_font, "Created by: Traill");
    rdpq_font_end();
}