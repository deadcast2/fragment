#include "crow.h"
#include "..\srandom.h"

void CrowUpdate(Actor *self, float deltaTime) {
  static float target = 0;
  static float nextCaw = 7.5f;

  if (nextCaw < 0) {
    PlayAudio(self->audioSource, self->audioBuffer, 0.7f);
    nextCaw = smooth_rand() % 20;

    if (nextCaw > 0 && nextCaw <= 5)
      target = 0.5f;
    else if (nextCaw > 5 && nextCaw <= 15)
      target = -0.5f;
    else
      target = 0;
  }

  nextCaw -= deltaTime;

  const int sign = target < 0 ? -1 : 1;

  if ((sign * self->rotation.x) < (sign * target)) {
    const float origX = self->position.x;
    const float origY = self->position.y;
    const float origZ = self->position.z;

    self->rotation.x += sign * 2.5f * deltaTime;

    self->position.x = origX;
    self->position.y = origY;
    self->position.z = origZ;
  }
}
