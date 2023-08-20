#include "EndState.h"
#include "LevelSelectState.h"
#include "StartGameState.h"

MainMenuState::MainMenuState() : m_selectionBox(80, 90)
{
    mixer_ch_stop(AUDIO_CHANNEL_MUSIC);
    mixer_ch_stop(AUDIO_CHANNEL_SFX);

    camera.resetCamera();
    camera.Transform();
    glClearColor(0, 0, 0, 1);
    textureMan::clearTextures();
    modelMan::clearModels();
    textureMan::loadTexture("rom:/skybox.sprite");
    modelMan::loadModel("rom:/skybox.model64");

    utils::loadControlTextures();

    skybox.setModel("skybox");
    skybox.setTexture("skybox");
    skybox.setScale({100.0f, 100.0f, 100.0f});

    m_selectionBox.addOption("Start Game");
    m_selectionBox.addOption("Controls");
    m_selectionBox.addOption("Level Select");

    if(global::saveFile.completed)
    {
        m_selectionBox.addOption("High Scores");
    }
}

void MainMenuState::execute()
{
    if (audio_can_write())
    {
        short *buf = audio_write_begin();
        mixer_poll(buf, audio_get_buffer_length());
        audio_write_end();
    }

    controller_scan();

    if(viewingControls)
    {
        struct controller_data down = get_keys_down();

        if(down.c[0].B)
        {
            viewingControls = false;
        }

        return;
    }

    switch(m_selectionBox.execute(m_cursorMove))
    {
        case START_GAME:
            nextState = new StartGameState();
            break;
        case LEVEL_SELECT:
            nextState = new LevelSelectState();
            break;
        case INSTRUCTIONS:
            viewingControls = true;
            break;
        case HIGH_SCORES:
            nextState = new EndState();
            break;
    }
}

void MainMenuState::renderGl()
{
    skybox.draw();
}

void MainMenuState::renderRdpq()
{
    if(viewingControls)
    {
        utils::drawControls();
        return;
    }

    rdpq_font_begin(RGBA32(0xff, 0xff, 0xff, 0xFF));

    rdpq_font_scale(1.0f, 1.0f);
    rdpq_font_position(60.0f, 45.0f);
    rdpq_font_print(global::font, "Sand Castle");

    rdpq_font_position(90.0f, 70.0f);
    rdpq_font_print(global::font, "Clamour!");

    m_selectionBox.draw();
}
