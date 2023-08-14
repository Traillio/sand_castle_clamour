#pragma once
#include "vecs.h"
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "globals.h"

class Camera
{
public:
    Camera();

public:
    static constexpr int ROTATION_INC = 90;
    static constexpr float ROTATION_SPEED = 90.0f;
    static constexpr float CAMERA_MOVE_SPEED = 10.0f; 
    static constexpr float MOVE_INC = 2.0f;

    static constexpr int MAX_HEIGHT = 25.5;
    static constexpr int MIN_HEIGHT = 7.5;
    static constexpr int MAX_DISTANCE = 40;
    static constexpr int MIN_DISTANCE = 2;

    static constexpr float INITIAL_TILT = 0;
    static constexpr float INITIAL_DISTANCE = 22.0f;

    float m_tilt;
    float m_distance;

    vec3f m_initialPosition;
    vec3f m_position;

    bool m_rotating = false;
    bool m_rotateRight;

    float m_rotation = 0.0f;
    float m_amountToRotateDegrees = 0.0f;

    bool m_moving = false;
    bool m_movingUp;
    float m_amountToMove = 0.0f;

public:
    bool lookDown = false;
    bool isRotating();
    void resetCamera();
    void Transform();
    void RotateRight();
    void RotateLeft();
    void MoveUp();
    void MoveDown();
    void MoveY(float amount);
    void Zoom(float amount);
    int getRotation();

private:
    void Rotate(float degrees);
};


