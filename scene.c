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
  seed_smooth_rand();
  randomFogEnd = fogStep = fogSpeed = 0;
  PlayAudio(self->audioSource, self->audioBuffer);
}

void SkyUpdate(Actor *self, float deltaTime)
{
  self->rotation.x += 0.1f * deltaTime;
  if (fogStep == randomFogEnd)
  {
    int fogDistances[12] = {
        10, 10, 10, 20, 20, 20, 200, 200, 200, 200, 200, 200
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
  RenderSettings.fogEnd = fogStep;
}

void InitScene()
{
  actors[0] = CreateActor((ActorParams) {
    .modelName = "IDR_ISLAND",
    .textureName = "IDR_ISLAND_TEX",
    .effectName = "IDR_DIFFUSE_FX",
    .position = (Vertex) { .x = 0, .y = -6.6, .z = 0 },
    .rotation = (Vertex) { .x = D3DX_PI, .y = 0, .z = 0 },
    .scale = (Vertex) { .x = 1, .y = 1, .z = 1 }
  });
  actors[1] = CreateActor((ActorParams) {
    .modelName = "IDR_SKY",
    .textureName = "IDR_SKY_TEX",
    .audioName = "IDR_WIND",
    .effectName = "IDR_DIFFUSE_FX",
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
    .effectName = "IDR_DIFFUSE_FX",
    .position = (Vertex) { .x = 0, .y = 0, .z = 0 },
    .rotation = (Vertex) { .x = 0, .y = 0, .z = 0 },
    .scale = (Vertex) { .x = 1, .y = 1, .z = 1 },
    .Start = CrowStart,
    .Update = CrowUpdate
  });
}

void RenderScene(float deltaTime)
{
  d3ddev->lpVtbl->Clear(d3ddev, 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
    D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
  d3ddev->lpVtbl->BeginScene(d3ddev);

  for (int i = 0; i < ACTOR_COUNT; i++)
  {
    DrawActor(actors[i], d3ddev, deltaTime);
  }

  d3ddev->lpVtbl->EndScene(d3ddev);
  d3ddev->lpVtbl->Present(d3ddev, NULL, NULL, NULL, NULL);
}

void CleanScene()
{
  for (int i = 0; i < ACTOR_COUNT; i++)
  {
    DeleteActor(actors[i]);
  }
}
