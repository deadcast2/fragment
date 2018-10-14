#include "scene.h"

void SkyStart(struct actor *self)
{
  PlayAudio(self->audioSource);
}

void SkyUpdate(struct actor *self, float deltaTime)
{
  self->rotation.x += 0.1f * deltaTime;
}

void InitScene()
{
  seed_smooth_rand();
  randomFogEnd = fogStep = fogSpeed = 200;
  actors[0] = CreateActor((struct actorProps) {
    .modelName = "IDR_ISLAND",
    .textureName = "IDR_ISLAND_TEX",
    .position = (struct vertex) { .x = 0, .y = -6.6, .z = 0 },
    .rotation = (struct vertex) { .x = D3DX_PI, .y = 0, .z = 0 },
    .scale = (struct vertex) { .x = 1, .y = 1, .z = 1 },
    .start = NULL,
    .update = NULL
  });
  actors[1] = CreateActor((struct actorProps) {
    .modelName = "IDR_SKY",
    .textureName = "IDR_SKY_TEX",
    .audioName = "IDR_WIND",
    .position = (struct vertex) { .x = 0, .y = 0, .z = 0 },
    .rotation = (struct vertex) { .x = 0, .y = 0, .z = 0 },
    .scale = (struct vertex) { .x = 1, .y = 1, .z = 1 },
    .start = SkyStart,
    .update = SkyUpdate,
    .audioProps = (struct audioProps) { .shouldLoop = TRUE }
  });
}

void RenderScene(float deltaTime)
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
    DrawActor(actors[i], stack, d3ddev, deltaTime);
  }
  d3ddev->lpVtbl->EndScene(d3ddev);
  d3ddev->lpVtbl->Present(d3ddev, NULL, NULL, NULL, NULL);

  if (fogStep == randomFogEnd)
  {
    int fogDistances[12] = {
		    17, 18, 18, 18, 18, 19, 25, 200, 200, 200, 200, 200
	  };
    float fogSpeeds[5] = {
		    0.1, 0.1, 0.15, 0.15, 0.08
	  };
    lastFogEnd = randomFogEnd;
    randomFogEnd = fogDistances[smooth_rand() % 12];
    fogSpeed = fogSpeeds[smooth_rand() % 5];
    fogTime = 0;
  }
  fogStep = smooth_inter(lastFogEnd, randomFogEnd, fogTime += deltaTime * fogSpeed);
  d3ddev->lpVtbl->SetRenderState(d3ddev, D3DRS_FOGEND, *(DWORD*)(&fogStep));
}

void CleanScene()
{
  for(int i = 0; i < ACTOR_COUNT; i++)
  {
    DeleteActor(actors[i]);
  }
}
