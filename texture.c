#include "texture.h"

void LoadTexture(const char *name, IDirect3DTexture9 **d3dTexture) {
  if (FAILED(D3DXCreateTextureFromResource(d3ddev, NULL, name, d3dTexture))) {
    Log("Could not load texture resource\n");
  }
}
