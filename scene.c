#include "scene.h"

void InitScene()
{
  struct CUSTOMVERTEX TestVertices[] =
  {
    {320.0f, 50.0f, 1.0f, 1.0f, D3DCOLOR_XRGB(0, 0, 255)},
    {520.0f, 400.0f, 1.0f, 1.0f, D3DCOLOR_XRGB(0, 255, 0)},
    {120.0f, 400.0f, 1.0f, 1.0f, D3DCOLOR_XRGB(255, 0, 0)}
  };
  d3ddev->lpVtbl->CreateVertexBuffer(d3ddev, 3 * sizeof(struct CUSTOMVERTEX),
    0, CUSTOMFVF, D3DPOOL_MANAGED, &testBuffer, NULL);
  VOID *pVoid;
  testBuffer->lpVtbl->Lock(testBuffer, 0, 0, (void**)&pVoid, 0);
  CopyMemory(pVoid, TestVertices, sizeof(TestVertices));
  testBuffer->lpVtbl->Unlock(testBuffer);
}

void RenderScene()
{
  d3ddev->lpVtbl->Clear(d3ddev, 0, NULL, D3DCLEAR_TARGET,
    D3DCOLOR_XRGB(0, 40, 100), 1.0f, 0);
  d3ddev->lpVtbl->BeginScene(d3ddev);
  {
    d3ddev->lpVtbl->SetFVF(d3ddev, CUSTOMFVF);
    d3ddev->lpVtbl->SetStreamSource(d3ddev, 0, testBuffer, 0,
      sizeof(struct CUSTOMVERTEX));
    d3ddev->lpVtbl->DrawPrimitive(d3ddev, D3DPT_TRIANGLELIST, 0, 1);
  }
  d3ddev->lpVtbl->EndScene(d3ddev);
  d3ddev->lpVtbl->Present(d3ddev, NULL, NULL, NULL, NULL);
}
