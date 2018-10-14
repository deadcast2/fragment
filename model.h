#ifndef _MODEL_H_
#define _MODEL_H_

#include <windows.h>
#include <stdio.h>
#include "vendor/fastlz.h"
#include "vendor/strtok_r.h"
#include "graphics.h"
#include "log.h"

#define SCALE_FACTOR 1000000.0
#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_TEX1)

struct vertex
{
  FLOAT x, y, z;
  FLOAT t, v;
};

void LoadModel(const char *name, int *vertexCount, IDirect3DVertexBuffer9 **vertexBuffer);

#endif
