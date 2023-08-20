#include "LevelSelectState.h"


LevelSelectState::LevelSelectState()
{
    camera.resetCamera();
    m_selectionBox.setYPos(60);

    m_selectionBox.clearOptions();

    m_selectionBox.addOption("Back");

    for(size_t i = 0; i < global::saveFile.levelsUnlocked; ++i)
    {
        m_selectionBox.addOption("Level " + std::to_string(i + 1));
    }
}

void LevelSelectState::execute()
{
    if (audio_can_write())
    {
        short *buf = audio_write_begin();
        mixer_poll(buf, audio_get_buffer_length());
        audio_write_end();
    }

    controller_scan();
    int selection = m_selectionBox.execute(m_cursorMove);

    struct controller_data down = get_keys_down();

    if(selection == 0 || down.c[0].B) 
    {
        nextState = new MainMenuState();
    }
    else if(selection > 0)
    {
        nextState = new LevelState(selection - 1);
    }
}

void LevelSelectState::renderRdpq()
{
    rdpq_font_begin(RGBA32(0xff, 0xff, 0xff, 0xFF));

    rdpq_font_scale(1.0f, 1.0f);
    rdpq_font_position(50.0f, 45.0f);
    rdpq_font_print(global::font, "Level Select");
    rdpq_font_end();

    m_selectionBox.draw();
}
