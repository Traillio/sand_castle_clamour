#pragma once
#include "textureMan.h"
#include "modelMan.h"
#include "TexturedModel.h"
#include "Camera.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/gl_integration.h>

class State
{
public:
    State();
    virtual ~State();
    virtual void execute() = 0;
    virtual void renderGl() = 0;
    virtual void renderRdpq() = 0;
    State* nextState = nullptr;
    rdpq_font_t* m_font;

protected:
    Camera camera;

    static const int AUDIO_CHANNEL_SFX = 0;
    static const int AUDIO_CHANNEL_MUSIC = 1;

    wav64_t m_cursorMove;
};