#include "LevelSelectState.h"


LevelSelectState::LevelSelectState()
{
    camera.resetCamera();
    m_selectionBox.setYPos(40);

    m_selectionBox.clearOptions();

    m_selectionBox.addOption("Back");

    for(size_t i = 1; i <= global::levelsUnlocked; ++i)
    {
        m_selectionBox.addOption("Level " + std::to_string(i));
    }
}

void LevelSelectState::execute()
{
    controller_scan();

    int selection = m_selectionBox.execute(m_cursorMove);

    if(selection == 0)
    {
        nextState = new MainMenuState();
    }
    else if(selection > 0)
    {
        nextState = new LevelState(selection);
    }
}

void LevelSelectState::renderRdpq()
{
    rdpq_font_begin(RGBA32(0xff, 0xff, 0xff, 0xFF));

    rdpq_font_scale(1.0f, 1.0f);
    rdpq_font_position(50, 25);
    rdpq_font_print(m_font, "Level Select");
    rdpq_font_end();

    m_selectionBox.draw(m_font);
}
