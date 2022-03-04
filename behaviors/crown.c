#include "crown.h"
#include "sky.h"

void CrownStart(Actor *self) {
  self->effect->lpVtbl->SetBool(self->effect, "_IsFoliage", TRUE);
  self->effect->lpVtbl->SetFloat(self->effect, "_BendScale", 0.08f);
}

void CrownUpdate(Actor *self) {
  self->effect->lpVtbl->SetFloat(self->effect, "_WindSpeed",
                                 currWindSpeed * 4.0f);
}
