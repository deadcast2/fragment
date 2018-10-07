#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <d3dx9.h>
#include "graphics.h"
#include "log.h"

void LoadTexture(const char *name, IDirect3DTexture9 **d3dTexture);

#endif
