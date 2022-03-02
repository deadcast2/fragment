#ifndef _VEC3_H_
#define _VEC3_H_

#include <d3dx9.h>

D3DXVECTOR3 Vec3_Add(const D3DXVECTOR3 *a, const D3DXVECTOR3 *b);
D3DXVECTOR3 Vec3_Sub(const D3DXVECTOR3 *a, const D3DXVECTOR3 *b);
D3DXVECTOR3 Vec3_Cross(const D3DXVECTOR3 *a, const D3DXVECTOR3 *b);
D3DXVECTOR3 Vec3_Norm(const D3DXVECTOR3 *v);
D3DXVECTOR3 Vec3_Scale(const D3DXVECTOR3 *v, float s);
float Vec3_Dot(const D3DXVECTOR3 *a, const D3DXVECTOR3 *b);
float Vec3_LenSq(const D3DXVECTOR3 *v);
float Vec3_Len(const D3DXVECTOR3 *v);
D3DXVECTOR3 Vec3_Div(const D3DXVECTOR3 *a, const D3DXVECTOR3 *b);
D3DXVECTOR3 Vec3_Mul(const D3DXVECTOR3 *a, const D3DXVECTOR3 *b);
void Vec3_SetLen(D3DVECTOR *v, float l);

#endif
