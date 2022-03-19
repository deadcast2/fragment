#include "loading.h"
#include "..\scene.h"
#include "..\vec3.h"

void LoadingUpdate(Actor *self, float deltaTime) {
  static float delay = 6.0f;

  if (delay > 0) {
    delay -= deltaTime;

    RECT textbox;
    // Yeah good enough size.
    SetRect(&textbox, 25, SCREEN_HEIGHT - 100, SCREEN_WIDTH,
            SCREEN_HEIGHT - 25);
    const char *blurb = "Loading...";
    d3dFont2->lpVtbl->DrawTextA(d3dFont2, NULL, blurb, strlen(blurb), &textbox,
                                DT_LEFT | DT_BOTTOM,
                                D3DCOLOR_ARGB(255, 255, 255, 255));
    return;
  }

  for (int i = 0; i < ACTOR_COUNT; i++) {
    // Now enable the ring intro.
    actors[i]->enabled = strstr(actors[i]->name, "ring") != NULL;

    if (actors[i]->Start && actors[i]->enabled) {
      actors[i]->Start(actors[i]);
    }
  }

  self->enabled = FALSE;
}
