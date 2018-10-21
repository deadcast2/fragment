#include "effect.h"

void LoadEffect(const char *name, LPD3DXEFFECT *effect)
{
  LPD3DXBUFFER errorlog;
  D3DXHANDLE technique;
  D3DXCreateEffectFromResource(d3ddev, 0, name, 0, 0, 0, 0, effect, &errorlog);
  if(errorlog) Log((char*)errorlog->lpVtbl->GetBufferPointer(errorlog));
  (*effect)->lpVtbl->FindNextValidTechnique(*effect, NULL, &technique);
}
