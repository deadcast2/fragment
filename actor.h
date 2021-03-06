#ifndef _ACTOR_H_
#define _ACTOR_H_

#include <d3dx9.h>
#include "camera.h"
#include "model.h"
#include "texture.h"
#include "audio.h"
#include "effect.h"

enum ActorBufferType { Line, Triangle };

typedef struct Actor
{
  enum ActorBufferType bufferType;
  IXAudio2SourceVoice *audioSource;
  XAUDIO2_BUFFER *audioBuffer;
  IDirect3DVertexBuffer9 *vertexBuffer;
  IDirect3DTexture9 *d3dTexture;
  LPD3DXEFFECT effect;
  int vertexCount;
  Vertex *vertices;
  Vertex position;
  Vertex rotation;
  Vertex scale;
  int enabled;
  char *name;
  void (*Start)(struct Actor *);
  void (*Update)(struct Actor *, float);
} Actor;

typedef struct ActorParams
{
  enum ActorBufferType bufferType;
  char *modelName;
  char *textureName;
  char *audioName;
  char *effectName;
  Vertex position;
  Vertex rotation;
  Vertex scale;
  int enabled;
  char *name;
  AudioParams audioParams;
  void *Start;
  void *Update;
} ActorParams;

Actor *CreateActor(ActorParams params);
void DeleteActor(Actor *actor);
void DrawActor(Actor *actor, LPDIRECT3DDEVICE9 d3ddev, float deltaTime);

#endif
