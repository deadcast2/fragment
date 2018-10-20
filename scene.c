#include "scene.h"

void CrowStart(Actor *self)
{
  PlayAudio(self->audioSource, self->audioBuffer);
  nextCaw = 0;
}

void CrowUpdate(Actor *self, float deltaTime)
{
  if (nextCaw < 0)
  {
    PlayAudio(self->audioSource, self->audioBuffer);
    nextCaw = smooth_rand() % 20;
  }
  nextCaw -= 0.5f * deltaTime;
}

void SkyStart(Actor *self)
{
  PlayAudio(self->audioSource, self->audioBuffer);
}

void SkyUpdate(Actor *self, float deltaTime)
{
  self->rotation.x += 0.1f * deltaTime;
}

void InitScene()
{
  seed_smooth_rand();
  randomFogEnd = fogStep = fogSpeed = 0;
  actors[0] = CreateActor((ActorParams) {
    .modelName = "IDR_ISLAND",
    .textureName = "IDR_ISLAND_TEX",
    .position = (Vertex) { .x = 0, .y = -6.6, .z = 0 },
    .rotation = (Vertex) { .x = D3DX_PI, .y = 0, .z = 0 },
    .scale = (Vertex) { .x = 1, .y = 1, .z = 1 },
    .Start = NULL,
    .Update = NULL
  });
  actors[1] = CreateActor((ActorParams) {
    .modelName = "IDR_SKY",
    .textureName = "IDR_SKY_TEX",
    .audioName = "IDR_WIND",
    .position = (Vertex) { .x = 0, .y = 0, .z = 0 },
    .rotation = (Vertex) { .x = 0, .y = 0, .z = 0 },
    .scale = (Vertex) { .x = 1, .y = 1, .z = 1 },
    .Start = SkyStart,
    .Update = SkyUpdate,
    .audioParams = (AudioParams) { .shouldLoop = TRUE }
  });
  actors[2] = CreateActor((ActorParams) {
    .modelName = "IDR_BIRD_HEAD",
    .textureName = "IDR_ISLAND_TEX",
    .audioName = "IDR_CROW",
    .position = (Vertex) { .x = 0, .y = 0, .z = 0 },
    .rotation = (Vertex) { .x = 0, .y = 0, .z = 0 },
    .scale = (Vertex) { .x = 1, .y = 1, .z = 1 },
    .Start = CrowStart,
    .Update = CrowUpdate
  });

  LPD3DXBUFFER errorlog;
  D3DXCreateEffectFromResource(d3ddev, 0, "IDR_DIFFUSE_FX", 0, 0, 0, 0,
    &effect, &errorlog);
  if(errorlog) Log((char*)errorlog->lpVtbl->GetBufferPointer(errorlog));
  effect->lpVtbl->FindNextValidTechnique(effect, NULL, &technique);
  textureHandle = effect->lpVtbl->GetParameterByName(effect, NULL, "Texture");
  fogStartHandle = effect->lpVtbl->GetParameterByName(effect, NULL, "FogStart");
  fogEndHandle = effect->lpVtbl->GetParameterByName(effect, NULL, "FogEnd");
  cameraPosHandle = effect->lpVtbl->GetParameterByName(effect, NULL, "CameraPos");
}

void RenderScene(float deltaTime)
{
  ID3DXMatrixStack *stack;
  D3DXCreateMatrixStack(0, &stack);
  D3DXMATRIX camMat = CameraViewMatrix();
  effect->lpVtbl->SetMatrix(effect, "Projection", &viewMat);
  effect->lpVtbl->SetMatrix(effect, "View", &camMat);
  stack->lpVtbl->LoadMatrix(stack, &camMat);
  //d3ddev->lpVtbl->SetTransform(d3ddev, D3DTS_VIEW, stack->lpVtbl->GetTop(stack));
  d3ddev->lpVtbl->Clear(d3ddev, 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
    D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
  d3ddev->lpVtbl->BeginScene(d3ddev);

  if (fogStep == randomFogEnd)
  {
    int fogDistances[12] = {
        17, 18, 18, 18, 18, 19, 200, 200, 200, 200, 200, 200
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
  effect->lpVtbl->SetFloat(effect, fogStartHandle, 1.0f);
  effect->lpVtbl->SetFloat(effect, fogEndHandle, fogStep);
  effect->lpVtbl->SetVector(effect, cameraPosHandle,
    &(D3DXVECTOR4){ cameraPos.x, cameraPos.y, cameraPos.z, 1 });

  for(int i = 0; i < ACTOR_COUNT; i++)
  {
    DrawActor(actors[i], stack, d3ddev, effect, textureHandle, deltaTime);
  }

  d3ddev->lpVtbl->EndScene(d3ddev);
  d3ddev->lpVtbl->Present(d3ddev, NULL, NULL, NULL, NULL);
}

void CleanScene()
{
  effect->lpVtbl->Release(effect);
  for(int i = 0; i < ACTOR_COUNT; i++)
  {
    DeleteActor(actors[i]);
  }
}
