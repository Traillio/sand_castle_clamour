#include <libdragon.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/gl_integration.h>
#include <math.h>

#include "Camera.h"
#include "modelMan.h"
#include <unordered_map>
#include "TexturedModel.h"
#include "textureMan.h"
#include <vector>
#include "IntroState.h"
#include "globals.h"

State* gameState;

surface_t* disp;
surface_t zbuffer;

void setup()
{
    // setup transparency

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    gameState = new IntroState();

    zbuffer = surface_alloc(FMT_RGBA16, display_get_width(), display_get_height());


    float aspect_ratio = (float)display_get_width() / (float)display_get_height();
    float near_plane = 7.50f;
    float far_plane = 480.0f;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(80.0f, aspect_ratio, near_plane, far_plane);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


int main()
{
    long long start, end;
    timer_init();

	debug_init_isviewer();
	debug_init_usblog();

    dfs_init(DFS_DEFAULT_LOCATION);

    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 3, GAMMA_NONE, ANTIALIAS_RESAMPLE_FETCH_ALWAYS);

    rdpq_init();
    gl_init();

    audio_init(44100, 4);
	mixer_init(16);

    setup();

    controller_init();
    glClearColor(0, 0, 0, 1);
    while (1)
    {
        start = timer_ticks();

        gameState->execute();

        disp = display_get();
        rdpq_attach(disp, &zbuffer);

        ////begin and clear surface
        gl_context_begin();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        gameState->renderGl();

        gl_context_end();

        gameState->renderRdpq();



        if(gameState->nextState != nullptr)
        {
            State* nextState = gameState->nextState;
            delete gameState;

            gameState = nextState;
        }

        end = timer_ticks();
        float previousFrame = global::elapsedSeconds;
        global::elapsedSeconds = (float)TIMER_MICROS_LL(end-start)/1000000.0f;


        if(global::elapsedSeconds < 0.0f || global::elapsedSeconds > 1.0f)
        {
            debugf("Frame shit its pants, using previous frame time");
            global::elapsedSeconds = previousFrame;
        }

        rdpq_detach_show();
    }

}
