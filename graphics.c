#include "graphics.h"

void InitGraphics(HWND hWnd)
{
  d3d = Direct3DCreate9(D3D_SDK_VERSION);
  D3DPRESENT_PARAMETERS d3dpp = {
    .Windowed = FALSE,
    .SwapEffect = D3DSWAPEFFECT_DISCARD,
    .hDeviceWindow = hWnd,
    .BackBufferFormat = D3DFMT_X8R8G8B8,
    .BackBufferWidth = SCREEN_WIDTH,
    .BackBufferHeight = SCREEN_HEIGHT,
    .EnableAutoDepthStencil = TRUE,
    .AutoDepthStencilFormat = D3DFMT_D16
  };

  d3d->lpVtbl->CreateDevice(d3d, D3DADAPTER_DEFAULT,
    D3DDEVTYPE_HAL, hWnd,
    D3DCREATE_SOFTWARE_VERTEXPROCESSING,
    &d3dpp, &d3ddev);

  RenderSettings = (_RenderSettings) {
    .fogStart = 2.0f,
    .fogEnd = 100.0f,
    .fogColor = { 1.0f, 1.0f, 1.0f }
  };

  D3DVERTEXELEMENT9 decl[] = {
    { 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
    { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
    { 0, 24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
    { 0, 28, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
    D3DDECL_END()
  };

  d3ddev->lpVtbl->CreateVertexDeclaration(d3ddev, decl, &vertDeclaration);

  D3DXCreateFont(d3ddev, 30, 0, FW_NORMAL, 1, TRUE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,   
    DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &d3dFont);

  startupTime = (double)timeGetTime();
}

void CleanGraphics()
{
  d3dFont->lpVtbl->Release(d3dFont);
  vertDeclaration->lpVtbl->Release(vertDeclaration);
  d3ddev->lpVtbl->Release(d3ddev);
  d3d->lpVtbl->Release(d3d);
}
