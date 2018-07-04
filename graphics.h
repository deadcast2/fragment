#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <windows.h>
#include <d3d9.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define CUSTOMFVF (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

LPDIRECT3D9 d3d;
LPDIRECT3DDEVICE9 d3ddev;
LPDIRECT3DVERTEXBUFFER9 testBuffer;

struct CUSTOMVERTEX
{
  FLOAT x, y, z, rhw;
  DWORD color;
};

void InitGraphics(HWND hWnd);
void CleanGraphics();

#endif
