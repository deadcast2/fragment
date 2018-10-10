#include "scene.h"

void InitScene()
{
  actors[0] = CreateActor((struct actorProps) {
    .modelName = "IDR_ISLAND_STATIC",
    .textureName = "IDR_ISLAND_TEX",
    .position = (struct vertex) { .x = 0, .y = -7, .z = 0 },
    .rotation = (struct vertex) { .x = 0, .y = D3DX_PI/2, .z = 0 },
    .scale = (struct vertex) { .x = 1, .y = 1, .z = 1 }
  });
  actors[1] = CreateActor((struct actorProps) {
    .modelName = "IDR_SKY",
    .textureName = "IDR_SKY_TEX",
    .position = (struct vertex) { .x = 0, .y = 0, .z = 0 },
    .rotation = (struct vertex) { .x = 0, .y = D3DX_PI/2, .z = 0 },
    .scale = (struct vertex) { .x = 1, .y = 1, .z = 1 }
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
  for(int i = 0; i < ACTOR_COUNT; i++)
  {
    DrawActor(actors[i], stack, d3ddev);
  }
  d3ddev->lpVtbl->EndScene(d3ddev);
  d3ddev->lpVtbl->Present(d3ddev, NULL, NULL, NULL, NULL);
}

void CleanScene()
{
  for(int i = 0; i < ACTOR_COUNT; i++)
  {
    DeleteActor(actors[i]);
  }
}
