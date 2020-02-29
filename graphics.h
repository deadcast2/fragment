#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

LPDIRECT3D9 d3d;
LPDIRECT3DDEVICE9 d3ddev;
LPDIRECT3DVERTEXDECLARATION9 vertDeclaration;
LPD3DXFONT d3dFont;
double startupTime;

typedef struct _RenderSettings
{
  float fogStart;
  float fogEnd;
  float fogColor[3];
} _RenderSettings;

_RenderSettings RenderSettings;

void InitGraphics(HWND hWnd);
void CleanGraphics();

#endif
