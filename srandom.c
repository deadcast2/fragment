#include "srandom.h"

float lin_inter(float x, float y, float s)
{
	return x + s * (y - x);
}

float smooth_inter(float x, float y, float s)
{
	if (s > 1)
		s = 1;
		
	return lin_inter(x, y, s * s * (3 - 2 * s));
}

void seed_smooth_rand()
{
	lfsr = 0xACE1u;
}

unsigned smooth_rand()
{
	unsigned bit = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5)) & 1;
	return lfsr = (lfsr >> 1) | (bit << 15);
}

float _fabs(float value)
{
	return value < 0 ? value * -1.0 : value;
}
