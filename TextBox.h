#pragma once
#include <controller.h>
#include <vector>
#include <string>
#include <rdpq_rect.h>
#include <rdpq_font.h>
#include <rdpq_mode.h>
#include "globals.h"

class TextBox
{
public:
    TextBox(size_t yPos = 20);
    void draw();
    void addLine(std::string&& line);

private:
    static const size_t SPACE_BETWEEN_LINES = 10;

    size_t m_xPos;
    size_t m_yPos;
    const float m_scale = 0.5f;

    std::vector<std::string> m_lines;
};

