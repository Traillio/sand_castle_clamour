#include "State.h"



State::State()
{
    textureMan::clearTextures();
    modelMan::clearModels();

    wav64_open(&m_cursorMove, "rom:/cursor_move.wav64");
    mixer_ch_set_vol(AUDIO_CHANNEL_SFX, 0.50f, 0.50f);
}

State::~State()
{

}