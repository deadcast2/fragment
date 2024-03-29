#include "player.h"
#include "..\scene.h"
#include "..\srandom.h"
#include "..\vec3.h"
#include "arrival.h"

void PlayerStart(Actor *self) { SetNearClip(0.1f); }

void PlayerUpdate(Actor *self, float deltaTime) {
  static D3DXVECTOR3 prevCameraPos;
  static BOOL walkingOnLid = FALSE;
  static BOOL shouldFart = TRUE;
  static float timeToFart = 0;

  if (arriving) {
    // Keep cursor locked in place until done in case player is moving the
    // mouse.
    SetCursorPos(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

    return;
  }

  D3DXVECTOR3 velocity = Vec3_Sub(&prevCameraPos, &cameraPos);

  // Determine if a footstep sound should play.
  if (Vec3_LenSq(&velocity) > 1.2f) {
    if (walkingOnLid)
      PlayAudio(actors[15]->audioSource, actors[15]->audioBuffer, 0.2f);
    else
      PlayAudio(self->audioSource, self->audioBuffer, 0.5f);

    prevCameraPos = cameraPos;

    // Keep resetting fart countdown when player moves.
    timeToFart = 3000 * deltaTime;
  }

  if (shouldFart && timeToFart < 0) {
    // Give the player a little gas...
    PlayAudio(actors[16]->audioSource, actors[16]->audioBuffer, 0.8f);
    shouldFart = FALSE;
  } else if (shouldFart) {
    timeToFart -= deltaTime;
  }

  const BOOL isLongitudinal =
      GetAsyncKeyState('W') || GetAsyncKeyState(VK_UP) ||
      GetAsyncKeyState('S') || GetAsyncKeyState(VK_DOWN);
  const BOOL isLateral = GetAsyncKeyState('A') || GetAsyncKeyState('D');

  // Handle forward and backward movement with collision.
  if (isLongitudinal || isLateral) {
    D3DXVECTOR3 newPos = cameraPos;

    if (isLongitudinal) {
      const int movementSign =
          GetAsyncKeyState('W') || GetAsyncKeyState(VK_UP) ? 1 : -1;
      const float units = movementSign * 2.0f * deltaTime;

      newPos.x += cameraForwardNoPitch.x * units;
      newPos.y += cameraForwardNoPitch.y * units;
      newPos.z += cameraForwardNoPitch.z * units;
    }

    if (isLateral) {
      const int movementSign = GetAsyncKeyState('D') ? 1 : -1;
      const float units = movementSign * 2.0f * deltaTime;

      newPos.x += cameraRight.x * units;
      newPos.y += cameraRight.y * units;
      newPos.z += cameraRight.z * units;
    }

    D3DXVECTOR3 newVelocity;
    D3DXVec3Subtract(&newVelocity, &newPos, &cameraPos);

    CollisionPacket packet;
    // The dimensions of the player.
    packet.eRadius = (D3DXVECTOR3){0.5f, 1.8f, 0.5f};

    D3DXVECTOR3 gravity = (D3DXVECTOR3){0, -0.1f, 0};
    cameraPos =
        Collision_CollideAndSlide(&packet, &cameraPos, &newVelocity, &gravity);

    // Activate the walking on lid sfx.
    walkingOnLid = packet.actorIndex == 15 ? TRUE : FALSE;
  }

  // Handle turning
  if (GetAsyncKeyState(VK_LEFT))
    CameraYaw(-2.0f * deltaTime);

  if (GetAsyncKeyState(VK_RIGHT))
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
  CameraPitch(mouseSmoothY * 0.05f * deltaTime, 1);

  SetCursorPos(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
  GetCursorPos(&lastMousePos);
}
