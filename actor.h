#ifndef _ACTOR_H_
#define _ACTOR_H_

#include <d3dx9.h>
#include "vendor/XAudio2_custom.h"
#include "model.h"
#include "texture.h"

struct actor
{
  IXAudio2SourceVoice *audioSource;
  IDirect3DVertexBuffer9 *vertexBuffer;
  IDirect3DTexture9 *d3dTexture;
  int vertexCount;
  struct vertex position;
  struct vertex rotation;
  struct vertex scale;
  void (*update)(struct actor *, float);
};

struct actorProps
{
  char *modelName;
  char *textureName;
  struct vertex position;
  struct vertex rotation;
  struct vertex scale;
  void *update;
};

struct actor *CreateActor(struct actorProps props);
void DeleteActor(struct actor *actor);
void DrawActor(struct actor *actor, ID3DXMatrixStack *stack,
  LPDIRECT3DDEVICE9 d3ddev, float deltaTime);

#endif
