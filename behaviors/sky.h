#ifndef _SKY_H_
#define _SKY_H_

#include "..\actor.h"

extern float currWindSpeed;

void SkyStart(Actor *self);
void SkyUpdate(Actor *self, float deltaTime);

#endif
