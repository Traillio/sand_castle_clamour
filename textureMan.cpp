#include "textureMan.h"

void textureMan::loadTexture(const char* filePath)
{
    sprite_t* sprite = sprite_load(filePath);
    GLuint textureId;
    glGenTextures(1, &textureId);

    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    rdpq_texparms_t params;
    params.s.repeats = REPEAT_INFINITE;
    params.t.repeats = REPEAT_INFINITE;
    params.tmem_addr = 0;
    params.palette = 0;
    params.s.scale_log = 0;
    params.t.scale_log = 0;
    params.s.mirror = false;
    params.t.mirror = false;
    params.s.translate = 0;
    params.t.translate = 0;

    glSpriteTextureN64(GL_TEXTURE_2D, sprite, &params);

    std::string key(filePath);
    key = utils::isolateFileName(key);
    textures[std::move(key)] = Texture{textureId, sprite};
}
void textureMan::setTexture(const std::string &name)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textures[name].m_textureId);
}
void textureMan::clearTextures()
{
    for(auto& texture: textures)
    {
        glDeleteTextures(1, &texture.second.m_textureId);

        if(texture.second.sprite != nullptr)
        {
            sprite_free(texture.second.sprite);
        }
            
    }

    textures.clear();
}

GLuint textureMan::getTextureId(const std::string &textureName)
{
    return textures[textureName].m_textureId;
}

sprite_t *textureMan::getSprite(const std::string &textureName)
{
    return textures[textureName].sprite;
}
