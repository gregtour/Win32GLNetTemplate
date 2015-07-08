/* game.h */
#ifndef _GAME_H
#define _GAME_H

#include "engine.h"

void StartGame();

class textbuffer : public entity
{
    static char buffer[16][256];
    static int count;
public:
    static void putline(const char* string);
    static void putchar(char a);
    void render();
    void event(message* m);
};

class tri : public entity
{
    int frame;
    void update(float dt);
    void render();
};

class cube : public entity
{
    int frame;
    void update(float dt);
    void render();
};

#endif

