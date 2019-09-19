#include "scene.h"

extern __inline HRESULT XAudio2CreateVolumeMeter(_Outptr_ IUnknown** ppApo);

static float currWindSpeed = 0;

void CrowStart(Actor *self)
{
  PlayAudio(self->audioSource, self->audioBuffer);
}

void CrowUpdate(Actor *self, float deltaTime)
{
  static float nextCaw = 0;
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
  PlayAudio(self->audioSource, self->audioBuffer);
  IUnknown *volumeMeter;
  if(XAudio2CreateVolumeMeter(&volumeMeter) == S_OK)
  {
    XAUDIO2_EFFECT_DESCRIPTOR effectDesc = {
      .pEffect = volumeMeter,
      .InitialState = TRUE,
      .OutputChannels = 1
    };
    XAUDIO2_EFFECT_CHAIN effectChain = {
      .EffectCount = 1,
      .pEffectDescriptors = &effectDesc
    };
    self->audioSource->lpVtbl->SetEffectChain(self->audioSource, &effectChain);
    volumeMeter->lpVtbl->Release(volumeMeter);
  }
}

void SkyUpdate(Actor *self, float deltaTime)
{
  const int fogStart = 400;
  static float randomFogEnd = fogStart, lastFogEnd = fogStart;
  static float fogTime = 0, fogStep = fogStart, fogSpeed = 0;

  self->rotation.x += 0.1f * deltaTime;
  if (_fabs(fogStep - randomFogEnd) <= FLT_EPSILON)
  {
    int fogDistances[11] = { 15, 15, 20, 20, 20, 20, 200, 200, 200, 200, 200 };
    float fogSpeeds[5] = { 0.1, 0.1, 0.15, 0.15, 0.08 };
    lastFogEnd = randomFogEnd;
    randomFogEnd = fogDistances[smooth_rand() % 11];
    fogSpeed = fogSpeeds[smooth_rand() % 5];
    fogTime = 0;
  }
  fogStep = smooth_inter(lastFogEnd, randomFogEnd, fogTime += deltaTime * fogSpeed);
  RenderSettings.fogEnd = fogStep;

  static float windEnd = 0, lastWindEnd = 0;
  static float windTime = 0;

  XAUDIO2FX_VOLUMEMETER_LEVELS effectParams = {
    .pPeakLevels = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(float)),
    .ChannelCount = 1
  };
  self->audioSource->lpVtbl->GetEffectParameters(self->audioSource, 0,
    &effectParams, sizeof(effectParams));
  if (_fabs(currWindSpeed - windEnd) <= FLT_EPSILON)
  {
    lastWindEnd = windEnd;
    windEnd = (float)(*effectParams.pPeakLevels) * 20;
    windTime = 0;
  }
  currWindSpeed = smooth_inter(lastWindEnd, windEnd, windTime += deltaTime * windEnd);
  HeapFree(GetProcessHeap(), 0, effectParams.pPeakLevels);
}

void BushStart(Actor *self)
{
  self->effect->lpVtbl->SetBool(self->effect, "_IsFoliage", TRUE);
  self->effect->lpVtbl->SetFloat(self->effect, "_BendScale", 0.05f);
}

void BushUpdate(Actor *self)
{
  self->effect->lpVtbl->SetFloat(self->effect, "_WindSpeed", currWindSpeed);
}

void CrownStart(Actor *self)
{
  self->effect->lpVtbl->SetBool(self->effect, "_IsFoliage", TRUE);
  self->effect->lpVtbl->SetFloat(self->effect, "_BendScale", 0.1f);
}

void CrownUpdate(Actor *self)
{
  self->effect->lpVtbl->SetFloat(self->effect, "_WindSpeed", currWindSpeed * 2.0f);
}

void InitScene()
{
  actors[0] = CreateActor((ActorParams) {
    .modelName = "IDR_ISLAND",
    .textureName = "IDR_ISLAND_TEX",
    .effectName = "IDR_DIFFUSE_FX",
    .position = (Vertex) { .x = 0, .y = -6.5, .z = 0 },
    .rotation = (Vertex) { .x = 0, .y = 0, .z = 0 },
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
    .modelName = "IDR_BUSH",
    .textureName = "IDR_ISLAND_TEX",
    .effectName = "IDR_DIFFUSE_FX",
    .position = (Vertex) { .x = -1.14, .y = -1.2, .z = -1.24 },
    .rotation = (Vertex) { .x = -D3DX_PI/4, .y = 0, .z = 0 },
    .scale = (Vertex) { .x = 1, .y = 1, .z = 1 },
    .Start = BushStart,
    .Update = BushUpdate
  });
  actors[3] = CreateActor((ActorParams) {
    .modelName = "IDR_CROWN_LEFT",
    .textureName = "IDR_ISLAND_TEX",
    .effectName = "IDR_DIFFUSE_FX",
    .position = (Vertex) { .x = 0, .y = -6.7, .z = 0 },
    .rotation = (Vertex) { .x = 0, .y = 0, .z = 0 },
    .scale = (Vertex) { .x = 1, .y = 1, .z = 1 },
    .Start = CrownStart,
    .Update = CrownUpdate
  });
  actors[4] = CreateActor((ActorParams) {
    .modelName = "IDR_CROWN_RIGHT",
    .textureName = "IDR_ISLAND_TEX",
    .effectName = "IDR_DIFFUSE_FX",
    .position = (Vertex) { .x = 0, .y = -6.7, .z = 0 },
    .rotation = (Vertex) { .x = 0, .y = 0, .z = 0 },
    .scale = (Vertex) { .x = 1, .y = 1, .z = 1 },
    .Start = CrownStart,
    .Update = CrownUpdate
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
