#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <d3d9.h>
#include <d3dx9.h>
#include <windows.h>

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

LPDIRECT3D9 d3d;
LPDIRECT3DDEVICE9 d3ddev;
LPDIRECT3DVERTEXDECLARATION9 vertDeclaration;
LPD3DXFONT d3dFont1;
LPD3DXFONT d3dFont2;
double startupTime;

typedef struct _RenderSettings {
  float fogStart;
  float fogEnd;
  float fogColor[3];
} _RenderSettings;

_RenderSettings RenderSettings;

void InitGraphics(HWND hWnd);
void CleanGraphics();

#endif
