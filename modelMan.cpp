#include "modelMan.h"

GLuint modelMan::loadModel(const char* filePath)
{
    model64_t* model = model64_load(filePath);
    GLuint listId = glGenLists(1);

    glNewList(listId, GL_COMPILE);

    model64_draw(model);

    glEndList();

    std::string key(filePath);
    key = utils::isolateFileName(key);
    displayLists[std::move(key)] = listId;

    return listId;
}

GLuint modelMan::getId(const std::string &modelName)
{
    return displayLists[modelName];
}

/// @brief create a plane model that will be stored under the specified name
/// @param width how many squares wide
/// @param height how many squares high
/// @param textureRepeat how many times to repeat the texture per square
/// @param name the id that you would like to store this plane under
void modelMan::generatePlane(size_t width, size_t height, float textureRepeat, const char *name)
{
    struct vertex
    {
        float positions[3];
        float texcoord[2];
    };

    uint16_t tileIndicies[] = 
    {
        0, 1, 2, 0, 2, 3
    };

    vertex verts[width * height * 4];
    uint16_t inds[width * height * 6];

    GLuint listId = glGenLists(1);
    glNewList(listId, GL_COMPILE);


    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(3, GL_FLOAT, sizeof(vertex), &verts[0].positions);
    glTexCoordPointer(2, GL_FLOAT, sizeof(vertex), &verts[0].texcoord);

    size_t currentTile = 0;
    size_t vertLocation = 0;
    size_t indLocation = 0;
    for(float h = 0; h < height; h++)
    {
        for(float w = 0; w < width; w++)
        {
            vertLocation = currentTile * 4; // 4 is the amount of verts per tile
            //postions                      x           y           z             s                t
            verts[vertLocation]         = {{w,          0.0f,       h         }, {0.0f,            0.0f            }};
            verts[vertLocation + 1]     = {{w,          0.0f,       h + 1.0f  }, {0.0f,            textureRepeat   }};
            verts[vertLocation + 2]     = {{w + 1.0f,   0.0f,       h + 1.0f  }, {textureRepeat,   textureRepeat   }};
            verts[vertLocation + 3]     = {{w + 1.0f,   0.0f,       h         }, {textureRepeat,   0.0f            }};

            // tex coords
            indLocation = currentTile * 6; // 6 is the number of indices per tile
            for(size_t i = 0; i < 6; ++i)
            {
                inds[indLocation + i] = tileIndicies[i] + vertLocation;
            }

            currentTile++;
        }
    }
    
    glTranslatef(-(float)width / 2.0f, 0.0f, -(float)height / 2.0f);
    glDrawElements(GL_TRIANGLES, sizeof(inds) / sizeof(uint16_t), GL_UNSIGNED_SHORT, inds);
    
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glEndList();
    displayLists[std::string(name)] = listId;
}

void modelMan::clearModels()
{
    for(auto& list: displayLists)
    {
        glDeleteLists(list.second, 1);
    }

    displayLists.clear();
}
