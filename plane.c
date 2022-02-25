#include "plane.h"

Plane *Plane_New(const D3DXVECTOR3 *origin, const D3DXVECTOR3 *normal)
{
    Plane *newPlane = malloc(sizeof(Plane));
    if (newPlane == NULL)
        return NULL;

    newPlane->normal = *normal;
    newPlane->origin = *origin;
    newPlane->equation[0] = normal->x;
    newPlane->equation[1] = normal->y;
    newPlane->equation[2] = normal->z;
    newPlane->equation[3] = -(normal->x * origin->x + normal->y * origin->y + normal->z * origin->z);

    return newPlane;
}

Plane *Plane_NewFromTriangle(const D3DXVECTOR3 *p1, const D3DXVECTOR3 *p2, const D3DXVECTOR3 *p3)
{
    Plane *newPlane = malloc(sizeof(Plane));
    if (newPlane == NULL)
        return NULL;

    D3DXVECTOR3 result;
    newPlane->normal = *D3DXVec3Cross(&result,
                                     D3DXVec3Subtract(&result, p2, p1), D3DXVec3Subtract(&result, p3, p1));
    newPlane->normal = *D3DXVec3Normalize(&result, &newPlane->normal);
    newPlane->origin = *p1;
    newPlane->equation[0] = newPlane->normal.x;
    newPlane->equation[1] = newPlane->normal.y;
    newPlane->equation[2] = newPlane->normal.z;
    newPlane->equation[3] = -(newPlane->normal.x * newPlane->origin.x + newPlane->normal.y * newPlane->origin.y + newPlane->normal.z * newPlane->origin.z);

    return newPlane;
}

int Plane_IsFrontFacingTo(const Plane *plane, const D3DXVECTOR3 *direction)
{
    double dot = D3DXVec3Dot(&plane->normal, direction);
    return dot <= 0;
}

double Plane_SignedDistanceTo(const Plane *plane, const D3DXVECTOR3 *point)
{
    return D3DXVec3Dot(point, &plane->normal) + plane->equation[3];
}
