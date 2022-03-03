#ifndef _COLLISION_H_
#define _COLLISION_H_

#include "model.h"

typedef unsigned int uint32;
#define in(a) ((uint32)a)

typedef struct CollisionPacket
{
    D3DXVECTOR3 eRadius; // ellipsoid radius

    // Information about the move being requested: (in R3)
    D3DXVECTOR3 R3Velocity;
    D3DXVECTOR3 R3Position;

    // Information about the move being requested: (in eSpace)
    D3DXVECTOR3 velocity;
    D3DXVECTOR3 normalizedVelocity;
    D3DXVECTOR3 basePoint;

    // Hit information
    int foundCollision;
    double nearestDistance;
    D3DXVECTOR3 intersectionPoint;
    int actorIndex;
} CollisionPacket;

int Collision_CheckPointInTriangle(const D3DXVECTOR3 *point,
                                   const D3DXVECTOR3 *a, const D3DXVECTOR3 *b, const D3DXVECTOR3 *c);
int Collision_GetLowestRoot(float a, float b, float c, float maxR,
                            float *root);
int Collision_CheckTriangle(CollisionPacket *colPackage,
                             const D3DXVECTOR3 *p1, const D3DXVECTOR3 *p2,
                             const D3DXVECTOR3 *p3);
D3DXVECTOR3 Collision_CollideAndSlide(CollisionPacket *colPackage, const D3DXVECTOR3 *pos, const D3DXVECTOR3 *vel,
                                      const D3DXVECTOR3 *gravity);
D3DXVECTOR3 Collision_CollideWithWorld(CollisionPacket *colPackage, const D3DXVECTOR3 *pos,
                                       const D3DXVECTOR3 *vel, int collisionRecursionDepth);

#endif
