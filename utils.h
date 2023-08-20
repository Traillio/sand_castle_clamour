#pragma once
#include <string>
#include <GL/gl_integration.h>
#include <libdragon.h>
#include "textureMan.h"
#include "globals.h"

namespace utils
{
    void rdpq_sprite_blit(sprite_t *sprite, float x0, float y0, const rdpq_blitparms_t *parms);

    struct Control
    {
        const char* control;
        const char* message;
    };

    std::string isolateFileName(const std::string& filePath);

    namespace
    {
        const char* controlTextures[] = 
        {
            "rom:/c_up.sprite",
            "rom:/c_down.sprite",
            "rom:/c_left.sprite",
            "rom:/c_right.sprite",
            "rom:/stick.sprite",
            "rom:/dpad.sprite",
            "rom:/a.sprite",
            "rom:/b.sprite",
            "rom:/start.sprite",
            "rom:/z.sprite"
        };

        Control controls[] = 
        {
            {"c_up", "Cam Up"},
            {"c_down", "Cam Down"},
            {"c_left", "Rot. Left"},
            {"c_right", "Rot. Right"},
            {"stick", "Move Cursor"},
            {"dpad", "Move Cursor"},
            {"a", "add block"},
            {"b", "delete block"},
            {"start", "Pause"},
            {"z", "Overhead Cam"}
        };
    }

    void drawControls();
    void loadControlTextures();
}