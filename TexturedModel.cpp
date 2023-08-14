#include "TexturedModel.h"

TexturedModel::TexturedModel(std::string modelName, std::string textureName) :
Model(modelName), m_textureName(textureName)
{

}

TexturedModel::TexturedModel(GLuint listId, std::string textureName) :
Model(listId), m_textureName(textureName)
{
}

TexturedModel::TexturedModel()
{
}

void TexturedModel::draw()
{
    textureMan::setTexture(m_textureName);

    glPushMatrix();
    glTranslatef(m_position.x, m_position.y, m_position.z);
    glRotatef(m_rotation.x, 1, 0, 0);
    glRotatef(m_rotation.y, 0, 1, 0);
    glRotatef(m_rotation.z, 0, 0, 1);
    glScalef(m_scale.x, m_scale.y, m_scale.z);

    // Draw the cube, replaying the display list
    glCallList(m_listId);

    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
}

void TexturedModel::setTexture(const std::string &textureName)
{
    m_textureName = textureName;
}
