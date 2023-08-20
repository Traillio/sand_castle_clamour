#pragma once
#include "State.h"
#include <vector>
#include <string>
#include <sstream>
#include "TexturedModel.h"
#include "Cursor.h"
#include "SelectionBox.h"
#include "LevelLoader.h"



class LevelState: public State
{
    friend class LevelLoader;

    enum Mode{ OBSERVE, BUILD, RESULTS };
    enum ResultSelection{ MAIN_MENU, REPLAY, NEXT_LEVEL};
    enum PauseSelection{ RESUME, SUBMIT, RESTART, QUIT, CONTROLS};

public:
    LevelState(const size_t level = 0);
    ~LevelState();
    virtual void execute() override;
    virtual void renderGl() override;
    virtual void renderRdpq() override;

private:
    void calculateScore();
    void unlockNextLevel();
    void nextLevel();
    void resetLevel();
    
    void executeObserve();
    void executeBuild();
    void executeResults();

    void executeCamera();

    void drawText(const char* message);
    void drawResultsText();

    void setModeResults();
    void setModeObserve();
    void setModeBuild();

    TexturedModel water;
    Cursor cursor;
    std::vector<TexturedModel> models;

    bool castle[10][10][10] = {0};
    bool placed[10][10][10] = {0};

    const size_t MAXIMUM_BLOCKS = 150;

    size_t m_currentLevel = 0;
    size_t m_numBlocksInCastle = 0;
    size_t m_numBlocksPlaced = 0;
    float m_timer = 0.0f;
    float m_amountOfTimeToObserve = 10.0f;
    float m_amountOfTimeToBuild = 60.0f;
    float m_score = 0.0f;
    float m_scorePassAmount = 80.0f;

    struct controller_data m_buttonsPressed;
    struct controller_data m_buttonsDown;

    enum Mode mode = OBSERVE; 
    bool paused = false;
    bool viewingControls = false;

    //wav64_t m_ambience;
    SelectionBox m_selectionBox;

    LevelLoader m_levelLoader;
};