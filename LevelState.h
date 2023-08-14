#pragma once
#include "State.h"
#include <vector>
#include <string>
#include <sstream>
#include "TexturedModel.h"
#include "Cursor.h"
#include "SelectionBox.h"


class LevelState: public State
{
    enum Mode{ OBSERVE, BUILD, RESULTS };
    enum Selection{ MAIN_MENU, REPLAY, NEXT_LEVEL};
    enum PauseSelection{ RESUME, RESTART, QUIT};

public:
    LevelState(const size_t level);
    ~LevelState();
    virtual void execute() override;
    virtual void renderGl() override;
    virtual void renderRdpq() override;

private:
    void loadLevel(const char* fileNamePath);
    void loadModel(const char* modelString);
    void loadCastle(FILE* file, const char* lastString);
    void calculateScore();
    void nextLevel();
    void resetLevel();

    TexturedModel water;
    Cursor cursor;
    std::vector<TexturedModel> models;

    bool castle[10][10][10] = {0};
    bool placed[10][10][10] = {0};

    const size_t MAXIMUM_BLOCKS = 150;

    size_t m_currentLevel = 1;
    size_t m_numBlocksInCastle = 0;
    size_t m_numBlocksPlaced = 0;
    float m_timer = 0.0f;
    float m_amountOfTimeToObserve = 10.0f;
    float m_amountOfTimeToBuild = 60.0f;

    float m_score = 0.0f;

    enum Mode mode = OBSERVE; 
    bool paused = false;

    wav64_t m_ambience;

    SelectionBox m_selectionBox;
};