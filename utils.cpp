#include "utils.h"

// DID NOT WRITE THIS CODE had issues with calling it normally
static void sprite_upload_palette(sprite_t *sprite, int palidx)
{
    // Check if the sprite has a palette
    tex_format_t fmt = sprite_get_format(sprite);
    rdpq_tlut_t tlut_mode = rdpq_tlut_from_format(fmt);

    // Configure the TLUT render mode
    rdpq_mode_tlut(tlut_mode);

    if (tlut_mode != TLUT_NONE) {
        // Load the palette (if any). We account for sprites being CI4
        // but without embedded palette: mksprite doesn't create sprites like
        // this today, but it could in the future (eg: sharing a palette across
        // multiple sprites).
        uint16_t *pal = sprite_get_palette(sprite);
        if (pal) rdpq_tex_upload_tlut(pal, palidx*16, fmt == FMT_CI4 ? 16 : 256);
    }
}

// DID NOT WRITE THIS CODE had issues with calling it normally
void utils::rdpq_sprite_blit(sprite_t *sprite, float x0, float y0, const rdpq_blitparms_t *parms)
{
    // Upload the palette and configure the render mode
    sprite_upload_palette(sprite, 0);

    // Get the sprite surface
    surface_t surf = sprite_get_pixels(sprite);
    rdpq_tex_blit(&surf, x0, y0, parms);
}

std::string utils::isolateFileName(const std::string& filePath)
{
    size_t lastSlash = filePath.rfind('/') + 1;
    size_t lastPeriod = filePath.rfind('.');
    return filePath.substr(lastSlash, lastPeriod - lastSlash);
}

void utils::drawControls()
{
    rdpq_font_begin(RGBA32(0x00, 0xff, 0x00, 0xFF));
    rdpq_font_scale(0.8f, 0.8f);
    rdpq_font_position(100.0f, 30.0f);
    rdpq_font_print(global::font, "Controls");
    rdpq_font_end();

    int row, col;
    col = 0;
    row = 1;
    sprite_t* sprite;

    for(int i = 0; i < sizeof(controls) / sizeof(Control); ++i)
    {
        rdpq_set_mode_standard();
        rdpq_mode_filter(FILTER_BILINEAR);
        rdpq_mode_alphacompare(1);
        sprite = textureMan::getSprite(controls[i].control);

        int xPos;
        if(col == 0)
        {
            xPos = (col * display_get_width() / 2) + 26;
        }
        else
        {
            xPos = (col * display_get_width() / 2);
        }

        utils::rdpq_sprite_blit(sprite, xPos, row * 34, NULL);

        rdpq_font_begin(RGBA32(0x00, 0xff, 0x00, 0xFF));
        rdpq_font_scale(0.5f, 0.5f);
        rdpq_font_position(xPos + 35, (row * 34) + 20);
        rdpq_font_printf(global::font, controls[i].message);
        rdpq_font_end();

        if( i % 2 != 0)
        {
            col = 0;
            row++;
        }
        else
        {
            col = 1;
            
        }
    }

    rdpq_font_begin(RGBA32(0x00, 0xff, 0x00, 0xFF));
    rdpq_font_scale(0.5f, 0.5f);
    rdpq_font_position(100.0f, 215.0f);
    rdpq_font_print(global::font, "B to go back");
    rdpq_font_end();
}

void utils::loadControlTextures()
{
    for(auto& texture: controlTextures)
    {
        textureMan::loadTexture(texture);
    }
}
