#include "graphics.h"

void InitDirectX(HWND hWnd)
{
  LPDIRECT3D9 d3d = Direct3DCreate9(D3D_SDK_VERSION);
  LPDIRECT3DDEVICE9 d3ddev = NULL;
  D3DPRESENT_PARAMETERS d3dpp = {
    .Windowed = TRUE,
    .SwapEffect = D3DSWAPEFFECT_DISCARD,
    .hDeviceWindow = hWnd
  };
  d3d->lpVtbl->CreateDevice(d3d, D3DADAPTER_DEFAULT,
    D3DDEVTYPE_HAL, hWnd,
    D3DCREATE_SOFTWARE_VERTEXPROCESSING,
    &d3dpp, &d3ddev);
}
