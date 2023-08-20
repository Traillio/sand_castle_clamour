#include "SelectionBox.h"

SelectionBox::SelectionBox(size_t xPos = 80, size_t yPos = 120)
    : m_xPos(xPos)
    , m_yPos(yPos)
    , m_currentOption(0)
{
}

int SelectionBox::execute(wav64_t& sfx)
{
    lastInput += global::elapsedSeconds;

    struct controller_data keysPressed = get_keys_pressed();
    struct controller_data keysDown = get_keys_down();

    float y = keysPressed.c[0].y / 128.f;

    if(keysDown.c[0].up || (y > 0.3f && lastInput > INPUT_BUFFER))
    {
        if(m_currentOption == 0)
        {
            m_currentOption = m_options.size() - 1;
        }
        else
        {
            m_currentOption--;
        }

        lastInput = 0.0f;
        wav64_play(&sfx, 0);
    }
    else if( keysDown.c[0].down || (y < -0.3f && lastInput > INPUT_BUFFER))
    {
        m_currentOption = (m_currentOption + 1) % m_options.size();
        lastInput = 0.0f;
        wav64_play(&sfx, 0);
    }
    
    if(keysDown.c[0].A)
    {
        return m_currentOption;
    }

    return -1;
}

void SelectionBox::draw()
{
        
        rdpq_set_mode_fill(RGBA32(0x00, 0x00, 0x00, 0xFF));
        rdpq_fill_rectangle(m_xPos, m_yPos, m_xPos + 160, m_yPos + (m_options.size() * SPACE_BETWEEN_OPTIONS) + SPACE_BETWEEN_OPTIONS);

        rdpq_font_begin(RGBA32(0xff, 0xff, 0xff, 0xFF));
        rdpq_font_scale(0.5f, 0.5f);

        for(size_t i = 0; i < m_options.size(); ++i)
        {
            rdpq_font_position(115, m_yPos + (i * SPACE_BETWEEN_OPTIONS) + SPACE_BETWEEN_OPTIONS);
            rdpq_font_print(global::font, m_options[i].c_str());
        }

        rdpq_font_scale(1.0f, 1.0f);
        rdpq_font_position(85, m_currentOption * SPACE_BETWEEN_OPTIONS + m_yPos + 5 + SPACE_BETWEEN_OPTIONS);
        rdpq_font_print(global::font, ">");

        rdpq_font_end();
}

void SelectionBox::addOption(std::string option)
{
    m_options.push_back(std::move(option));
}

void SelectionBox::clearOptions()
{
    m_options.clear();
    m_currentOption = 0;

    lastInput = 0.0f;
}

void SelectionBox::setCurrentOption(size_t index)
{
    if(index < m_options.size())
    {
        m_currentOption = index;
    }
}

void SelectionBox::setXPos(size_t xPos)
{
    if(xPos > 0 && xPos < 320)
    {
        m_xPos = xPos;
    }
    
}

void SelectionBox::setYPos(size_t yPos)
{
    if(yPos > 0 && yPos < 240)
    {
        m_yPos = yPos;
    }
}
