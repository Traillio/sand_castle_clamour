#pragma once
#include "vecs.h"
#include <string>
#include "modelMan.h"
#include "textureMan.h"
#include "GL/gl.h"

class Model
{
protected:
    vec3f m_rotation;
    vec3f m_position;
    vec3f m_scale;

    GLuint m_listId;
    
public:
    Model(const std::string& modelName);
    Model(const GLuint listId);
    Model();
    virtual ~Model();

    void setModel(std::string modelName);

    void setPosition(const vec3f pos);
    void setRotation(const vec3f rot);
    const vec3f& getPosition();
    void setScale(const vec3f scale);
    void move(const vec3f amount);
    void moveX(float amount);
    void moveY(float amount);
    void moveZ(float amount);

    void rotate(vec3f amount);
    void rotateX(float amount);
    void rotateY(float amount);
    void rotateZ(float amount);

    void snapX();
    void snapY();
    void snapZ();

    void scalef(float x, float y, float z);

    virtual void draw();
};