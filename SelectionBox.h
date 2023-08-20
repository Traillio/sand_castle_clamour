#pragma once
#include <controller.h>
#include <vector>
#include <string>
#include <rdpq_rect.h>
#include <rdpq_font.h>
#include <rdpq_mode.h>
#include "globals.h"
#include "wav64.h"

class SelectionBox
{
public:
    SelectionBox(size_t xPos = 80, size_t yPos = 120);

private:
    static const size_t SPACE_BETWEEN_OPTIONS = 25;

    size_t m_xPos;
    size_t m_yPos;
    size_t m_currentOption;

    static constexpr float INPUT_BUFFER = 0.2f;

    float lastInput = 0.0f;

    std::vector<std::string> m_options;

public:
    int execute(wav64_t& sfx);
    void draw();
    void addOption(std::string option);
    void clearOptions();
    void setCurrentOption(size_t index);
    void setXPos(size_t xPos);
    void setYPos(size_t yPos);
};