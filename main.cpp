/*
  Cube-Strike
  main.cpp
*/

#define WINDOWS_LEAN_AND_MEAN
#include <windows.h>
#include <cstdio>
#include "engine.h"
#include "game.h"

/*  declare windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  make the class name into a global variable  */
//#ifndef
char szClassName[ ] = "Cube-Strike";

int WINAPI WinMain (HINSTANCE   hThisInstance,
                    HINSTANCE   hPrevInstance,
                    LPSTR       lpszArgument,
                    int         nCmdShow)
{
    HWND hwnd;
    MSG messages;
    WNDCLASSEX wincl;

    /* window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;
    wincl.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wincl.cbSize = sizeof (WNDCLASSEX);
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;
    wincl.hbrBackground = NULL;

    /* register with windows */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* place in the center of the screen */
    int x, y, width, height, windowType;
    width = SCR_WIDTH + GetSystemMetrics(SM_CXFIXEDFRAME) * 2;
    height = SCR_HEIGHT + GetSystemMetrics(SM_CYFIXEDFRAME) * 2
                 + GetSystemMetrics(SM_CYCAPTION);
    x = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
    y = (GetSystemMetrics(SM_CYSCREEN) - height)/2;
    windowType = WS_POPUPWINDOW | WS_CAPTION | WS_MINIMIZEBOX
        | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
    
    /* create the game window */
    hwnd = CreateWindow (
           szClassName,         /* Classname */
           szClassName,         /* Title Text */
           windowType,          /* default window */
           x,                   /* Windows decides the position */
           y,                   /* where the window ends up on the screen */
           width,               /* The programs width */
           height,              /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* make the window visible on the screen */
    ShowWindow(hwnd, nCmdShow);
    
    /* game code */
    StartGame();

    /* run the message loop */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }

    /* post quit */
    return messages.wParam;
}

/* set up window's pixel format */
void SetDCPixelFormat(HDC hDC)
{
    int nPixelFormat;
    
    static PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW |
        PFD_SUPPORT_OPENGL |
        PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        32,
        0, 0, 0, 0, 0, 0,
        0, 0,
        0, 0, 0, 0, 0,
        16,
        0,
        0,
        0,
        0,
        0, 0, 0
    };
    
    nPixelFormat = ChoosePixelFormat(hDC, &pfd);
    SetPixelFormat(hDC, nPixelFormat, &pfd);
}

/* create bitmap font */
void SetupRC(HDC hDC)
{
    HFONT hFont;
    LOGFONT logfont;

    logfont.lfHeight = -20;
    logfont.lfWidth = 0;
    logfont.lfEscapement = 0;
    logfont.lfOrientation = 0;
    logfont.lfWeight = FW_BOLD;
    logfont.lfItalic = false;
    logfont.lfUnderline = false;
    logfont.lfStrikeOut = false;
    logfont.lfCharSet = ANSI_CHARSET;
    logfont.lfOutPrecision = OUT_DEFAULT_PRECIS;
    logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
    logfont.lfQuality = DEFAULT_QUALITY;
    logfont.lfPitchAndFamily = DEFAULT_PITCH;
    strcpy(logfont.lfFaceName, "Arial");

    // create the font and display list
    hFont = CreateFontIndirect(&logfont);
    SelectObject(hDC, hFont);

    // Create display lists for glyphs 0 through 128
    nFontList = glGenLists(128);
    wglUseFontBitmaps(hDC, 0, 128, nFontList);
    DeleteObject(hFont);
}

/*  This function is called by the Windows function DispatchMessage()  */
LRESULT CALLBACK WindowProcedure (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HGLRC hRC = NULL;
    static HDC hDC = NULL;

    switch (message)
    {
    case WM_CREATE:
        // create window, bind opengl, & start networking
        hDC = GetDC(hWnd);
        SetDCPixelFormat(hDC);
        hRC = wglCreateContext(hDC);
        wglMakeCurrent(hDC, hRC);
        SetupRC(hDC);
        SetupGL();
        SetupNetwork();
        SetTimer(hWnd, 33, 1, NULL);
        break;
        
    case WM_DESTROY:
        // shutdown
        KillTimer(hWnd, 101);
        wglMakeCurrent(hDC, NULL);
        wglDeleteContext(hRC);
        entity::Shutdown();
        PostQuitMessage(0);
        break;
        
    case WM_CLOSE:
        // send wm_quit
        PostQuitMessage(0);
        break;

    case WM_TIMER:
        // frame timer
        entity::Update(0.033f);
        InvalidateRect(hWnd, NULL, false);
        break;
        
    case WM_PAINT:
        // redraw screen
        entity::Render();
        SwapBuffers(hDC);
        ValidateRect(hWnd, NULL);
        return 0;
        
    case WM_SYSCHAR:
        // ?
        break;
        
    case WM_CHAR:
        // keyboard input
        if (wParam == 27)
        {
            PostQuitMessage(0);
            return 0;
        }
        else
        {
            inputmessage im((char)wParam);
            entity::Event(&im);
        }
        break;
        
    case WM_QUIT:
    default:
        // let windows handle it
        return DefWindowProc (hWnd, message, wParam, lParam);
    }

    return 0;
}
