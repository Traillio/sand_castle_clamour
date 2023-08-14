#pragma once
#include <unordered_map>
#include <string>
#include "sprite.h"
#include "GL/gl.h"
#include "utils.h"
#include "rdpq_tex.h"

namespace textureMan
{
    struct Texture
    {
        GLuint m_textureId;
        sprite_t* sprite;
    };

    namespace
    {
        std::unordered_map<std::string, Texture> textures;
    }

    void loadTexture(const char* filePath);
    void setTexture(const std::string& name);
    void clearTextures();
    GLuint getTextureId(const std::string& textureName);
    sprite_t* getSprite(const std::string& textureName);
}