#ifndef _ARRIVAL_H_
#define _ARRIVAL_H_

#include "..\actor.h"

extern BOOL arriving;

void ArrivalStart(Actor *self);
void ArrivalUpdate(Actor *self, float deltaTime);

#endif
