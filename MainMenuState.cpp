#include "MainMenuState.h"
#include "LevelSelectState.h"
#include "InstructionState.h"

MainMenuState::MainMenuState()
{
    camera.resetCamera();
    camera.Transform();
    glClearColor(0, 0, 0, 1);
    textureMan::clearTextures();
    modelMan::clearModels();
    textureMan::loadTexture("rom:/skybox.sprite");
    modelMan::loadModel("rom:/skybox.model64");

    skybox.setModel("skybox");
    skybox.setTexture("skybox");
    skybox.setScale({100.0f, 100.0f, 100.0f});

    m_selectionBox.addOption("Start Game");
    m_selectionBox.addOption("Controls");
    m_selectionBox.addOption("Level Select");
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

    switch(m_selectionBox.execute(m_cursorMove))
    {
        case START_GAME:
            nextState = new LevelState(1);
            break;
        case LEVEL_SELECT:
            nextState = new LevelSelectState();
            break;
        case INSTRUCTIONS:
            nextState = new InstructionState();
            break;
    }
}

void MainMenuState::renderGl()
{
    skybox.draw();
}

void MainMenuState::renderRdpq()
{
    rdpq_font_begin(RGBA32(0xff, 0xff, 0xff, 0xFF));

    rdpq_font_scale(1.0f, 1.0f);
    rdpq_font_position(60, 25);
    rdpq_font_print(m_font, "Sand Castle");

    rdpq_font_position(90, 50);
    rdpq_font_print(m_font, "Clamour!");

    m_selectionBox.draw(m_font);
}
