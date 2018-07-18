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
    .BackBufferHeight = SCREEN_HEIGHT
  };
  d3d->lpVtbl->CreateDevice(d3d, D3DADAPTER_DEFAULT,
    D3DDEVTYPE_HAL, hWnd,
    D3DCREATE_SOFTWARE_VERTEXPROCESSING,
    &d3dpp, &d3ddev);
  d3ddev->lpVtbl->SetRenderState(d3ddev, D3DRS_LIGHTING, FALSE);
}

void CleanGraphics()
{
  d3ddev->lpVtbl->Release(d3ddev);
  d3d->lpVtbl->Release(d3d);
}
