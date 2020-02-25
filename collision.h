#ifndef _COLLISION_H_
#define _COLLISION_H_

#include "model.h"

D3DXVECTOR3 ClosestPoint(D3DXVECTOR3 p, D3DXVECTOR3 a, D3DXVECTOR3 b, D3DXVECTOR3 c);
int TestSphereTriangle(D3DXVECTOR3 center, float radius, D3DXVECTOR3 a, D3DXVECTOR3 b, D3DXVECTOR3 c);

#endif
