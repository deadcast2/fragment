#include "sign.h"
#include "..\scene.h"
#include "..\vec3.h"

void SignUpdate(Actor *self) {
  D3DXVECTOR3 activationPoint =
      (D3DXVECTOR3){self->vertices[0].x, cameraPos.y, self->vertices[0].z};
  D3DXVECTOR3 dir = Vec3_Sub(&cameraPos, &activationPoint);
  float dist = Vec3_LenSq(&dir);

  // We're close enough so show text.
  if (dist < 3) {
    actors[17]->enabled = 1;

    static RECT textbox;
    SetRect(&textbox, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    d3dFont->lpVtbl->DrawTextA(d3dFont, NULL, "Welcome to the island fragment.",
                               15, &textbox, DT_CENTER | DT_VCENTER,
                               D3DCOLOR_ARGB(255, 255, 255, 255));
  } else {
    actors[17]->enabled = 0;
  }
}
