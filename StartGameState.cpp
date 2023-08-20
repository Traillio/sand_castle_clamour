#include "StartGameState.h"

StartGameState::StartGameState() : m_textBox(50)
{
    m_textBox.addLine("          Welcome!           ");
    m_textBox.addLine("");
    m_textBox.addLine("Thanks for joining the sand");
    m_textBox.addLine("castle cup! There are 5 days.");
    m_textBox.addLine("");
    m_textBox.addLine("      Heres what to do:      ");
    m_textBox.addLine("");
    m_textBox.addLine("-Observe the castle");
    m_textBox.addLine("-Build before the tide comes");
    m_textBox.addLine("-Get 80% of the castle right");
    m_textBox.addLine("");
    m_textBox.addLine("         good luck!          ");
    m_textBox.addLine("");
    m_textBox.addLine("     Press A to continue     ");
}

void StartGameState::execute()
{
    controller_scan();

    if(get_keys_down().c[0].A)
    {
        nextState = new LevelState();
        
        if(global::saveFile.levelsUnlocked == 0)
        {
            global::saveFile.levelsUnlocked = 1;
        }
    }
}

void StartGameState::renderRdpq()
{
    m_textBox.draw();
}
