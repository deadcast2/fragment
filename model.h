#ifndef _MODEL_H_
#define _MODEL_H_

#include "graphics.h"
#include "log.h"
#include "vendor/fastlz.h"
#include "vendor/strtok_r.h"
#include <stdio.h>
#include <windows.h>

#define SCALE_FACTOR 1000000.0

typedef struct Vertex {
  FLOAT x, y, z;
  FLOAT nx, ny, nz;
  D3DCOLOR color;
  FLOAT t, v;
} Vertex;

void LoadModel(const char *name, int *vertexCount, Vertex **vertices,
               IDirect3DVertexBuffer9 **vertexBuffer);
Vertex AddVertex(const Vertex a, const Vertex b);

#endif
