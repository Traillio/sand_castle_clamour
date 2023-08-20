#include "globals.h"

namespace global
{
    const size_t NUM_LEVELS = 5;
    float elapsedSeconds = 0.0f;
    const int BOARD_DIMENSION = 10;
    size_t levelsUnlocked = 0;
    SaveFile saveFile = {0, {0.0f}, false};
    rdpq_font_t* font = nullptr;
}