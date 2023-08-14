#include "Camera.h"

Camera::Camera() :
m_initialPosition({0.0f, MIN_HEIGHT, INITIAL_DISTANCE}), m_position(m_initialPosition), m_tilt(INITIAL_TILT), m_distance(INITIAL_DISTANCE)
{

}

bool Camera::isRotating()
{
    return m_rotating;
}

void Camera::resetCamera()
{
    lookDown = false;
    m_initialPosition = {0.0f, MIN_HEIGHT, INITIAL_DISTANCE};
    m_position = m_initialPosition;
    m_tilt = INITIAL_TILT;
    m_distance = INITIAL_DISTANCE;

    m_moving = false;
    m_rotation = 0.0f;
    m_rotating = false;
    m_amountToRotateDegrees = 0.0f;
    m_amountToMove = 0.0f;
}

void Camera::Transform()
{
    if(m_rotating)
    {
        
        float degrees = ROTATION_SPEED * global::elapsedSeconds;

        if(m_amountToRotateDegrees < degrees)
        {
            m_rotating = false;
            m_rotation = round(m_rotation / 90.0f) * 90.0f;
            Rotate(0);
            m_amountToRotateDegrees = 0.0f;
            
        }
        else
        {
            m_rotateRight ? Rotate(degrees) : Rotate(-degrees);
            m_amountToRotateDegrees -= degrees;
        }

    }

    if(m_moving)
    {
        float amount = CAMERA_MOVE_SPEED * global::elapsedSeconds;

        if(m_amountToMove < amount)
        {
            m_moving = false;
            amount = m_amountToMove;
        }

        m_movingUp ? MoveY(amount) : MoveY(-amount);
        m_amountToMove -= amount;
    }

    glLoadIdentity();
    if(lookDown)
    {
        gluLookAt(0.001, m_position.y + MAX_HEIGHT , 0.001, 0, 1 ,0 , -m_position.x, m_position.y, -m_position.z);
    }
    else
    {
        gluLookAt( m_position.x, m_position.y, m_position.z, 0, m_position.y - m_tilt ,0 , 0, 1, 0);
    }
    
    
}

void Camera::RotateRight()
{
    //prevent stacking rotation increments
    if(m_rotating)
    {
        return;
    }

    m_amountToRotateDegrees = 90.0f;
    m_rotateRight = true;
    m_rotating = true;
}

void Camera::RotateLeft()
{
    //prevent stacking rotation increments
    if(m_rotating)
    {
        return;
    }

    m_amountToRotateDegrees = 90.0f;
    m_rotateRight = false;
    m_rotating = true;
}

void Camera::MoveUp()
{
    if(m_moving)
    {
        return;
    }

    m_moving = true;
    m_movingUp = true;
    m_amountToMove = MOVE_INC;
}

void Camera::MoveDown()
{
    if(m_moving)
    {
        return;
    }

    m_moving = true;
    m_movingUp = false;
    m_amountToMove = MOVE_INC;
}

void Camera::Zoom(float amount)
{
    m_initialPosition.z += amount;

    if(m_initialPosition.z > MAX_DISTANCE)
    {
        m_initialPosition.z = MAX_DISTANCE;
    }
    else if(m_initialPosition.z < MIN_DISTANCE)
    {
        m_initialPosition.z = MIN_DISTANCE;
    }

    Rotate(0.0f); // laziness
}

int Camera::getRotation()
{
    return m_rotation;
}

void Camera::MoveY(float amount)
{
    m_position.y += amount;
    

    if(m_position.y > MAX_HEIGHT)
    {
        m_position.y = MAX_HEIGHT;
        m_moving = false;
        m_amountToMove = 0.0f;
    }
    else if(m_position.y < MIN_HEIGHT)
    {
        m_position.y = MIN_HEIGHT;
        m_moving = false;
        m_amountToMove = 0.0f;
    }
}

void Camera::Rotate(float degrees)
{
    m_rotation += degrees;

    //give em the clamps
    while(m_rotation < 0.0f)
    {
        m_rotation += 360.0f;
    }

    while(m_rotation >= 360.0f)
    {
        m_rotation -= 360.0f;
    }

    float rads = -m_rotation * ( 3.14159 / 180);
    m_position.x = m_initialPosition.x * cosf(rads) - m_initialPosition.z * sinf(rads);
    m_position.z = m_initialPosition.x * sinf(rads) + m_initialPosition.z * cosf(rads);
}