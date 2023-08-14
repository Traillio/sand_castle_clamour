#include "InstructionState.h"
InstructionState::InstructionState()
{
    glClearColor(1, 1, 1, 1);
    textureMan::clearTextures();
    modelMan::clearModels();

    textureMan::loadTexture("rom:/c_up.sprite");
    textureMan::loadTexture("rom:/c_left.sprite");
    textureMan::loadTexture("rom:/c_right.sprite");
    textureMan::loadTexture("rom:/c_down.sprite");
    textureMan::loadTexture("rom:/a.sprite");
    textureMan::loadTexture("rom:/b.sprite");
    textureMan::loadTexture("rom:/start.sprite");
    textureMan::loadTexture("rom:/stick.sprite");
    textureMan::loadTexture("rom:/dpad.sprite");
    textureMan::loadTexture("rom:/skybox.sprite");
    textureMan::loadTexture("rom:/z.sprite");

    modelMan::loadModel("rom:/skybox.model64");

    skybox.setModel("skybox");
    skybox.setTexture("skybox");
    skybox.setScale({100.0f, 100.0f, 100.0f});
}

InstructionState::~InstructionState()
{
}

void InstructionState::execute()
{

}

void InstructionState::renderGl()
{
    
    skybox.draw(); 

}


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
void rdpq_sprite_blit(sprite_t *sprite, float x0, float y0, const rdpq_blitparms_t *parms)
{
    // Upload the palette and configure the render mode
    sprite_upload_palette(sprite, 0);

    // Get the sprite surface
    surface_t surf = sprite_get_pixels(sprite);
    rdpq_tex_blit(&surf, x0, y0, parms);
}

void InstructionState::renderRdpq()
{
    //avert ye eyes mateys

    rdpq_set_mode_standard();
    rdpq_mode_filter(FILTER_BILINEAR);
    rdpq_mode_alphacompare(1);  
    rdpq_blitparms_t params = {TILE0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    params.cx = 16;
    params.cy = 32;
    
    int baseX = 320/10;
    int baseY = 240/6 - 7;
    int xOffset = 10;

    sprite = textureMan::getSprite("c_up");
    
    rdpq_sprite_blit(sprite, baseX, baseY, &params);

    sprite = textureMan::getSprite("c_down");
    rdpq_sprite_blit(sprite, baseX * 2 + xOffset, baseY, &params);

    sprite = textureMan::getSprite("c_left");
    rdpq_sprite_blit(sprite, baseX, baseY * 2, &params);

    sprite = textureMan::getSprite("c_right");
    rdpq_sprite_blit(sprite, baseX * 2 +xOffset, baseY * 2, &params);

    sprite = textureMan::getSprite("a");
    rdpq_sprite_blit(sprite, baseX + 22, baseY * 3, &params);

    sprite = textureMan::getSprite("b");
    rdpq_sprite_blit(sprite, baseX + 22, baseY * 4, &params);

    sprite = textureMan::getSprite("start");
    rdpq_sprite_blit(sprite, baseX + 22, baseY * 5, &params);

    sprite = textureMan::getSprite("dpad");
    rdpq_sprite_blit(sprite, baseX, baseY * 6, &params);

    sprite = textureMan::getSprite("stick");
    rdpq_sprite_blit(sprite, baseX * 2 + xOffset, baseY * 6, &params);

    sprite = textureMan::getSprite("z");
    rdpq_sprite_blit(sprite, baseX + 22, baseY * 7, &params);

    sprite = nullptr;

    int fontX = 100;
    int fontY = 22;
    int fontyOffset = 33;

    rdpq_font_begin(RGBA16(0, 0, 0, 255));
    rdpq_font_scale(0.5f, 0.5f);

    rdpq_font_position(fontX, fontY);
    rdpq_font_printf(m_font, "Move the Camera up/down");

    rdpq_font_position(fontX, fontY + fontyOffset);
    rdpq_font_printf(m_font, "  Rotate the camera");

    rdpq_font_position(fontX, fontY + fontyOffset * 2);
    rdpq_font_printf(m_font, "    Place a block");

    rdpq_font_position(fontX, fontY + fontyOffset * 3);
    rdpq_font_printf(m_font, "    Delete a block");

    rdpq_font_position(fontX, fontY + fontyOffset * 4);
    rdpq_font_printf(m_font, "    Pause the game");

    rdpq_font_position(fontX, fontY + fontyOffset * 5);
    rdpq_font_printf(m_font, "    Move the cursor");

    rdpq_font_position(fontX, fontY + fontyOffset * 6);
    rdpq_font_printf(m_font, " Toggle overhead view");

    rdpq_font_end();


    controller_scan();
    struct controller_data down = get_keys_down();

    if(down.c[0].B)
    {
        debugf("\nB pressed\n");
        nextState = new MainMenuState();
    }
}