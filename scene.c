#include "scene.h"

void InitScene()
{
  testVertices = LoadModel("IDR_MODEL1", &testVertexCount);
  d3ddev->lpVtbl->CreateVertexBuffer(
    d3ddev, testVertexCount * sizeof(struct vertex), 0,
    CUSTOMFVF, D3DPOOL_MANAGED, &testVertexBuffer, 0);

  VOID *pVoid;
  testVertexBuffer->lpVtbl->Lock(testVertexBuffer, 0,
    0, (void**)&pVoid, 0);
  CopyMemory(pVoid, testVertices, testVertexCount * sizeof(struct vertex));
  testVertexBuffer->lpVtbl->Unlock(testVertexBuffer);

  LoadTexture("IDR_TEX1");
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
    d3ddev->lpVtbl->SetTexture(d3ddev, 0, (IDirect3DBaseTexture9*)testTexture);
    d3ddev->lpVtbl->SetFVF(d3ddev, CUSTOMFVF);
    d3ddev->lpVtbl->SetStreamSource(d3ddev, 0, testVertexBuffer,
      0, sizeof(struct vertex));
    d3ddev->lpVtbl->DrawPrimitive(d3ddev, D3DPT_TRIANGLELIST, 0,
      testVertexCount / 3);
  }
  d3ddev->lpVtbl->EndScene(d3ddev);
  d3ddev->lpVtbl->Present(d3ddev, NULL, NULL, NULL, NULL);
}

void CleanScene()
{
  HeapFree(GetProcessHeap(), 0, testVertices);
  testVertexBuffer->lpVtbl->Release(testVertexBuffer);
  testTexture->lpVtbl->Release(testTexture);
}
