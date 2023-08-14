#include "State.h"



State::State()
{
    textureMan::clearTextures();
    modelMan::clearModels();
    m_font = rdpq_font_load("rom:/PressStart2P-Regular.font64");

    wav64_open(&m_cursorMove, "rom:/cursor_move.wav64");
    mixer_ch_set_vol(AUDIO_CHANNEL_SFX, 0.50f, 0.50f);
}

State::~State()
{
    rdpq_font_free(m_font);
}