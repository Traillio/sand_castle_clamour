#include "Model.h"

void Model::draw()
{
    //glDisable(GL_TEXTURE_2D);
    glPushMatrix();
    
    glTranslatef(m_position.x, m_position.y, m_position.z);
    glRotatef(m_rotation.x, 1, 0, 0);
    glRotatef(m_rotation.y, 0, 1, 0);
    glRotatef(m_rotation.z, 0, 0, 1);
    glScalef(m_scale.x, m_scale.y, m_scale.z);
    // Draw the cube, replaying the display list
    glCallList(m_listId);

    glPopMatrix();
}

void Model::snapX()
{
    m_position.x = roundf(m_position.x);
}

void Model::snapY()
{
    m_position.y = roundf(m_position.y);
}

void Model::snapZ()
{
    m_position.z = roundf(m_position.z);
}

void Model::scalef(float x, float y, float z)
{
    m_scale.x = x;
    m_scale.y = y;
    m_scale.z = z;
}

Model::Model(const std::string& modelName) :
m_listId(modelMan::getId(modelName)), m_scale({1.0f, 1.0f, 1.0f}), m_position({0.0f, 0.0f, 0.0f}), m_rotation({0.0f, 0.0f, 0.0f})
{
}

Model::Model(const GLuint listId) :
m_listId(listId), m_scale({1.0f, 1.0f, 1.0f}), m_position({0.0f, 1.0f, 0.0f}), m_rotation({0.0f, 0.0f, 0.0f})
{
}

Model::Model() :
m_scale({1.0f, 1.0f, 1.0f}), m_position({0.0f, 1.0f, 0.0f}), m_rotation({0.0f, 0.0f, 0.0f})
{
}

Model::~Model()
{
}

void Model::setModel(std::string modelName)
{
    m_listId =  modelMan::getId(modelName);
}

void Model::setPosition(const vec3f pos)
{
    m_position = pos;
}

void Model::setScale(const vec3f scale)
{
    m_scale = scale;
}

const vec3f& Model::getPosition()
{
    return m_position;
}

void Model::setRotation(const vec3f rot)
{
    m_rotation = rot;
}

void Model::move(const vec3f amount)
{
}

void Model::moveX(float amount)
{
    m_position.x += amount;
}

void Model::moveY(float amount)
{
    m_position.y += amount;
}

void Model::moveZ(float amount)
{
    m_position.z += amount;
}

void Model::rotate(vec3f amount)
{
}

void Model::rotateX(float amount)
{
    m_rotation.x += amount;
    m_rotation.x = (int)m_rotation.x % 360;
}

void Model::rotateY(float amount)
{
    m_rotation.y += amount;
    m_rotation.y = (int)m_rotation.y % 360;
}

void Model::rotateZ(float amount)
{
    m_rotation.z += amount;
    m_rotation.z = (int)m_rotation.z % 360;
}
