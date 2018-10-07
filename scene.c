#include "scene.h"

void InitScene()
{
  testActor = CreateActor((struct actorProps) {
    .modelName = "IDR_MODEL1",
    .textureName = "IDR_TEX1"
  });
}

void RenderScene()
{
  D3DXMATRIX camMat = CameraViewMatrix();
  d3ddev->lpVtbl->SetTransform(d3ddev, D3DTS_VIEW, &camMat);

  d3ddev->lpVtbl->Clear(d3ddev, 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
    D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

  d3ddev->lpVtbl->BeginScene(d3ddev);
  {
    d3ddev->lpVtbl->SetTexture(d3ddev, 0, (IDirect3DBaseTexture9*)testActor->d3dTexture);
    d3ddev->lpVtbl->SetFVF(d3ddev, CUSTOMFVF);
    d3ddev->lpVtbl->SetStreamSource(d3ddev, 0, testActor->vertexBuffer,
      0, sizeof(struct vertex));
    d3ddev->lpVtbl->DrawPrimitive(d3ddev, D3DPT_TRIANGLELIST, 0,
      testActor->vertexCount / 3);
  }
  d3ddev->lpVtbl->EndScene(d3ddev);
  d3ddev->lpVtbl->Present(d3ddev, NULL, NULL, NULL, NULL);
}

void CleanScene()
{
  DeleteActor(testActor);
}
