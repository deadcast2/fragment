#include "vec3.h"

D3DXVECTOR3 Vec3_Add(const D3DXVECTOR3 *a, const D3DXVECTOR3 *b) {
  D3DXVECTOR3 result;
  D3DXVec3Add(&result, a, b);
  return result;
}

D3DXVECTOR3 Vec3_Sub(const D3DXVECTOR3 *a, const D3DXVECTOR3 *b) {
  D3DXVECTOR3 result;
  D3DXVec3Subtract(&result, a, b);
  return result;
}

D3DXVECTOR3 Vec3_Cross(const D3DXVECTOR3 *a, const D3DXVECTOR3 *b) {
  D3DXVECTOR3 result;
  D3DXVec3Cross(&result, a, b);
  return result;
}

D3DXVECTOR3 Vec3_Scale(const D3DXVECTOR3 *v, float s) {
  D3DXVECTOR3 result;
  D3DXVec3Scale(&result, v, s);
  return result;
}

float Vec3_Dot(const D3DXVECTOR3 *a, const D3DXVECTOR3 *b) {
  return D3DXVec3Dot(a, b);
}

D3DXVECTOR3 Vec3_Norm(const D3DXVECTOR3 *v) {
  D3DXVECTOR3 result;
  D3DXVec3Normalize(&result, v);
  return result;
}

float Vec3_LenSq(const D3DXVECTOR3 *v) { return D3DXVec3LengthSq(v); }

float Vec3_Len(const D3DXVECTOR3 *v) { return sqrt(D3DXVec3LengthSq(v)); }

D3DXVECTOR3 Vec3_Div(const D3DXVECTOR3 *a, const D3DXVECTOR3 *b) {
  D3DXVECTOR3 result;
  result.x = a->x / b->x;
  result.y = a->y / b->y;
  result.z = a->z / b->z;
  return result;
}

D3DXVECTOR3 Vec3_Mul(const D3DXVECTOR3 *a, const D3DXVECTOR3 *b) {
  D3DXVECTOR3 result;
  result.x = a->x * b->x;
  result.y = a->y * b->y;
  result.z = a->z * b->z;
  return result;
}

void Vec3_SetLen(D3DVECTOR *v, float l) {
  float len = sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
  v->x *= l / len;
  v->y *= l / len;
  v->z *= l / len;
}
