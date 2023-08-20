#pragma once
#include <cstddef>
#include <libdragon.h>

struct SaveFile
{
    size_t levelsUnlocked;
    float highestScores[5];
    bool completed;
};

namespace global
{
    extern float elapsedSeconds;
    extern const int BOARD_DIMENSION; 
    extern size_t levelsUnlocked;
    extern const size_t NUM_LEVELS;
    extern SaveFile saveFile;
    extern rdpq_font_t* font;
}  

