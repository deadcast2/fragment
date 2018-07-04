#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <windows.h>
#include <d3d9.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

LPDIRECT3D9 d3d;
LPDIRECT3DDEVICE9 d3ddev;

void InitGraphics(HWND hWnd);
void CleanGraphics();

#endif
