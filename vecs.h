#pragma once

struct vec3f
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
};

union vec4f
{
    struct 
    {
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
        float w = 0.0f;  
    };

    struct 
    {
        float r;
        float g;
        float b;
        float a;  
    };
};

struct vec3i
{
    int x = 0;
    int y = 0;
    int z = 0;
};

