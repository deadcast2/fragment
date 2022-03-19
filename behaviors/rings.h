#ifndef _RINGS_H_
#define _RINGS_H_

#include "..\actor.h"

#define CYCLE_COUNT 4
#define RING_SCALE_FACTOR 0.002
#define RING_HSCALE_FACTOR 0.005

void RingStart(Actor *self);
void Ring1Update(Actor *self, float deltaTime);
void Ring2Update(Actor *self, float deltaTime);
void Ring3Update(Actor *self, float deltaTime);
void Ring4Update(Actor *self, float deltaTime);
void Ring5Update(Actor *self, float deltaTime);
void RingGongUpdate(Actor *self, float deltaTime);
BOOL RingDelayHelper(Actor *actor, float *delay, int *cycles, float deltaTime);

#endif
