#include "collision.h"

D3DXVECTOR3 ClosestPoint(D3DXVECTOR3 p, D3DXVECTOR3 a, D3DXVECTOR3 b, D3DXVECTOR3 c)
{
  D3DXVECTOR3 ab;
  D3DXVec3Subtract(&ab, &b, &a);

  D3DXVECTOR3 ac;
  D3DXVec3Subtract(&ac, &c, &a);

  D3DXVECTOR3 ap;
  D3DXVec3Subtract(&ap, &p, &a);

  float d1 = D3DXVec3Dot(&ab, &ap);
  float d2 = D3DXVec3Dot(&ac, &ap);
  if (d1 <= 0.0f && d2 <= 0.0f) return a;

  D3DXVECTOR3 bp;
  D3DXVec3Subtract(&bp, &p, &b);

  float d3 = D3DXVec3Dot(&ab, &bp);
  float d4 = D3DXVec3Dot(&ac, &bp);
  if (d3 >= 0.0f && d4 <= d3) return b;

  float vc = (d1 * d4) - (d3 * d2);
  if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
  {
    float v = d1 / (d1 - d3);
    D3DXVECTOR3 scaled;
    D3DXVec3Scale(&scaled, &ab, v);

    D3DXVECTOR3 added;
    D3DXVec3Add(&added, &a, &scaled); 
    return added;
  }

  D3DXVECTOR3 cp;
  D3DXVec3Subtract(&cp, &p, &c);

  float d5 = D3DXVec3Dot(&ab, &cp);
  float d6 = D3DXVec3Dot(&ac, &cp);
  if (d6 >= 0.0f && d5 <= d6) return c;

  float vb = (d5 * d2) - (d1 * d6);
  if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
  {
    float w = d2 / (d2 - d6);
    D3DXVECTOR3 scaled;
    D3DXVec3Scale(&scaled, &ac, w);

    D3DXVECTOR3 added;
    D3DXVec3Add(&added, &a, &scaled); 
    return added;
  }

  float va = (d3 * d6) - (d5 * d4);
  if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f)
  {
    float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));

    D3DXVECTOR3 cb;
    D3DXVec3Subtract(&cb, &c, &b);

    D3DXVECTOR3 scaled;
    D3DXVec3Scale(&scaled, &cb, w);

    D3DXVECTOR3 added;
    D3DXVec3Add(&added, &b, &scaled); 
    return added;
  }

  float denom = 1.0f / (va + vb + vc);
  float v = vb * denom;
  float w = vc * denom;

  D3DXVECTOR3 scaledAB;
  D3DXVec3Scale(&scaledAB, &ab, v);

  D3DXVECTOR3 scaledAC;
  D3DXVec3Scale(&scaledAC, &ac, w);

  D3DXVECTOR3 added;
  D3DXVec3Add(&added, &a, &scaledAB); 
  D3DXVec3Add(&added, &a, &scaledAC); 
  return added;
}

int TestSphereTriangle(D3DXVECTOR3 center, float radius, D3DXVECTOR3 a, D3DXVECTOR3 b, D3DXVECTOR3 c)
{
  D3DXVECTOR3 p = ClosestPoint(center, a, b, c);
  D3DXVECTOR3 v;
  D3DXVec3Subtract(&v, &p, &center);

  return D3DXVec3Dot(&v, &v) <= radius * radius;
}
