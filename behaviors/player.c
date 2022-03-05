#include "player.h"
#include "..\scene.h"
#include "..\srandom.h"
#include "arrival.h"

void PlayerUpdate(Actor *self, float deltaTime) {
  static float lastX = -100;
  static float lastZ = -100;
  static int walkingOnLid = 0;
  static float timeToFart = 0;
  static int shouldFart = 1;

  if (arriving)
    return;

  float xDiff = lastX - cameraPos.x;
  float zDiff = lastZ - cameraPos.z;
  float dist = sqrt((xDiff * xDiff) + (zDiff * zDiff));

  // Determine if a footstep sound should play
  if (dist > 1.15f) {
    if (walkingOnLid)
      PlayAudio(actors[15]->audioSource, actors[15]->audioBuffer, 0.2f);
    else
      PlayAudio(self->audioSource, self->audioBuffer, 0.5f);

    lastX = cameraPos.x;
    lastZ = cameraPos.z;
    timeToFart = 2000 * deltaTime;
  }

  if (shouldFart && timeToFart < 0) {
    // Give the player a little gas...
    PlayAudio(actors[16]->audioSource, actors[16]->audioBuffer, 0.8f);
    shouldFart = 0;
  } else if (shouldFart) {
    timeToFart -= deltaTime;
  }

  // Handle forward and backward movement with collision
  if (GetAsyncKeyState('W') || GetAsyncKeyState(VK_UP) ||
      GetAsyncKeyState('S') || GetAsyncKeyState(VK_DOWN)) {
    const int movementSign =
        GetAsyncKeyState('W') || GetAsyncKeyState(VK_UP) ? 1 : -1;
    const float units = movementSign * 2.0f * deltaTime;

    D3DXVECTOR3 newPos;
    newPos.x = cameraPos.x + cameraForwardNoPitch.x * units;
    newPos.y = cameraPos.y + cameraForwardNoPitch.y * units;
    newPos.z = cameraPos.z + cameraForwardNoPitch.z * units;

    D3DXVECTOR3 velocity;
    D3DXVec3Subtract(&velocity, &newPos, &cameraPos);

    CollisionPacket packet;
    packet.eRadius = (D3DXVECTOR3){0.5f, 1.6f, 0.5f};
    D3DXVECTOR3 gravity = (D3DXVECTOR3){0, -0.1f, 0};
    cameraPos =
        Collision_CollideAndSlide(&packet, &cameraPos, &velocity, &gravity);

    // Activate the walking on lid sfx.
    walkingOnLid = packet.actorIndex == 15 ? 1 : 0;
  }

  // Handle turning
  if (GetAsyncKeyState('A') || GetAsyncKeyState(VK_LEFT))
    CameraYaw(-2.0f * deltaTime);

  if (GetAsyncKeyState('D') || GetAsyncKeyState(VK_RIGHT))
    CameraYaw(2.0f * deltaTime);

  // Handle mouse movement
  POINT currMousePos;
  GetCursorPos(&currMousePos);
  static POINT lastMousePos = (POINT){SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
  static float mouseSmoothX = 0;
  static float mouseSmoothY = 0;

  mouseSmoothX = smooth_inter(mouseSmoothX, currMousePos.x - lastMousePos.x,
                              deltaTime * 50.0f);
  mouseSmoothY = smooth_inter(mouseSmoothY, currMousePos.y - lastMousePos.y,
                              deltaTime * 50.0f);

  CameraYaw(mouseSmoothX * 0.05f * deltaTime);
  CameraPitch(mouseSmoothY * 0.05f * deltaTime);

  SetCursorPos(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
  GetCursorPos(&lastMousePos);
}
