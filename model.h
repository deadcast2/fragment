#ifndef _MODEL_H_
#define _MODEL_H_

#include <windows.h>
#include <stdio.h>
#include "vendor/fastlz.h"
#include "vendor/strtok_r.h"
#include "graphics.h"
#include "log.h"

#define SCALE_FACTOR 1000000.0

typedef struct Vertex
{
  FLOAT x, y, z;
  FLOAT nx, ny, nz;
  D3DCOLOR color;
  FLOAT t, v;
} Vertex;

void LoadModel(const char *name, int *vertexCount, IDirect3DVertexBuffer9 **vertexBuffer);

#endif
