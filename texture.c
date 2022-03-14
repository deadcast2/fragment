#include "texture.h"

void LoadTexture(const char *name, IDirect3DTexture9 **d3dTexture) {
  D3DXCreateTextureFromResource(d3ddev, NULL, name, d3dTexture);
}
