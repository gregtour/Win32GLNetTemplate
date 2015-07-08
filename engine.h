/* engine.h */
#ifndef _ENGINE_H
#define _ENGINE_H

#include "graphics.h"
#include "network.h"

/* game event message types */
enum
{
    E_NONE = 0,
    E_INPUT,
    E_GAME,
    E_NETWORK,
    E_SYSTEM
};

/* event messages */
class message
{
public:
    virtual int type();
};

/* input message */
class inputmessage : public message
{
    char value;
public:
    inputmessage(char a);
    int  type();
    char character();
};

/* entity */
class entity
{
    static entity* entities[256];
public:
    static void ClearEntities();
    static bool AddEntity(entity* e);
    static void RemoveEntity(entity* e);
    static void Shutdown();
    static void Update(float dt);
    static void Render();
    static void Event(message* m);
private:
    virtual void update(float dt);
    virtual void render();
    virtual void event(message* m);
};

#endif
