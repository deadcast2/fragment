#ifndef _ACTOR_H_
#define _ACTOR_H_

#include <d3dx9.h>
#include "vendor/XAudio2_custom.h"

struct actor {
  IXAudio2SourceVoice *audioSource;
  IDirect3DVertexBuffer9 *vertexBuffer;
  IDirect3DTexture9 *texture;
  struct vertex *position;
  struct vertex *rotation;
};

#endif
