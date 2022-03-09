#include "scene.h"
#include "behaviors\arrival.h"
#include "behaviors\bush.h"
#include "behaviors\crow.h"
#include "behaviors\crown.h"
#include "behaviors\player.h"
#include "behaviors\rings.h"
#include "behaviors\sign.h"
#include "behaviors\sky.h"
#include "srandom.h"

extern __inline HRESULT XAudio2CreateVolumeMeter(_Outptr_ IUnknown **ppApo);

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
                                .effectName = "IDR_COLOR_FX",
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
                                .effectName = "IDR_COLOR_FX",
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
                                .effectName = "IDR_COLOR_FX",
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
                                .effectName = "IDR_COLOR_FX",
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
                                .effectName = "IDR_COLOR_FX",
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

  actors[17] = CreateActor((ActorParams){
      .name = "sign bg",
      .enabled = 0,
      .bufferType = Triangle,
      .modelName = "IDR_SIGN_BG",
      .effectName = "IDR_COLOR_FX",
      .position = (Vertex){.x = 0, .y = 0.85f, .z = 0},
      .rotation = (Vertex){.x = 0, .y = D3DX_PI / 2, .z = 0},
      .scale = (Vertex){.x = 0.45f, .y = 1, .z = 0.6f},
      .screenSpaceOnly = 1,
      .Start = SignStart,
  });

  actors[18] =
      CreateActor((ActorParams){.name = "sign location",
                                .enabled = 0,
                                .bufferType = Triangle,
                                .modelName = "IDR_SIGN_LOCATION",
                                .position = (Vertex){.x = 0, .y = -6.5, .z = 0},
                                .rotation = (Vertex){.x = 0, .y = 0, .z = 0},
                                .scale = (Vertex){.x = 1, .y = 1, .z = 1},
                                .Update = SignUpdate});
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
