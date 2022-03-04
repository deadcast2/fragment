#include "arrival.h"

int arriving = 1;

void ArrivalStart(Actor *self) {
  CameraPosition((D3DXVECTOR3){0, 50, -3.5f});
  CameraYaw(D3DX_PI);
  CameraPitch(D3DX_PI / 2.0f);
  PlayAudio(self->audioSource, self->audioBuffer, 1.0f);
}

void ArrivalUpdate(Actor *self, float deltaTime) {
  static float pitch = 0.0f;
  static float yaw = 0.0f;
  static float yAxis = 0.0f;

  if (pitch < D3DX_PI / 2.0f) {
    const float angle = (D3DX_PI / 20.0f) * deltaTime;
    pitch += angle;
    CameraPitch(-angle);
  }

  if (yaw < D3DX_PI) {
    const float angle = (D3DX_PI / 10.0f) * deltaTime;
    yaw += angle;
    CameraYaw(angle);
  }

  if (yAxis > -50.0f) {
    const float units = 5.0f * deltaTime;
    yAxis -= units;
    CameraFly(-units);
  } else {
    arriving = 0;
    self->enabled = 0;
  }
}
