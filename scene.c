#include "scene.h"
#include "behaviors\crow.h"
#include "behaviors\rings.h"
#include "behaviors\sky.h"
#include "srandom.h"

extern __inline HRESULT XAudio2CreateVolumeMeter(_Outptr_ IUnknown **ppApo);

static int arriving = 1;

void BushStart(Actor *self) {
  self->effect->lpVtbl->SetBool(self->effect, "_IsFoliage", TRUE);
  self->effect->lpVtbl->SetFloat(self->effect, "_BendScale", 0.08f);
}

void BushUpdate(Actor *self) {
  self->effect->lpVtbl->SetFloat(self->effect, "_WindSpeed", currWindSpeed);
}

void CrownStart(Actor *self) {
  self->effect->lpVtbl->SetBool(self->effect, "_IsFoliage", TRUE);
  self->effect->lpVtbl->SetFloat(self->effect, "_BendScale", 0.08f);
}

void CrownUpdate(Actor *self) {
  self->effect->lpVtbl->SetFloat(self->effect, "_WindSpeed",
                                 currWindSpeed * 4.0f);
}

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

  if (GetAsyncKeyState('W') || GetAsyncKeyState(VK_UP) ||
      GetAsyncKeyState('S') || GetAsyncKeyState(VK_DOWN)) {
    const int movementSign =
        GetAsyncKeyState('W') || GetAsyncKeyState(VK_UP) ? 1 : -1;
    const float units = movementSign * 2.0f * deltaTime;

    D3DXVECTOR3 newPos;
    newPos.x = cameraPos.x + cameraForward.x * units;
    newPos.y = cameraPos.y + cameraForward.y * units;
    newPos.z = cameraPos.z + cameraForward.z * units;

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

  if (GetAsyncKeyState('A') || GetAsyncKeyState(VK_LEFT))
    CameraYaw(-2.0f * deltaTime);

  if (GetAsyncKeyState('D') || GetAsyncKeyState(VK_RIGHT))
    CameraYaw(2.0f * deltaTime);
}

void InitScene() {
  actors[0] =
      CreateActor((ActorParams){.name = "island",
                                .enabled = 0,
                                .bufferType = Triangle,
                                .modelName = "IDR_ISLAND",
                                .textureName = "IDR_ISLAND_TEX",
                                .effectName = "IDR_DIFFUSE_FX",
                                .position = (Vertex){.x = 0, .y = -6.5, .z = 0},
                                .rotation = (Vertex){.x = 0, .y = 0, .z = 0},
                                .scale = (Vertex){.x = 1, .y = 1, .z = 1}});

  actors[1] = CreateActor(
      (ActorParams){.name = "sky",
                    .enabled = 0,
                    .bufferType = Triangle,
                    .modelName = "IDR_SKY",
                    .textureName = "IDR_SKY_TEX",
                    .audioName = "IDR_WIND",
                    .effectName = "IDR_DIFFUSE_FX",
                    .position = (Vertex){.x = 0, .y = 0, .z = 0},
                    .rotation = (Vertex){.x = 0, .y = 0, .z = 0},
                    .scale = (Vertex){.x = 1, .y = 1, .z = 1},
                    .Start = SkyStart,
                    .Update = SkyUpdate,
                    .audioParams = (AudioParams){.shouldLoop = TRUE}});

  actors[2] =
      CreateActor((ActorParams){.name = "bush",
                                .enabled = 0,
                                .bufferType = Triangle,
                                .modelName = "IDR_BUSH",
                                .textureName = "IDR_ISLAND_TEX",
                                .effectName = "IDR_DIFFUSE_FX",
                                .position = (Vertex){.x = 0, .y = -6.5, .z = 0},
                                .rotation = (Vertex){.x = 0, .y = 0, .z = 0},
                                .scale = (Vertex){.x = 1, .y = 1, .z = 1},
                                .Start = BushStart,
                                .Update = BushUpdate});

  actors[3] = CreateActor(
      (ActorParams){.name = "crown left",
                    .enabled = 0,
                    .bufferType = Triangle,
                    .modelName = "IDR_CROWN_LEFT",
                    .textureName = "IDR_ISLAND_TEX",
                    .effectName = "IDR_DIFFUSE_FX",
                    .position = (Vertex){.x = 0.3, .y = -7.4, .z = 0},
                    .rotation = (Vertex){.x = 0, .y = 0, .z = 0},
                    .scale = (Vertex){.x = 1, .y = 1, .z = 1},
                    .Start = CrownStart,
                    .Update = CrownUpdate});

  actors[4] = CreateActor(
      (ActorParams){.name = "crown right",
                    .enabled = 0,
                    .bufferType = Triangle,
                    .modelName = "IDR_CROWN_RIGHT",
                    .textureName = "IDR_ISLAND_TEX",
                    .effectName = "IDR_DIFFUSE_FX",
                    .position = (Vertex){.x = 0.1, .y = -6.6, .z = -0.1},
                    .rotation = (Vertex){.x = 0, .y = 0, .z = 0},
                    .scale = (Vertex){.x = 1, .y = 1, .z = 1},
                    .Start = CrownStart,
                    .Update = CrownUpdate});

  actors[5] = CreateActor(
      (ActorParams){.name = "crow head",
                    .enabled = 0,
                    .bufferType = Triangle,
                    .modelName = "IDR_HEAD",
                    .textureName = "IDR_ISLAND_TEX",
                    .audioName = "IDR_CROW",
                    .effectName = "IDR_DIFFUSE_FX",
                    .position = (Vertex){.x = -1.078, .y = 7.13, .z = -0.73},
                    .rotation = (Vertex){.x = 0, .y = 0, .z = 0},
                    .scale = (Vertex){.x = 1, .y = 1, .z = 1},
                    .Update = CrowUpdate});

  actors[6] = CreateActor((ActorParams){.name = "arrival",
                                        .enabled = 0,
                                        .audioName = "IDR_GONG",
                                        .Start = ArrivalStart,
                                        .Update = ArrivalUpdate});

  actors[7] =
      CreateActor((ActorParams){.name = "ring 1",
                                .enabled = 1,
                                .bufferType = Line,
                                .modelName = "IDR_RING",
                                .effectName = "IDR_DIFFUSE_FX",
                                .position = (Vertex){.x = 0, .y = 0, .z = 0},
                                .rotation = (Vertex){.x = 0, .y = 0, .z = 0},
                                .scale = (Vertex){.x = 1, .y = 1, .z = 1},
                                .Start = RingStart,
                                .Update = Ring1Update});

  actors[8] =
      CreateActor((ActorParams){.name = "ring 2",
                                .enabled = 1,
                                .bufferType = Line,
                                .modelName = "IDR_RING",
                                .effectName = "IDR_DIFFUSE_FX",
                                .position = (Vertex){.x = 0, .y = 0, .z = 0},
                                .rotation = (Vertex){.x = 0, .y = 0, .z = 0},
                                .scale = (Vertex){.x = 1, .y = 1, .z = 1},
                                .Start = RingStart,
                                .Update = Ring2Update});

  actors[9] =
      CreateActor((ActorParams){.name = "ring 3",
                                .enabled = 1,
                                .bufferType = Line,
                                .modelName = "IDR_RING",
                                .effectName = "IDR_DIFFUSE_FX",
                                .position = (Vertex){.x = 0, .y = 0, .z = 0},
                                .rotation = (Vertex){.x = 0, .y = 0, .z = 0},
                                .scale = (Vertex){.x = 1, .y = 1, .z = 1},
                                .Start = RingStart,
                                .Update = Ring3Update});

  actors[10] =
      CreateActor((ActorParams){.name = "ring 4",
                                .enabled = 1,
                                .bufferType = Line,
                                .modelName = "IDR_RING",
                                .effectName = "IDR_DIFFUSE_FX",
                                .position = (Vertex){.x = 0, .y = 0, .z = 0},
                                .rotation = (Vertex){.x = 0, .y = 0, .z = 0},
                                .scale = (Vertex){.x = 1, .y = 1, .z = 1},
                                .Start = RingStart,
                                .Update = Ring4Update});

  actors[11] =
      CreateActor((ActorParams){.name = "ring 5",
                                .enabled = 1,
                                .bufferType = Line,
                                .modelName = "IDR_RING",
                                .effectName = "IDR_DIFFUSE_FX",
                                .position = (Vertex){.x = 0, .y = 0, .z = 0},
                                .rotation = (Vertex){.x = 0, .y = 0, .z = 0},
                                .scale = (Vertex){.x = 1, .y = 1, .z = 1},
                                .Start = RingStart,
                                .Update = Ring5Update});

  actors[12] = CreateActor((ActorParams){.name = "player",
                                         .enabled = 0,
                                         .audioName = "IDR_FOOTSTEP",
                                         .Update = PlayerUpdate});

  actors[13] = CreateActor((ActorParams){.name = "ring gong",
                                         .enabled = 1,
                                         .audioName = "IDR_RING_GONG",
                                         .Update = RingGongUpdate});

  actors[14] =
      CreateActor((ActorParams){.name = "boundary",
                                .enabled = 0,
                                .bufferType = Triangle,
                                .modelName = "IDR_BOUNDARY",
                                .position = (Vertex){.x = 0, .y = -6.5, .z = 0},
                                .rotation = (Vertex){.x = 0, .y = 0, .z = 0},
                                .scale = (Vertex){.x = 1, .y = 1, .z = 1}});

  actors[15] =
      CreateActor((ActorParams){.name = "lid",
                                .enabled = 0,
                                .bufferType = Triangle,
                                .modelName = "IDR_LID",
                                .audioName = "IDR_FOOTSTEP_METAL",
                                .position = (Vertex){.x = 0, .y = -6.5, .z = 0},
                                .rotation = (Vertex){.x = 0, .y = 0, .z = 0},
                                .scale = (Vertex){.x = 1, .y = 1, .z = 1}});

  actors[16] = CreateActor(
      (ActorParams){.name = "fart", .enabled = 0, .audioName = "IDR_FART"});
}

void RenderScene(float deltaTime) {
  d3ddev->lpVtbl->Clear(d3ddev, 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                        D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
  d3ddev->lpVtbl->BeginScene(d3ddev);

  for (int i = 0; i < ACTOR_COUNT; i++) {
    DrawActor(actors[i], d3ddev, deltaTime);
  }

  d3ddev->lpVtbl->EndScene(d3ddev);
  d3ddev->lpVtbl->Present(d3ddev, NULL, NULL, NULL, NULL);
}

void CleanScene() {
  for (int i = 0; i < ACTOR_COUNT; i++) {
    DeleteActor(actors[i]);
  }
}
