/* game.cpp */
#include "graphics.h"
#include "game.h"

void StartGame()
{
    entity::ClearEntities();
    entity::AddEntity(new cube());
    entity::AddEntity(new textbuffer());
}

/* text buffer */
char textbuffer::buffer[16][256];
int  textbuffer::count = 0;

void textbuffer::putline(const char* string)
{
    int i;
    for (i = 0; i < 255 && string[i]; i++)
    {
        buffer[count%16][i] = string[i];
        if (string[i] == '\n') break;
    }
    buffer[count%16][i] = '\0';
    count++;

    if (string[i] == '\n') {
        putline(&string[i+1]);
    }
}

void textbuffer::putchar(char a)
{
    int i = strlen(buffer[count%16]);
    i = i < 254 ? i : 254;
    if (a != '\n') {
        buffer[count%16][i] = a;
        buffer[count%16][i+1] = '\0';
    }
    if (a == '\r') count++;
    if (a == '\b') {
        i = i ? i - 1 : 0;
        buffer[count%16][i] = '\0';
    }
}

void textbuffer::render()
{
    /* draw text */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    for (int i = 0; i < 16; i++)
    {
        if (i <= count)
        {
            glColor3f(1.0f, 1.0f, 1.0f);
            glRasterPos2i(0, 25*(i+1));
            glListBase(nFontList);
            glCallLists(strlen(buffer[i]), GL_UNSIGNED_BYTE, buffer[i]);
        }
    }
}

void textbuffer::event(message* m)
{
    switch (m->type())
    {
        case E_INPUT:
        {
            inputmessage* input = (inputmessage*)m;
            textbuffer::putchar(input->character());
            return;
        }
        
        default:
            return;
    }
}


/* spinning triangle */
void tri::update(float dt)
{
    frame++;
}

void tri::render()
{
    glTranslatef(SCR_WIDTH/2, SCR_HEIGHT/2, 0.0);
    glRotatef(frame, 0, 0, 1);
    glScalef(2.0f, 2.0f, 2.0f);

    glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(-87.0f, -50.0f, 0.0f);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, 100.0f, 0.0f);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(87.0f, -50.0f, 0.0f);
    glEnd();
}

/* spinning cube */
void cube::update(float dt)
{
    frame++;
}

void cube::render()
{
    float vertex[] = {
        -1, -1, -1, 1, -1, -1,
        -1, -1, 1,  1, -1, 1,
        -1, 1, -1,  1, 1, -1,
        -1, 1, 1,   1, 1, 1
    };
    
    float color[] = {
        1, 0, 0,    0, 1, 0,
        0, 0, 1,    1, 0, 0,
        0, 1, 0,    0, 0, 1,
        1, 0, 0,    0, 1, 0
    };
    
    int index[] = {
        1, 2, 3, 2, 4, 3,
        7, 6, 5, 7, 8, 6,
        1, 6, 2, 1, 5, 6,
        3, 4, 8, 3, 8, 7,
        1, 3, 7, 1, 7, 5,
        2, 6, 4, 6, 8, 4
    };
    
    glTranslatef(SCR_WIDTH/2, SCR_HEIGHT/2, -100.0f);
    glRotatef(frame, 1, 1, 1);
    glScalef(100.0f, 100.0f, 100.0f);

    glBegin(GL_TRIANGLES);
        for (int i = 0; i < 36; i++)
        {
            int j = (index[i] - 1)*3;
            glColor4f(color[j], color[j+1], color[j+2], 1.0f);
            glVertex3f(vertex[j], vertex[j+1], vertex[j+2]);
        }
    glEnd();
}
