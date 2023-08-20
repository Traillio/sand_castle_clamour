#include "TextBox.h"

TextBox::TextBox(size_t yPos) : m_xPos(20), m_yPos(yPos)
{
}

void TextBox::draw()
{
        rdpq_set_mode_fill(RGBA32(0x00, 0x00, 0x00, 0xFF));
        rdpq_fill_rectangle(m_xPos, m_yPos, m_xPos + 280, m_yPos + (m_lines.size() * SPACE_BETWEEN_LINES) + SPACE_BETWEEN_LINES * 2);

        rdpq_font_begin(RGBA32(0xff, 0xff, 0xff, 0xFF));
        rdpq_font_scale(m_scale, m_scale);

        for(size_t i = 0; i < m_lines.size(); ++i)
        {
            rdpq_font_position(m_xPos + SPACE_BETWEEN_LINES , m_yPos + (i * SPACE_BETWEEN_LINES) + SPACE_BETWEEN_LINES * 2);
            rdpq_font_print(global::font, m_lines[i].c_str());
        }

        rdpq_font_end();
}

void TextBox::addLine(std::string&& line)
{
    while(line.size() >= 30)
    {
        m_lines.push_back(line.substr(0, 29));
        line = line.substr(29, line.size());
    }

    m_lines.push_back(line);
}
