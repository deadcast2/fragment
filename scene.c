#include "scene.h"

void InitScene()
{

}

void RenderScene()
{
  d3ddev->lpVtbl->Clear(d3ddev, 0, NULL, D3DCLEAR_TARGET,
    D3DCOLOR_XRGB(0, 40, 100), 1.0f, 0);
  d3ddev->lpVtbl->BeginScene(d3ddev);
  d3ddev->lpVtbl->EndScene(d3ddev);
  d3ddev->lpVtbl->Present(d3ddev, NULL, NULL, NULL, NULL);
}
