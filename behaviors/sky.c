#include "sky.h"
#include "..\srandom.h"
#include <float.h>

float currWindSpeed = 0;

void SkyStart(Actor *self) {
  seed_smooth_rand();
  PlayAudio(self->audioSource, self->audioBuffer, 1.0f);
  IUnknown *volumeMeter;

  XAudio2CreateVolumeMeter(&volumeMeter);

  XAUDIO2_EFFECT_DESCRIPTOR effectDesc = {
      .pEffect = volumeMeter, .InitialState = TRUE, .OutputChannels = 1};
  XAUDIO2_EFFECT_CHAIN effectChain = {.EffectCount = 1,
                                      .pEffectDescriptors = &effectDesc};
  self->audioSource->lpVtbl->SetEffectChain(self->audioSource, &effectChain);
  volumeMeter->lpVtbl->Release(volumeMeter);
}

void SkyUpdate(Actor *self, float deltaTime) {
  const int fogStart = 400;
  static float randomFogEnd = fogStart, lastFogEnd = fogStart;
  static float fogTime = 0, fogStep = fogStart, fogSpeed = 0;

  self->rotation.x += 0.1f * deltaTime;

  if (_fabs(fogStep - randomFogEnd) <= FLT_EPSILON) {
    int fogDistances[11] = {15, 15, 20, 20, 20, 20, 200, 200, 200, 200, 200};
    float fogSpeeds[5] = {0.1, 0.1, 0.15, 0.15, 0.08};
    lastFogEnd = randomFogEnd;
    randomFogEnd = fogDistances[smooth_rand() % 11];
    fogSpeed = fogSpeeds[smooth_rand() % 5];
    fogTime = 0;
  }

  fogStep =
      smooth_inter(lastFogEnd, randomFogEnd, fogTime += deltaTime * fogSpeed);
  RenderSettings.fogEnd = fogStep;

  static float windEnd = 0, lastWindEnd = 0;
  static float windTime = 0;

  XAUDIO2FX_VOLUMEMETER_LEVELS effectParams = {
      .pPeakLevels =
          HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(float)),
      .ChannelCount = 1};

  self->audioSource->lpVtbl->GetEffectParameters(
      self->audioSource, 0, &effectParams, sizeof(effectParams));

  if (_fabs(currWindSpeed - windEnd) <= FLT_EPSILON) {
    lastWindEnd = windEnd;
    windEnd = (float)(*effectParams.pPeakLevels) * 20;
    windTime = 0;
  }

  currWindSpeed =
      smooth_inter(lastWindEnd, windEnd, windTime += deltaTime * windEnd);
  HeapFree(GetProcessHeap(), 0, effectParams.pPeakLevels);
}
