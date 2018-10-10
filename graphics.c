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
  d3ddev->lpVtbl->SetRenderState(d3ddev, D3DRS_LIGHTING, FALSE);
  d3ddev->lpVtbl->SetRenderState(d3ddev, D3DRS_ZENABLE, TRUE);
  d3ddev->lpVtbl->SetRenderState(d3ddev, D3DRS_ALPHAREF, 0x0000008f);
  d3ddev->lpVtbl->SetRenderState(d3ddev, D3DRS_ALPHATESTENABLE, TRUE);
  d3ddev->lpVtbl->SetRenderState(d3ddev, D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
  d3ddev->lpVtbl->SetRenderState(d3ddev, D3DRS_SRCBLEND, D3DBLEND_ONE);
  d3ddev->lpVtbl->SetRenderState(d3ddev, D3DRS_DESTBLEND, D3DBLEND_ZERO);
}

void CleanGraphics()
{
  d3ddev->lpVtbl->Release(d3ddev);
  d3d->lpVtbl->Release(d3d);
}
