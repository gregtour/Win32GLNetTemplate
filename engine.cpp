/* engine.cpp */
#include "engine.h"
#include "game.h"

entity* entity::entities[256];

/* entity base functions */
void entity::update(float dt) {}
void entity::render() {}
void entity::event(message* m) {}

/* messages */
int message::type()
{
    return 0;
}

/* inputmessage class */
inputmessage::inputmessage(char a)
{
    value = a;
}

int inputmessage::type()
{
    return E_INPUT;
}

char inputmessage::character()
{
    return value;
}

/* clear entity list */
void entity::ClearEntities()
{
    for (int i = 0; i < 256; i++)
    {
        entities[i] = 0l;
    }
}

/* add entity to list */
bool entity::AddEntity(entity* e)
{
    for (int i = 0; i < 256; i++)
    {
        if (entities[i] == 0l) {
            entities[i] = e;
            return true;
        }
    }
    return false;
}

/* remove entity from list */
void entity::RemoveEntity(entity* e)
{
    for (int i = 0; i < 256; i++)
    {
        if (entities[i] == e)
        {
            entities[i] = 0l;
            delete e;
        }
    }
}

/* remove and delete all entities */
void entity::Shutdown()
{
    for (int i = 0; i < 256; i++)
    {
        if (entities[i])
        {
            delete entities[i];
            entities[i] = 0l;
        }
    }
}

/* update all entities */
void entity::Update(float dt)
{
    for (int i = 0; i < 256; i++)
        if (entities[i])
            entities[i]->update(dt);
}

/* render all entities */
void entity::Render()
{
    /* clear screen */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glLoadIdentity();
    
    /* render all */
    for (int i = 0; i < 256; i++)
        if (entities[i])
            entities[i]->render();
}

/* message passing */
void entity::Event(message* m)
{
    for (int i = 0; i < 256; i++)
        if (entities[i])
            entities[i]->event(m);
}
