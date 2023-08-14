#pragma once
#include "vecs.h"
#include "Model.h"
#include "globals.h"


class Cursor
{
    struct CursorComponent
    {
        bool moving;
        float amountToMove;
        int& position;
    };

    static constexpr vec4f GOOD_COLOUR = {0.0f, 1.0f, 0.0f, 0.75f};
    static constexpr vec4f BAD_COLOUR = {1.0f, 0.0f, 0.0f, 0.75f};
    const vec3i INITIAL_POSITION;
    static constexpr float CURSOR_MOVE_SPEED = 10.0f;

    
    Model m_cube;

    CursorComponent m_x;
    CursorComponent m_y;
    CursorComponent m_z;
    vec3i m_position;
    vec3i actualPosition;
    
    
public:
    Cursor();
    
    void resetCursor();
    bool inEmptySpace = true;
    const vec3i getPosition();
    void moveForward(const int rotationDegrees);
    void moveBackward(const int rotationDegrees);
    void moveLeft(const int rotationDegrees);
    void moveRight(const int rotationDegrees);
    void handleMove(float x, float y, float rotationDegrees);
    void doMove(CursorComponent& comp, const int amount);
    void moveUp();
    void moveDown();
    Model& getModel();

    void transformCursor();
    float doTransorm(CursorComponent& comp, float amount);
    void draw();
};