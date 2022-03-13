#ifndef _PLANE_H_
#define _PLANE_H_

#include <d3dx9.h>

typedef struct Plane {
  float equation[4];
  D3DXVECTOR3 origin;
  D3DXVECTOR3 normal;
} Plane;

Plane *Plane_New(const D3DXVECTOR3 *origin, const D3DXVECTOR3 *normal);
Plane *Plane_NewFromTriangle(const D3DXVECTOR3 *p1, const D3DXVECTOR3 *p2,
                             const D3DXVECTOR3 *p3);
int Plane_IsFrontFacingTo(const Plane *plane, const D3DXVECTOR3 *direction);
double Plane_SignedDistanceTo(const Plane *plane, const D3DXVECTOR3 *point);

#endif
