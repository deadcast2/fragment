#include "texture.h"

void LoadTexture(const char *name)
{
  if(FAILED(D3DXCreateTextureFromResource(d3ddev, NULL, name, &testTexture)))
  {
    Log("Could not load test texture resource\n");
    return;
  }
}
