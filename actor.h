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
};

struct actorProps
{
  char *modelName;
  char *textureName;
  struct vertex position;
  struct vertex rotation;
};

struct actor *CreateActor(struct actorProps props);

void DeleteActor(struct actor *actor);

#endif
