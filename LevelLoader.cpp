#include "LevelLoader.h"
#include "LevelState.h"


LevelLoader::LevelLoader(LevelState& levelState) : m_levelState(levelState)
{
}

void LevelLoader::loadLevel(const char *fileNamePath)
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
                m_levelState.m_amountOfTimeToBuild = strtol(buf, NULL, 10);
            case 'O': // observatoin time
                m_levelState.m_amountOfTimeToObserve = strtof(buf, NULL);
                break;
            default:
                break;
            }
        }
    }

    fclose(file);
}

void LevelLoader::loadModel(const char *modelString)
{
    std::istringstream ss(modelString);
    std::string model;
    std::string texture;
    vec3f position;
    vec3f scale;
    vec3f rotation;

    ss >> model >> texture >> position.x >> position.y >> position.z >> scale.x >> scale.y >> scale.z >> rotation.x >> rotation.y >> rotation.z;

    TexturedModel texModel(model, texture);
    texModel.setPosition(position);
    texModel.setScale(scale);
    texModel.setRotation(rotation);

    m_levelState.models.push_back(std::move(texModel));
}

void LevelLoader::loadCastle(FILE *file, const char *firstLine)
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
            ss >> m_levelState.castle[height][row][col];
            if (m_levelState.castle[height][row][col])
            {
                m_levelState.m_numBlocksInCastle++;
            }
        }

        row++;
        row %= 10;
    } while (fgets(buf, BUFFER_SIZE, file) && buf[0] != '*');
}
