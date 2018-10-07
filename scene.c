#include "scene.h"

void InitScene()
{
  testActor = CreateActor((struct actorProps) {
    .modelName = "IDR_MODEL1",
    .textureName = "IDR_TEX1",
    .position = (struct vertex) {
      .x = 0, .y = -2, .z = 10
    },
    .rotation = (struct vertex) {
      .x = 10, .y = 0, .z = 0
    },
    .scale = (struct vertex) {
      .x = 0.2, .y = 0.2, .z = 0.2
    }
  });
}

void RenderScene()
{
  ID3DXMatrixStack *stack;
  D3DXCreateMatrixStack(0, &stack);
  D3DXMATRIX camMat = CameraViewMatrix();
  stack->lpVtbl->LoadMatrix(stack, &camMat);
  d3ddev->lpVtbl->SetTransform(d3ddev, D3DTS_VIEW, stack->lpVtbl->GetTop(stack));

  d3ddev->lpVtbl->Clear(d3ddev, 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
    D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

  d3ddev->lpVtbl->BeginScene(d3ddev);
  {
    D3DXMATRIX translation;
    D3DXMatrixTranslation(&translation, testActor->position.x,
      testActor->position.y, testActor->position.z);

    D3DXMATRIX rotation;
    D3DXMatrixRotationYawPitchRoll(&rotation, testActor->rotation.x,
      testActor->rotation.y, testActor->rotation.z);

    D3DXMATRIX scale;
    D3DXMatrixScaling(&scale, testActor->scale.x,
      testActor->scale.y, testActor->scale.z);

    D3DXMATRIX actorMat;
    D3DXMatrixMultiply(&actorMat, &scale, &rotation);
    D3DXMatrixMultiply(&actorMat, &actorMat, &translation);

    stack->lpVtbl->Push(stack);
    stack->lpVtbl->MultMatrixLocal(stack, &actorMat);

    d3ddev->lpVtbl->SetTransform(d3ddev, D3DTS_VIEW, stack->lpVtbl->GetTop(stack));
    d3ddev->lpVtbl->SetTexture(d3ddev, 0, (IDirect3DBaseTexture9*)testActor->d3dTexture);
    d3ddev->lpVtbl->SetFVF(d3ddev, CUSTOMFVF);
    d3ddev->lpVtbl->SetStreamSource(d3ddev, 0, testActor->vertexBuffer,
      0, sizeof(struct vertex));
    d3ddev->lpVtbl->DrawPrimitive(d3ddev, D3DPT_TRIANGLELIST, 0,
      testActor->vertexCount / 3);

    stack->lpVtbl->Pop(stack);
  }
  d3ddev->lpVtbl->EndScene(d3ddev);
  d3ddev->lpVtbl->Present(d3ddev, NULL, NULL, NULL, NULL);
}

void CleanScene()
{
  DeleteActor(testActor);
}
