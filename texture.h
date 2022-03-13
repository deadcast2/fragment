#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "graphics.h"
#include "log.h"
#include <d3dx9.h>

void LoadTexture(const char *name, IDirect3DTexture9 **d3dTexture);

#endif
