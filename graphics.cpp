/* graphics.cpp */
#define WINDOWS_LEAN_AND_MEAN
#include <windows.h>
#include "engine.h"
#include "graphics.h"

/* static data */
int SCR_WIDTH  = 640;
int SCR_HEIGHT = 480;
int nFontList;

/* intialize OpenGL state */
void SetupGL()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_CW);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, SCR_WIDTH, SCR_HEIGHT, 0, -1000.0f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
