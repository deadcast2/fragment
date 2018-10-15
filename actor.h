#ifndef _ACTOR_H_
#define _ACTOR_H_

#include <d3dx9.h>
#include "vendor/XAudio2_custom.h"
#include "model.h"
#include "texture.h"
#include "audio.h"

typedef struct Actor
{
  IXAudio2SourceVoice *audioSource;
  XAUDIO2_BUFFER *audioBuffer;
  IDirect3DVertexBuffer9 *vertexBuffer;
  IDirect3DTexture9 *d3dTexture;
  int vertexCount;
  Vertex position;
  Vertex rotation;
  Vertex scale;
  void (*Update)(struct Actor *, float);
} Actor;

typedef struct ActorParams
{
  char *modelName;
  char *textureName;
  char *audioName;
  Vertex position;
  Vertex rotation;
  Vertex scale;
  AudioParams audioParams;
  void (*Start)(struct Actor *);
  void *Update;
} ActorParams;

Actor *CreateActor(ActorParams params);
void DeleteActor(Actor *actor);
void DrawActor(Actor *actor, ID3DXMatrixStack *stack,
  LPDIRECT3DDEVICE9 d3ddev, float deltaTime);

#endif
