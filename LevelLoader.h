#pragma once
#include <cstdio>

class LevelState;

class LevelLoader
{
public:
    LevelLoader(LevelState& levelState);

    void loadLevel(const char* fileNamePath);
    void loadModel(const char* modelString);
    void loadCastle(FILE* file, const char* lastString);

private:
    LevelState& m_levelState;
};