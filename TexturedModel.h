#pragma once
#include "Model.h"

class TexturedModel : public Model
{
public:
    TexturedModel(std::string modelName, std::string textureName);
    TexturedModel(GLuint listId, std::string textureName);
    TexturedModel();
    virtual void draw() override;

    void setTexture(const std::string& textureName);
private:
    std::string m_textureName;
    
};