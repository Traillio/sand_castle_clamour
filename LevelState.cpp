#include "MainMenuState.h"
#include "LevelState.h"

LevelState::LevelState(const size_t level) : m_selectionBox(80, 120), m_currentLevel(level)
{
    camera.resetCamera();
    glClearColor(0, 0, 1, 1);
    textureMan::clearTextures();
    modelMan::clearModels();

    char str[20];
    sprintf(str, "rom:/level_%d.txt", m_currentLevel);
    loadLevel(str);

    m_timer = m_amountOfTimeToObserve;
    cursor.getModel().setModel("cube");

    water.setTexture("skybox");
    water.setModel("water");
    water.setScale({106.0f, 106.0f, 106.0f});
    water.moveY(-5.9f);
    water.moveZ(-60.0f);

    wav64_open(&m_ambience, "rom:/ambience.wav64");
    wav64_set_loop(&m_ambience, true);
    wav64_play(&m_ambience, AUDIO_CHANNEL_MUSIC);
    mixer_ch_set_vol(AUDIO_CHANNEL_MUSIC, 0.1f, 0.1f);
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
    struct controller_data pressed = get_keys_pressed();
    struct controller_data down = get_keys_down();

    float y = pressed.c[0].y / 128.f;
    float x = pressed.c[0].x / 128.f;

    if (down.c[0].start && mode == BUILD)
    {
        if(paused)
        {
            paused = false;
        }
        else
        {
            paused = true;
            m_selectionBox.clearOptions();
            m_selectionBox.addOption("Resume");
            m_selectionBox.addOption("Restart");
            m_selectionBox.addOption("Quit");
        }
    }

    if(paused)
    {
        switch (m_selectionBox.execute(m_cursorMove))
        {
        case RESUME:
            paused = false;
            break;
        case RESTART:
            resetLevel();
            paused = false;
            break;
        case QUIT:
            nextState = new MainMenuState();
            break;
        
        default:
            break;
        }

        return;
    }

    if (mode == OBSERVE || mode == BUILD)
    {
        m_timer -= global::elapsedSeconds;
        camera.Transform();
        cursor.transformCursor();

        if (down.c[0].Z)
        {
            camera.lookDown = !camera.lookDown;
        }

        // handle camera movement
        if (pressed.c[0].C_up)
        {
            camera.MoveUp();
            cursor.moveUp();
        }
        else if (pressed.c[0].C_down)
        {
            camera.MoveDown();
            cursor.moveDown();
        }

        if (pressed.c[0].C_right)
        {
            camera.RotateRight();
        }
        else if (pressed.c[0].C_left)
        {
            camera.RotateLeft();
        }
    }

    if (mode == OBSERVE && m_timer <= 0.0f)
    {
        m_timer = m_amountOfTimeToBuild;
        mode = BUILD;
    }
    else if (mode == BUILD)
    {
        float amountToMove = 60.0f / (float)m_amountOfTimeToBuild * global::elapsedSeconds;

        water.moveZ(amountToMove);
        water.moveY(amountToMove / 10.0f);

        const vec3i cursorPos = cursor.getPosition();
        cursor.inEmptySpace = !placed[cursorPos.y][cursorPos.z][cursorPos.x];

        if (pressed.c[0].A)
        {
            if (cursor.inEmptySpace && m_numBlocksPlaced < m_numBlocksInCastle)
            {
                placed[cursorPos.y][cursorPos.z][cursorPos.x] = true;
                m_numBlocksPlaced++;
            }
        }

        if (pressed.c[0].B)
        {
            if (!cursor.inEmptySpace)
            {
                placed[cursorPos.y][cursorPos.z][cursorPos.x] = false;
                m_numBlocksPlaced--;
            }
        }
        if(!camera.isRotating())
        {
            if (pressed.c[0].left || x < -0.3f)
            {
                cursor.moveLeft(camera.getRotation());
                wav64_play(&m_cursorMove, AUDIO_CHANNEL_SFX);
            }
            else if (pressed.c[0].right || x > 0.3f)
            {
                cursor.moveRight(camera.getRotation());
                wav64_play(&m_cursorMove, AUDIO_CHANNEL_SFX);
            }

            if (pressed.c[0].up || y > 0.3f)
            {
                cursor.moveForward(camera.getRotation());
                wav64_play(&m_cursorMove, AUDIO_CHANNEL_SFX);
            }
            else if (pressed.c[0].down || y < -0.3f)
            {
                cursor.moveBackward(camera.getRotation());
                wav64_play(&m_cursorMove, AUDIO_CHANNEL_SFX);
            }
        }


        if (m_timer <= 0.0f)
        {
            mode = RESULTS;
            calculateScore();

            m_selectionBox.clearOptions();
            m_selectionBox.addOption("Main Menu");
            m_selectionBox.addOption("Restart Level");
            m_selectionBox.setCurrentOption(1);
            
            if(m_score > 75.0f)
            {
                m_selectionBox.addOption("Next Level");
                m_selectionBox.setCurrentOption(2);
            }
        }
    }
    else if (mode == RESULTS)
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

}

void LevelState::renderGl()
{
    for (auto &model : models)
    {
        model.draw();
    }

    bool(*arrayToDraw)[10][10][10];

    if (mode != OBSERVE)
    {
        arrayToDraw = &placed;
    }
    else
    {
        arrayToDraw = &castle;
    }

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
    if (mode == OBSERVE || mode == BUILD)
    {
        const char *modeMessage;
        if(!paused)
        {
            modeMessage = (mode == OBSERVE) ? "Observe!" : " Build! ";
        }
        else
        {
            modeMessage = " Paused!";
        }
        

        rdpq_font_begin(RGBA32(0x00, 0xff, 0x00, 0xFF));
        rdpq_font_scale(1.0f, 1.0f);
        rdpq_font_position(90, 20);
        rdpq_font_print(m_font, modeMessage);

        rdpq_font_scale(0.5f, 0.5f);
        rdpq_font_position(80, 40);
        rdpq_font_printf(m_font, "Time remaining: %d", (int)ceilf(m_timer));
        rdpq_font_end();
    }
    else
    {
        rdpq_font_begin(RGBA32(0x00, 0xff, 0x00, 0xFF));
        rdpq_font_scale(1.0f, 1.0f);
        rdpq_font_position(90, 20);
        rdpq_font_print(m_font, "Results!");

        rdpq_font_scale(0.5f, 0.5f);
        rdpq_font_position(80, 40);
        rdpq_font_printf(m_font, "Your score is %.2f%%", m_score);

        rdpq_font_scale(0.4f, 0.4f);
        rdpq_font_position(30, 50);
        rdpq_font_printf(m_font, (m_score < 75.0f) ? "You didn't score enough to continue" : "You scored high enough to continue!");

        rdpq_font_end();

        m_selectionBox.draw(m_font);
    }

    rdpq_font_begin(RGBA32(0x38, 0x38, 0x7F, 0xFF));
    rdpq_font_scale(0.5f, 0.5f);
    rdpq_font_position(10,230);

    rdpq_font_printf(m_font, "%d/%d", m_numBlocksPlaced, m_numBlocksInCastle);

    rdpq_font_position(240,230);
    rdpq_font_printf(m_font, " fps:%d", (int)(1.0f / global::elapsedSeconds));
    rdpq_font_end();

    if(paused)
    {
        m_selectionBox.draw(m_font);
    }
}

void LevelState::loadLevel(const char *fileNamePath)
{
    size_t BUFFER_SIZE = 100;
    char mode;

    FILE *file = asset_fopen(fileNamePath);

    char buf[BUFFER_SIZE];

    if (file)
    {
        while (fgets(buf, BUFFER_SIZE, file))
        {
            if (buf[0] == '*')
            {
                mode = buf[1];
                continue;
            }
            else if (buf[0] == '#')
            {
                continue;
            }

            // remove newline
            buf[strlen(buf) - 1] = '\0';

            switch (mode)
            {
            case 't': // textures
                textureMan::loadTexture(buf);
                break;
            case 'm': // models
                modelMan::loadModel(buf);
                break;
            case 'M': // model objects
                loadModel(buf);
                break;
            case 'f': // fonts
                      // todo
                break;
            case 'c': // castle
                loadCastle(file, buf);
                break;
            case 'T': // time to complete
                m_amountOfTimeToBuild = strtol(buf, NULL, 10);
            case 'O': // observatoin time
                m_amountOfTimeToObserve = strtof(buf, NULL);
                break;
            default:
                break;
            }
        }
    }

    fclose(file);
}

void LevelState::loadModel(const char *modelString)
{
    std::istringstream ss(modelString);
    std::string model;
    std::string texture;
    vec3f position;
    vec3f scale;

    ss >> model >> texture >> position.x >> position.y >> position.z >> scale.x >> scale.y >> scale.z;

    TexturedModel texModel(model, texture);
    texModel.setPosition(position);
    texModel.setScale(scale);

    models.push_back(std::move(texModel));
}

void LevelState::loadCastle(FILE *file, const char *firstLine)
{
    size_t BUFFER_SIZE = 100;
    char buf[BUFFER_SIZE];

    strcpy(buf, firstLine);

    size_t row, col, height;
    row = 0;
    col = 0;
    height = 0;
    do
    {
        std::istringstream ss(buf);

        if (buf[0] == '#')
        {
            height++;
            continue;
        }

        for (size_t col = 0; col < 10; ++col)
        {
            ss >> castle[height][row][col];
            if (castle[height][row][col])
            {
                m_numBlocksInCastle++;
            }
        }

        row++;
        row %= 10;
    } while (fgets(buf, BUFFER_SIZE, file) && buf[0] != '*');
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
}

void LevelState::nextLevel()
{
    m_currentLevel++;
    if(global::levelsUnlocked < m_currentLevel)
    {
        global::levelsUnlocked++;
    }
    

    if (m_currentLevel > 5)
    {
        global::levelsUnlocked = 5;
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

        loadLevel(str);

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
