#include "bush.h"
#include "sky.h"

void BushStart(Actor *self) {
  self->effect->lpVtbl->SetBool(self->effect, "_IsFoliage", TRUE);
  self->effect->lpVtbl->SetFloat(self->effect, "_BendScale", 0.08f);
}

void BushUpdate(Actor *self) {
  self->effect->lpVtbl->SetFloat(self->effect, "_WindSpeed", currWindSpeed);
}
