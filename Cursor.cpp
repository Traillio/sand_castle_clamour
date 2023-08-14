#include "Cursor.h"

Cursor::Cursor():
m_position(INITIAL_POSITION), INITIAL_POSITION({global::BOARD_DIMENSION / 2, 0, global::BOARD_DIMENSION / 2}), actualPosition(m_position),
m_x({false, 0.0f, m_position.x}), 
m_y({false, 0.0f, m_position.y}),
m_z({false, 0.0f, m_position.z})
{
    //m_cube.moveY(0.50f);

    if(global::BOARD_DIMENSION % 2 == 0)
    {
        m_cube.moveX(1.0f);
        m_cube.moveZ(1.0f);
    }
    
    m_cube.scalef(1.1f, 1.1f, 1.1f);
}

void Cursor::moveForward(const int rotationDegrees)
{
    float z = -1.0f;
    float x = 0.0f;

    handleMove(x, z, rotationDegrees);
}

void Cursor::moveBackward(const int rotationDegrees)
{
    float z = 1.0f;
    float x = 0.0f;

    handleMove(x, z, rotationDegrees);
}

void Cursor::moveLeft(const int rotationDegrees)
{
    float z = 0.0f;
    float x = -1.0f;

    handleMove(x, z, rotationDegrees);
}

void Cursor::moveRight(const int rotationDegrees)
{
    float z = 0.0f;
    float x = 1.0f;

    handleMove(x, z, rotationDegrees);
}

void Cursor::handleMove(float x, float z, float rotationDegrees)
{
    //block movement if the camera is rotating
    if((int)rotationDegrees % 90 != 0)
    {
        return;
    }

    int newX;
    int newZ;

    float rads = -(float)rotationDegrees * ( 3.14159 / 180);
    newX = x * cosf(rads) - z * sinf(rads);
    newZ = x * sinf(rads) + z * cosf(rads);


    if(newX == 0)
    {
        doMove(m_z, newZ);
    }
    else
    {
        doMove(m_x, newX);
    }
}

void Cursor::doMove(CursorComponent& comp, const int amount)
{
        if(comp.moving)
        {
            return;
        }

        comp.position += amount;
        
        if(comp.position >= global::BOARD_DIMENSION)
        {
            comp.position = comp.position = global::BOARD_DIMENSION - 1;
        }
        else if(comp.position < 0)
        {
            comp.position = 0;
        }
        else
        {
            comp.amountToMove = 2.0f * amount;
            comp.moving = true;
        }
}

void Cursor::moveUp()
{
    doMove(m_y, 1);
}

void Cursor::moveDown()
{
    doMove(m_y, -1);
}

void Cursor::transformCursor()
{
    float amount = CURSOR_MOVE_SPEED * global::elapsedSeconds;

    if(m_x.moving)
    {
        m_cube.moveX(doTransorm(m_x, amount));

        if(!m_x.moving)
        {
            actualPosition.x = m_position.x;
        }
    }

    if(m_y.moving)
    {
        m_cube.moveY(doTransorm(m_y, amount));

        if(!m_y.moving)
        {
            actualPosition.y = m_position.y;
        }
    }

    if(m_z.moving)
    {
        m_cube.moveZ(doTransorm(m_z, amount));

        if(!m_z.moving)
        {
            actualPosition.z = m_position.z;
        }
    }
}

void Cursor::resetCursor()
{
    m_position = INITIAL_POSITION;
    actualPosition = m_position;
    m_x.amountToMove = 0.0f;
    m_y.amountToMove = 0.0f;
    m_z.amountToMove = 0.0f;

    m_x.moving = false;
    m_y.moving = false;
    m_z.moving = false;

    m_x.position = m_position.x;
    m_y.position = m_position.y;
    m_z.position = m_position.z;

    m_cube.setPosition({0.0f, 1.0f, 0.0f});

    if(global::BOARD_DIMENSION % 2 == 0)
    {
        m_cube.moveX(1.0f);
        m_cube.moveZ(1.0f);
    }
}

const vec3i Cursor::getPosition()
{
    return actualPosition;
}

float Cursor::doTransorm(CursorComponent& comp, float amount)
{
    
    float newAmount;

    if(abs(amount) > abs(comp.amountToMove))
    {
        newAmount = comp.amountToMove;
        comp.moving = false;
    }
    else
    {
        newAmount = amount;

        if(comp.amountToMove < 0.0f)
        {
            newAmount = -newAmount;
        }
    }

    comp.amountToMove -= newAmount;
    return newAmount;
}

Model& Cursor::getModel()
{
    return m_cube;
}

void Cursor::draw()
{
    glEnable(GL_BLEND);
    inEmptySpace ? glColor4f(GOOD_COLOUR.r, GOOD_COLOUR.g, GOOD_COLOUR.b, GOOD_COLOUR.a) : glColor4f(BAD_COLOUR.r, BAD_COLOUR.g, BAD_COLOUR.b, BAD_COLOUR.a);
    m_cube.draw();
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glDisable(GL_BLEND);
}