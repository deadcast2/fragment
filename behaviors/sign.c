#include "sign.h"
#include "..\scene.h"
#include "..\vec3.h"

void SignStart(Actor *self) {
  self->effect->lpVtbl->SetVector(self->effect, "_Color",
                                  &(D3DXVECTOR4){0, 0, 0, 0.6f});
}

void SignUpdate(Actor *self) {
  D3DXVECTOR3 activationPoint =
      (D3DXVECTOR3){self->vertices[0].x, cameraPos.y, self->vertices[0].z};
  D3DXVECTOR3 dir = Vec3_Sub(&cameraPos, &activationPoint);
  float dist = Vec3_LenSq(&dir);

  // We're close enough so show text.
  if (dist < 3) {
    actors[17]->enabled = 1;

    RECT textbox;
    SetRect(&textbox, SCREEN_WIDTH / 2 - 190, 180, SCREEN_WIDTH / 2 + 190, 610);
    const char *blurb =
        "You have arrived on fragment #13. Its function is to "
        "carry the central communication lines to fragment #7. Hallucinations "
        "or seizures may occur "
        "due to sporadic voltage spikes in the faulty R.S.U. or Reality "
        "Stabilization Unit. "
        "Every island is equipped with an R.S.U. to help with "
        "emergency load cycles distributed by the Lemonium Core. Don't attempt "
        "to open the hatch unless instructed by the Crow. Please make your "
        "stay brief.";
    d3dFont2->lpVtbl->DrawTextA(d3dFont2, NULL, blurb, strlen(blurb), &textbox,
                                DT_CENTER | DT_WORDBREAK,
                                D3DCOLOR_ARGB(255, 255, 255, 255));
  } else {
    actors[17]->enabled = 0;
  }
}