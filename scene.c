#include "scene.h"

void InitScene()
{
  struct CUSTOMVERTEX TestVertices[] =
  {
    {2.5f, -3.0f, -5.0f, D3DCOLOR_XRGB(255, 0, 0)},
    {0.0f, 3.0f, -5.0f, D3DCOLOR_XRGB(0, 255, 0)},
    {-2.5f, -3.0f, -5.0f, D3DCOLOR_XRGB(0, 0, 255)}
  };
  d3ddev->lpVtbl->CreateVertexBuffer(d3ddev, 3 * sizeof(struct CUSTOMVERTEX),
    0, CUSTOMFVF, D3DPOOL_MANAGED, &testBuffer, NULL);
  VOID *pVoid;
  testBuffer->lpVtbl->Lock(testBuffer, 0, 0, (void**)&pVoid, 0);
  CopyMemory(pVoid, TestVertices, sizeof(TestVertices));
  testBuffer->lpVtbl->Unlock(testBuffer);

  LoadModel("IDR_MODEL1");
}

void RenderScene()
{
  D3DMATRIX matRotY;
  D3DXMatrixRotationY(&matRotY, 3.14f);
  d3ddev->lpVtbl->SetTransform(d3ddev, D3DTS_WORLD, &matRotY);

  D3DXMATRIX temp = CameraViewMatrix();
  d3ddev->lpVtbl->SetTransform(d3ddev, D3DTS_VIEW, &temp);

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
