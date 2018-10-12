#ifndef _SRANDOM_H_
#define _SRANDOM_H_

unsigned short lfsr;

float lin_inter(float x, float y, float s);
float smooth_inter(float x, float y, float s);
void seed_smooth_rand();
unsigned smooth_rand();

#endif
