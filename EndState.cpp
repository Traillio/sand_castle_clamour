#include "EndState.h"

EndState::EndState() : m_textBox(10)
{
    wav64_open(&m_winSound, "rom:/beachy.wav64");
    mixer_ch_set_vol(AUDIO_CHANNEL_MUSIC, 0.3f, 0.3f);

    const char* cupColor;
    char fileName[25];
    char spriteName[20];
    

    for(int i = 0; i < global::NUM_LEVELS; i++)
    {
        m_score += global::saveFile.highestScores[i];
    }

    m_score /= global::NUM_LEVELS;

    if(m_score <= 87.0f)
    {
        cupColor = "bronze";
    }
    else if(m_score < 94.0f)
    {
        cupColor = "silver";
    }
    else
    {
        cupColor = "gold";
    }

    sprintf(fileName, "rom:/cup_%s.sprite", cupColor);
    sprintf(spriteName, "cup_%s", cupColor);

    textureMan::loadTexture(fileName);
    m_cupSprite = textureMan::getSprite(std::string(spriteName));

    char scoreLine[30];

    sprintf(scoreLine, "Average Highest Score: %.2f!", m_score);

    m_textBox.addLine("      Congratulations!");
    m_textBox.addLine("");
    m_textBox.addLine("  You finished in the top 3");
    m_textBox.addLine("");
    m_textBox.addLine("  Here are your high scores:");
    m_textBox.addLine("");

    for(int i = 0; i < global::NUM_LEVELS; i++)
    {
        char levelScore[30];
        sprintf(levelScore, "       Day %d: %.2f", i + 1, global::saveFile.highestScores[i]);
        m_textBox.addLine(std::string(levelScore));
    }

    m_textBox.addLine("");
    m_textBox.addLine(std::string(scoreLine));
}

void EndState::execute()
{
     if (audio_can_write())
    {
        short *buf = audio_write_begin();
        mixer_poll(buf, audio_get_buffer_length());
        audio_write_end();
    }
    controller_scan();
    struct controller_data down = get_keys_down();

    if(!m_playedSound)
    {
        m_playedSound = true;
        wav64_play(&m_winSound, AUDIO_CHANNEL_MUSIC);
    }

    if(down.c[0].B)
    {
        nextState = new MainMenuState();
    }

}

void EndState::renderRdpq()
{
    // I appear to have made a mistake somewhere, this prevents a crash
    if(nextState != nullptr)
    {
        return;
    }

    m_textBox.draw();

    rdpq_set_mode_standard();
    rdpq_mode_filter(FILTER_BILINEAR);
    rdpq_mode_alphacompare(1);
    utils::rdpq_sprite_blit(m_cupSprite, 160 - 32, 165, NULL);
}
