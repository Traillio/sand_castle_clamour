#pragma once
#include <unordered_map>
#include <string>
#include <GL/gl.h>
#include "utils.h"
#include "model64.h"
#include "libdragon.h"
#include <vector>


namespace modelMan
{
    namespace
    {
        std::unordered_map<std::string, GLuint> displayLists;
    }

    GLuint loadModel(const char* filePath);
    GLuint getId(const std::string& modelName);
    void generatePlane(size_t width, size_t height, float textureRepeat, const char* name);

    void clearModels();

}