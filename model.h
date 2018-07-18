#ifndef _MODEL_H_
#define _MODEL_H_

#include <windows.h>
#include <stdio.h>
#include "vendor/fastlz.h"
#include "vendor/strtok_r.h"
#include "graphics.h"
#include "log.h"

#define SCALE_FACTOR 1000000.0
#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE)

struct vertex
{
  FLOAT x, y, z;
  DWORD color;
};

struct vertex *LoadModel(const char *name, int *count);

#endif
