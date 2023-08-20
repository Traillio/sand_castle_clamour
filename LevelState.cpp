#include "EndState.h"
#include "LevelState.h"

LevelState::LevelState(const size_t level) : m_selectionBox(80, 60), m_currentLevel(level), m_levelLoader(*this)
{
    camera.resetCamera();
    glClearColor(0, 0, 1, 1);
    textureMan::clearTextures();
    modelMan::clearModels();

    char str[20];
    sprintf(str, "rom:/level_%d.txt", m_currentLevel);
    m_levelLoader.loadLevel(str);

    m_timer = m_amountOfTimeToObserve;
    cursor.getModel().setModel("cube");

    water.setTexture("water");
    water.setModel("water");
    water.setScale({106.0f, 106.0f, 106.0f});
    water.moveY(-5.8f);
    water.moveZ(-60.0f);

    //wav64_open(&m_ambience, "rom:/ambience.wav64");
    //wav64_set_loop(&m_ambience, true);
    //wav64_play(&m_ambience, AUDIO_CHANNEL_MUSIC);
    //mixer_ch_set_vol(AUDIO_CHANNEL_MUSIC, 0.1f, 0.1f);
}

LevelState::~LevelState()
{
    mixer_ch_stop(AUDIO_CHANNEL_MUSIC);
    mixer_ch_stop(AUDIO_CHANNEL_SFX);
}

void LevelState::execute()
{
    if (audio_can_write())
    {
        short *buf = audio_write_begin();
        mixer_poll(buf, audio_get_buffer_length());
        audio_write_end();
    }

    controller_scan();
    
    m_buttonsPressed = get_keys_pressed();
    m_buttonsDown = get_keys_down();

    if(paused)
    {
        if(viewingControls)
        {
            if (m_buttonsDown.c[0].B)
            {
                viewingControls = false;
            }
            return;
        }

        switch (m_selectionBox.execute(m_cursorMove))
        {
        case RESUME:
            paused = false;
            break;
        case SUBMIT:
            paused = false;
            setModeResults();
            break;
        case RESTART:
            resetLevel();
            paused = false;
            break;
        case QUIT:
            nextState = new MainMenuState();
            break;
        case CONTROLS:
            viewingControls = true;
            break;
        
        default:
            break;
        }

        return;
    }
    else
    {
        switch (mode)
        {
        case BUILD:
            executeBuild();
            break;
        case OBSERVE:
            executeObserve();
            break;
        case RESULTS:
            executeResults();
            break;
        
        default:
            break;
        }
    }

}

void LevelState::executeObserve()
{
    m_timer -= global::elapsedSeconds;

    executeCamera();

    if(m_buttonsDown.c[0].B || m_timer <= 0.0f)
    {
        setModeBuild();
    }
}

void LevelState::executeBuild()
{

    if (m_buttonsDown.c[0].start)
    {
        if(paused)
        {
            paused = false;
            viewingControls = false;
        }
        else
        {
            paused = true;
            m_selectionBox.clearOptions();
            m_selectionBox.addOption("Resume");
            m_selectionBox.addOption("Submit");
            m_selectionBox.addOption("Restart");
            m_selectionBox.addOption("Quit");
            m_selectionBox.addOption("Controls");
        }
    }

    m_timer -= global::elapsedSeconds;

    executeCamera();

    float amountToMove = 60.0f / (float)m_amountOfTimeToBuild * global::elapsedSeconds;
    water.moveZ(amountToMove);
    water.moveY(amountToMove / 10.0f);

    const vec3i cursorPos = cursor.getPosition();
    cursor.inEmptySpace = !placed[cursorPos.y][cursorPos.z][cursorPos.x];
    
    if (m_buttonsPressed.c[0].A)
    {
        if (cursor.inEmptySpace && m_numBlocksPlaced < m_numBlocksInCastle)
        {
            placed[cursorPos.y][cursorPos.z][cursorPos.x] = true;
            m_numBlocksPlaced++;
        }
    }

    if (m_buttonsPressed.c[0].B)
    {
        if (!cursor.inEmptySpace)
        {
            placed[cursorPos.y][cursorPos.z][cursorPos.x] = false;
            m_numBlocksPlaced--;
        }
    }
    if(!camera.isRotating())
    {
        float analogY = m_buttonsPressed.c[0].y / 128.f;
        float analogX = m_buttonsPressed.c[0].x / 128.f;

        if (m_buttonsPressed.c[0].left || analogX < -0.3f)
        {
            cursor.moveLeft(camera.getRotation());
            wav64_play(&m_cursorMove, AUDIO_CHANNEL_SFX);
        }
        else if (m_buttonsPressed.c[0].right || analogX > 0.3f)
        {
            cursor.moveRight(camera.getRotation());
            wav64_play(&m_cursorMove, AUDIO_CHANNEL_SFX);
        }

        if (m_buttonsPressed.c[0].up || analogY > 0.3f)
        {
            cursor.moveForward(camera.getRotation());
            wav64_play(&m_cursorMove, AUDIO_CHANNEL_SFX);
        }
        else if (m_buttonsPressed.c[0].down || analogY < -0.3f)
        {
            cursor.moveBackward(camera.getRotation());
            wav64_play(&m_cursorMove, AUDIO_CHANNEL_SFX);
        }
    }

    if (m_timer <= 0.0f)
    {
        setModeResults();
    }
}

void LevelState::executeResults()
{
    switch (m_selectionBox.execute(m_cursorMove))
    {
    case NEXT_LEVEL:
        nextLevel();
        break;
    case REPLAY:
        resetLevel();
        break;
    case MAIN_MENU:
        nextState = new MainMenuState();
        break;
    
    default:
        break;
    }
}

void LevelState::executeCamera()
{
    camera.Transform();
    cursor.transformCursor();

    if (m_buttonsDown.c[0].Z)
    {
        camera.lookDown = !camera.lookDown;
    }

    // handle camera movement
    if (m_buttonsPressed.c[0].C_up)
    {
        camera.MoveUp();
        cursor.moveUp();
    }
    else if (m_buttonsPressed.c[0].C_down)
    {
        camera.MoveDown();
        cursor.moveDown();
    }

    if (m_buttonsPressed.c[0].C_right)
    {
        camera.RotateRight();
    }
    else if (m_buttonsPressed.c[0].C_left)
    {
        camera.RotateLeft();
    }
}

void LevelState::renderGl()
{
    for (auto &model : models)
    {
        model.draw();
    }

    bool(*arrayToDraw)[10][10][10]; // pointer to the array to draw
    arrayToDraw = mode != OBSERVE ? &placed : &castle;

    GLuint listId = modelMan::getId("cube");
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureMan::getTextureId("sand_outline"));
    float x, y, z;
    for (int height = 0; height < 10; ++height)
    {
        for (int row = 0; row < 10; ++row)
        {
            for (int col = 0; col < 10; ++col)
            {
                if ((*arrayToDraw)[height][row][col])
                {
                    x = col * 2.0f - 9.0f;
                    y = height * 2.0f + 1.0f;
                    z = row * 2.0f - 9.0f;

                    glPushMatrix();

                    glTranslatef(x, y, z);
                    glCallList(listId);

                    glPopMatrix();
                }
            }
        }
    }
    glDisable(GL_TEXTURE_2D);

    if (mode == BUILD)
    {
        cursor.draw();
    }

    glEnable(GL_BLEND);
    glColor4f(1.0f, 1.0f, 1.0f, 0.75f);
    water.draw();
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glDisable(GL_BLEND);
}

void LevelState::renderRdpq()
{
    if(paused)
    {
        if(viewingControls)
        {
            utils::drawControls();
        }
        else
        {
            drawText(" Paused");
            m_selectionBox.draw();
        }
    }
    else
    {
        switch (mode)
        {
        case BUILD:
            drawText(" Build! ");
            break;
        case OBSERVE:
            drawText("Observe!");
            break;
        case RESULTS:
            drawResultsText();
            break;
        
        default:
            break;
        }
    }

    rdpq_font_begin(RGBA32(0x38, 0x38, 0x7F, 0xFF));
    rdpq_font_scale(0.5f, 0.5f);
    rdpq_font_position(30.0f,220.0f);

    rdpq_font_printf(global::font, "%d/%d", m_numBlocksPlaced, m_numBlocksInCastle);

    // rdpq_font_position(230.0f,220.0f);
    // rdpq_font_printf(global::font, "fps:%d", (int)(1.0f / global::elapsedSeconds));
    rdpq_font_end();


}

void LevelState::calculateScore()
{
    int amountRight = 0;

    for (int height = 0; height < 10; ++height)
    {
        for (int row = 0; row < 10; ++row)
        {
            for (int col = 0; col < 10; ++col)
            {
                if (castle[height][row][col] && placed[height][row][col])
                {
                    amountRight++;
                }
            }
        }
    }

    if (amountRight == 0)
    {
        m_score = 0;
    }
    else
    {
        m_score = (100.0f / (float)m_numBlocksInCastle) * (float)amountRight;
    }

    if(m_score > global::saveFile.highestScores[m_currentLevel])
    {
        global::saveFile.highestScores[m_currentLevel] = m_score;
        const int result = eepfs_write("/levelScores.dat", &global::saveFile, sizeof(SaveFile));
    }

    if(m_score >= m_scorePassAmount)
    {
        unlockNextLevel();
    }
}

void LevelState::unlockNextLevel()
{
    if(global::saveFile.levelsUnlocked <= m_currentLevel + 1)
    {
        global::saveFile.levelsUnlocked++;

        if (global::saveFile.levelsUnlocked >= global::NUM_LEVELS)
        {
            global::saveFile.levelsUnlocked = global::NUM_LEVELS;
        }

        const int result = eepfs_write("/levelScores.dat", &global::saveFile, sizeof(SaveFile));
    }

}

void LevelState::nextLevel()
{
    m_currentLevel++;

    if (m_currentLevel >= global::NUM_LEVELS)
    {
        if(!global::saveFile.completed)
        {
            global::saveFile.completed = true;
            const int result = eepfs_write("/levelScores.dat", &global::saveFile, sizeof(SaveFile));
        }
        
        nextState = new EndState();
    }
    else
    {
        textureMan::clearTextures();
        modelMan::clearModels();
        models.clear();

        char str[20];
        sprintf(str, "rom:/level_%d.txt", m_currentLevel);

        for (int height = 0; height < 10; ++height)
        {
            for (int row = 0; row < 10; ++row)
            {
                for (int col = 0; col < 10; ++col)
                {
                    placed[height][row][col] = false;
                }
            }
        }

        for (int height = 0; height < 10; ++height)
        {
            for (int row = 0; row < 10; ++row)
            {
                for (int col = 0; col < 10; ++col)
                {
                    castle[height][row][col] = false;
                }
            }
        }

        m_numBlocksInCastle = 0;

        m_levelLoader.loadLevel(str);

        cursor.getModel().setModel("cube");
        water.setTexture("skybox");
        water.setModel("water");

        resetLevel();
    }
}

void LevelState::resetLevel()
{
    cursor.resetCursor();
    camera.resetCamera();
    m_selectionBox.clearOptions();

    //reset water back to tide out
    water.setPosition({0.0f, 0.0f, 0.0f});
    water.moveY(-5.9f);
    water.moveZ(-60.0f);

    mode = OBSERVE;
    m_timer = m_amountOfTimeToObserve;
    m_score = 0.0f;
    m_numBlocksPlaced = 0;

    //set all blocks to not placed
    for (int height = 0; height < 10; ++height)
    {
        for (int row = 0; row < 10; ++row)
        {
            for (int col = 0; col < 10; ++col)
            {
                placed[height][row][col] = false;
            }
        }
    }
}

void LevelState::drawText(const char* message)
{
    rdpq_font_begin(RGBA32(0x00, 0xff, 0x00, 0xFF));

    rdpq_font_scale(0.80f, 0.80f);
    rdpq_font_position(100.0f, 30.0f);
    rdpq_font_print(global::font, message);

    rdpq_font_scale(0.5f, 0.5f);
    rdpq_font_position(80.0f, 40.0f);
    rdpq_font_printf(global::font, "Time remaining:%d", (int)ceilf(m_timer));

    if(mode == OBSERVE)
    {
        rdpq_font_scale(0.5f, 0.5f);
        rdpq_font_position(110.0f, 220.0f);
        rdpq_font_print(global::font, "B to skip");
    }

    rdpq_font_end();
}

void LevelState::drawResultsText()
{
    rdpq_font_begin(RGBA32(0x00, 0xff, 0x00, 0xFF));    

    rdpq_font_scale(0.80f, 0.80f);
    rdpq_font_position(110.0f, 30.0f);
    rdpq_font_print(global::font, "Results!");

    rdpq_font_scale(0.5f, 0.5f);
    rdpq_font_position(80.0f, 40.0f);
    rdpq_font_printf(global::font, "You scored:%.2f", m_score);

    rdpq_font_scale(0.4f, 0.4f);
    rdpq_font_position(30.0f, 50.0f);
    rdpq_font_printf(global::font, (m_score < m_scorePassAmount) ? "You didn't score enough to continue" : "You scored high enough to continue!");

    rdpq_font_end();

    m_selectionBox.draw();
}

void LevelState::setModeResults()
{
    mode = RESULTS;
    calculateScore();

    m_selectionBox.clearOptions();
    m_selectionBox.addOption("Main Menu");
    m_selectionBox.addOption("Restart Level");
    m_selectionBox.setCurrentOption(1);
    
    if(m_score >= m_scorePassAmount)
    {
        m_selectionBox.addOption("Next Level");
        m_selectionBox.setCurrentOption(2);
    }
}

void LevelState::setModeObserve()
{
}

void LevelState::setModeBuild()
{
    m_timer = m_amountOfTimeToBuild;
    mode = BUILD;
}
