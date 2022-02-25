#include <math.h>
#include "collision.h"
#include "plane.h"
#include "srandom.h"
#include "scene.h"

int Collision_CheckPointInTriangle(const D3DXVECTOR3 *point,
                                   const D3DXVECTOR3 *pa, const D3DXVECTOR3 *pb, const D3DXVECTOR3 *pc)
{
    D3DXVECTOR3 result;
    D3DXVECTOR3 e10 = *D3DXVec3Subtract(&result, pb, pa);
    D3DXVECTOR3 e20 = *D3DXVec3Subtract(&result, pc, pa);

    float a = D3DXVec3Dot(&e10, &e10);
    float b = D3DXVec3Dot(&e10, &e20);
    float c = D3DXVec3Dot(&e20, &e20);
    float ac_bb = (a * c) - (b * b);

    D3DXVECTOR3 vp = (D3DXVECTOR3){point->x - pa->x, point->y - pa->y, point->z - pa->z};
    float d = D3DXVec3Dot(&vp, &e10);
    float e = D3DXVec3Dot(&vp, &e20);
    float x = (d * c) - (e * b);
    float y = (e * a) - (d * b);
    float z = x + y - ac_bb;

    return ((in(z) & ~(in(x) | in(y))) & 0x80000000);
}

int Collision_GetLowestRoot(float a, float b, float c, float maxR,
                            float *root)
{
    // Check if a solution exists
    float determinant = b * b - 4.0f * a * c;

    // If determinant is negative it means no solutions.
    if (determinant < 0.0f)
        return 0;

    // calculate the two roots: (if determinant == 0 then
    // x1==x2 but let’s disregard that slight optimization)
    float sqrtD = sqrt(determinant);
    float r1 = (-b - sqrtD) / (2 * a);
    float r2 = (-b + sqrtD) / (2 * a);

    // Sort so x1 <= x2
    if (r1 > r2)
    {
        float temp = r2;
        r2 = r1;
        r1 = temp;
    }

    // Get lowest root:
    if (r1 > 0 && r1 < maxR)
    {
        *root = r1;
        return 1;
    }

    // It is possible that we want x2 - this can happen
    // if x1 < 0
    if (r2 > 0 && r2 < maxR)
    {
        *root = r2;
        return 1;
    }

    // No (valid) solutions
    return 0;
}

void Collision_CheckTriangle(CollisionPacket *colPackage,
                             const D3DXVECTOR3 *p1, const D3DXVECTOR3 *p2,
                             const D3DXVECTOR3 *p3)
{
    // Make the plane containing this triangle.
    Plane *trianglePlane = Plane_NewFromTriangle(p1, p2, p3);

    // Is triangle front-facing to the velocity vector?
    // We only check front-facing triangles
    // (your choice of course)
    if (Plane_IsFrontFacingTo(trianglePlane, &colPackage->normalizedVelocity))
    {
        // Get interval of plane intersection:
        double t0, t1;
        int embeddedInPlane = 0;

        // Calculate the signed distance from sphere
        // position to triangle plane
        double signedDistToTrianglePlane =
            Plane_SignedDistanceTo(trianglePlane, &colPackage->basePoint);

        // cache this as we’re going to use it a few times below:
        float normalDotVelocity =
            D3DXVec3Dot(&trianglePlane->normal, &colPackage->velocity);

        // if sphere is travelling parrallel to the plane:
        if (normalDotVelocity == 0.0f)
        {
            if (_fabs(signedDistToTrianglePlane) >= 1.0f)
            {
                // Sphere is not embedded in plane.
                // No collision possible:
                HeapFree(GetProcessHeap(), 0, trianglePlane);
                return;
            }
            else
            {
                // sphere is embedded in plane.
                // It intersects in the whole range [0..1]
                embeddedInPlane = 1;
                t0 = 0.0;
                t1 = 1.0;
            }
        }
        else
        {
            // N dot D is not 0. Calculate intersection interval:
            t0 = (-1.0 - signedDistToTrianglePlane) / normalDotVelocity;
            t1 = (1.0 - signedDistToTrianglePlane) / normalDotVelocity;

            // Swap so t0 < t1
            if (t0 > t1)
            {
                double temp = t1;
                t1 = t0;
                t0 = temp;
            }

            // Check that at least one result is within range:
            if (t0 > 1.0f || t1 < 0.0f)
            {
                // Both t values are outside values [0,1]
                // No collision possible:
                HeapFree(GetProcessHeap(), 0, trianglePlane);
                return;
            }

            // Clamp to [0,1]
            if (t0 < 0.0)
                t0 = 0.0;
            if (t1 < 0.0)
                t1 = 0.0;
            if (t0 > 1.0)
                t0 = 1.0;
            if (t1 > 1.0)
                t1 = 1.0;
        }

        // OK, at this point we have two time values t0 and t1
        // between which the swept sphere intersects with the
        // triangle plane. If any collision is to occur it must
        // happen within this interval.
        D3DXVECTOR3 collisionPoint;
        int foundCollison = 0;
        float t = 1.0;

        // First we check for the easy case - collision inside
        // the triangle. If this happens it must be at time t0
        // as this is when the sphere rests on the front side
        // of the triangle plane. Note, this can only happen if
        // the sphere is not embedded in the triangle plane.
        if (!embeddedInPlane)
        {
            D3DXVECTOR3 result;
            D3DXVECTOR3 planeIntersectionPoint =
                *D3DXVec3Add(&result,
                             D3DXVec3Subtract(&result, &colPackage->basePoint, &trianglePlane->normal),
                             D3DXVec3Scale(&result, &colPackage->velocity, t0));
            if (Collision_CheckPointInTriangle(&planeIntersectionPoint,
                                               p1, p2, p3))
            {
                foundCollison = 1;
                t = t0;
                collisionPoint = planeIntersectionPoint;
            }
        }

        HeapFree(GetProcessHeap(), 0, trianglePlane);

        // if we haven’t found a collision already we’ll have to
        // sweep sphere against points and edges of the triangle.
        // Note: A collision inside the triangle (the check above)
        // will always happen before a vertex or edge collision!
        // This is why we can skip the swept test if the above
        // gives a collision!
        if (foundCollison == 0)
        {
            // some commonly used terms:
            D3DXVECTOR3 result;
            D3DXVECTOR3 velocity = colPackage->velocity;
            D3DXVECTOR3 base = colPackage->basePoint;
            float velocitySquaredLength = D3DXVec3LengthSq(&velocity);
            float a, b, c; // Params for equation
            float newT;

            // For each vertex or edge a quadratic equation have to
            // be solved. We parameterize this equation as
            // a*t^2 + b*t + c = 0 and below we calculate the
            // parameters a,b and c for each test.
            // Check against points:
            a = velocitySquaredLength;

            // P1
            b = 2.0 * D3DXVec3Dot(&velocity, D3DXVec3Subtract(&result, &base, p1));
            c = D3DXVec3LengthSq(D3DXVec3Subtract(&result, p1, &base)) - 1.0;
            if (Collision_GetLowestRoot(a, b, c, t, &newT))
            {
                t = newT;
                foundCollison = 1;
                collisionPoint = *p1;
            }

            // P2
            b = 2.0 * D3DXVec3Dot(&velocity, D3DXVec3Subtract(&result, &base, p2));
            c = D3DXVec3LengthSq(D3DXVec3Subtract(&result, p2, &base)) - 1.0;
            if (Collision_GetLowestRoot(a, b, c, t, &newT))
            {
                t = newT;
                foundCollison = 1;
                collisionPoint = *p2;
            }

            // P3
            b = 2.0 * D3DXVec3Dot(&velocity, D3DXVec3Subtract(&result, &base, p3));
            c = D3DXVec3LengthSq(D3DXVec3Subtract(&result, p3, &base)) - 1.0;
            if (Collision_GetLowestRoot(a, b, c, t, &newT))
            {
                t = newT;
                foundCollison = 1;
                collisionPoint = *p3;
            }

            // Check agains edges:
            // p1 -> p2:
            D3DXVECTOR3 edge = *D3DXVec3Subtract(&result, p2, p1);
            D3DXVECTOR3 baseToVertex = *D3DXVec3Subtract(&result, p1, &base);
            float edgeSquaredLength = D3DXVec3LengthSq(&edge);
            float edgeDotVelocity = D3DXVec3Dot(&edge, &velocity);
            float edgeDotBaseToVertex = D3DXVec3Dot(&edge, &baseToVertex);

            // Calculate parameters for equation
            a = edgeSquaredLength * -velocitySquaredLength +
                edgeDotVelocity * edgeDotVelocity;
            b = edgeSquaredLength * (2 * D3DXVec3Dot(&velocity, &baseToVertex)) -
                2.0 * edgeDotVelocity * edgeDotBaseToVertex;
            c = edgeSquaredLength * (1 - D3DXVec3LengthSq(&baseToVertex)) +
                edgeDotBaseToVertex * edgeDotBaseToVertex;

            // Does the swept sphere collide against infinite edge?
            if (Collision_GetLowestRoot(a, b, c, t, &newT))
            {
                // Check if intersection is within line segment:
                float f = (edgeDotVelocity * newT - edgeDotBaseToVertex) /
                          edgeSquaredLength;
                if (f >= 0.0 && f <= 1.0)
                {
                    // intersection took place within segment.
                    t = newT;
                    foundCollison = 1;
                    collisionPoint = *D3DXVec3Add(&result, p1, D3DXVec3Scale(&result, &edge, f));
                }
            }

            // p2 -> p3:
            edge = *D3DXVec3Subtract(&result, p3, p2);
            baseToVertex = *D3DXVec3Subtract(&result, p2, &base);
            edgeSquaredLength = D3DXVec3LengthSq(&edge);
            edgeDotVelocity = D3DXVec3Dot(&edge, &velocity);
            edgeDotBaseToVertex = D3DXVec3Dot(&edge, &baseToVertex);
            a = edgeSquaredLength * -velocitySquaredLength +
                edgeDotVelocity * edgeDotVelocity;
            b = edgeSquaredLength * (2 * D3DXVec3Dot(&velocity, &baseToVertex)) -
                2.0 * edgeDotVelocity * edgeDotBaseToVertex;
            c = edgeSquaredLength * (1 - D3DXVec3LengthSq(&baseToVertex)) +
                edgeDotBaseToVertex * edgeDotBaseToVertex;
            if (Collision_GetLowestRoot(a, b, c, t, &newT))
            {
                float f = (edgeDotVelocity * newT - edgeDotBaseToVertex) /
                          edgeSquaredLength;
                if (f >= 0.0 && f <= 1.0)
                {
                    t = newT;
                    foundCollison = 1;
                    collisionPoint = *D3DXVec3Add(&result, p2, D3DXVec3Scale(&result, &edge, f));
                }
            }

            // p3 -> p1:
            edge = *D3DXVec3Subtract(&result, p1, p3);
            baseToVertex = *D3DXVec3Subtract(&result, p3, &base);
            edgeSquaredLength = D3DXVec3LengthSq(&edge);
            edgeDotVelocity = D3DXVec3Dot(&edge, &velocity);
            edgeDotBaseToVertex = D3DXVec3Dot(&edge, &baseToVertex);
            a = edgeSquaredLength * -velocitySquaredLength +
                edgeDotVelocity * edgeDotVelocity;
            b = edgeSquaredLength * (2 * D3DXVec3Dot(&velocity, &baseToVertex)) -
                2.0 * edgeDotVelocity * edgeDotBaseToVertex;
            c = edgeSquaredLength * (1 - D3DXVec3LengthSq(&baseToVertex)) +
                edgeDotBaseToVertex * edgeDotBaseToVertex;
            if (Collision_GetLowestRoot(a, b, c, t, &newT))
            {
                float f = (edgeDotVelocity * newT - edgeDotBaseToVertex) /
                          edgeSquaredLength;
                if (f >= 0.0 && f <= 1.0)
                {
                    t = newT;
                    foundCollison = 1;
                    collisionPoint = *D3DXVec3Add(&result, p3, D3DXVec3Scale(&result, &edge, f));
                }
            }
        }

        // Set result:
        if (foundCollison == 1)
        {
            // distance to collision: ’t’ is time of collision
            float distToCollision = t * sqrt(D3DXVec3LengthSq(&colPackage->velocity));

            // Does this triangle qualify for the closest hit?
            // it does if it’s the first hit or the closest
            if (colPackage->foundCollision == 0 ||
                distToCollision < colPackage->nearestDistance)
            {
                // Collision information nessesary for sliding
                colPackage->nearestDistance = distToCollision;
                colPackage->intersectionPoint = collisionPoint;
                colPackage->foundCollision = 1;
            }
        }
    } // if not backface
}

D3DXVECTOR3 Collision_CollideAndSlide(CollisionPacket *colPackage, const D3DXVECTOR3 *pos, const D3DXVECTOR3 *vel, const D3DXVECTOR3 *gravity)
{
    // Do collision detection:
    colPackage->R3Position = *pos;
    colPackage->R3Velocity = *vel;

    // calculate position and velocity in eSpace
    D3DXVECTOR3 eSpacePosition = Collision_Divide(&colPackage->R3Position, &colPackage->eRadius);
    D3DXVECTOR3 eSpaceVelocity = Collision_Divide(&colPackage->R3Velocity, &colPackage->eRadius);

    // Iterate until we have our final position.
    D3DXVECTOR3 finalPosition = Collision_CollideWithWorld(colPackage, &eSpacePosition, &eSpaceVelocity, 0);

    // Add gravity pull:
    // Set the new R3 position (convert back from eSpace to R3
    colPackage->R3Position = Collision_Multiply(&finalPosition, &colPackage->eRadius);
    colPackage->R3Velocity = *gravity;
    eSpaceVelocity = Collision_Divide(gravity, &colPackage->eRadius);
    finalPosition = Collision_CollideWithWorld(colPackage, &finalPosition, &eSpaceVelocity, 0);

    // Convert final result back to R3:
    finalPosition = Collision_Multiply(&finalPosition, &colPackage->eRadius);

    // Move the entity (application specific function)
    return finalPosition;
}

D3DXVECTOR3 Collision_CollideWithWorld(CollisionPacket *colPackage, const D3DXVECTOR3 *pos, const D3DXVECTOR3 *vel, int collisionRecursionDepth)
{
    // All hard-coded distances in this function is
    // scaled to fit the setting above..
    D3DXVECTOR3 result;
    const float unitsPerMeter = 100.0f;
    float unitScale = unitsPerMeter / 100.0f;
    float veryCloseDistance = 0.005f * unitScale;

    // do we need to worry?
    if (collisionRecursionDepth > 5)
        return *pos;

    // Ok, we need to worry:
    colPackage->velocity = *vel;
    colPackage->normalizedVelocity = *D3DXVec3Normalize(&result, vel);
    colPackage->basePoint = *pos;
    colPackage->foundCollision = 0;

    // Check for collision (calls the collision routines)
    // Application specific!!
    for (int i = 0; i < actors[14]->vertexCount; i += 3)
    {
        const Vertex a = AddVertex(actors[14]->vertices[i + 0], actors[14]->position);
        const Vertex b = AddVertex(actors[14]->vertices[i + 1], actors[14]->position);
        const Vertex c = AddVertex(actors[14]->vertices[i + 2], actors[14]->position);

        D3DXVECTOR3 aV = (D3DXVECTOR3){ a.x, a.y, a.z };
        D3DXVECTOR3 bV = (D3DXVECTOR3){ b.x, b.y, b.z };
        D3DXVECTOR3 cV = (D3DXVECTOR3){ c.x, c.y, c.z };

        D3DXVECTOR3 aVd = Collision_Divide(&aV, &colPackage->eRadius);
        D3DXVECTOR3 bVd = Collision_Divide(&bV, &colPackage->eRadius);
        D3DXVECTOR3 cVd = Collision_Divide(&cV, &colPackage->eRadius);
        
        Collision_CheckTriangle(colPackage, &aVd, &bVd, &cVd);
    }

    // If no collision we just move along the velocity
    if (colPackage->foundCollision == 0)
    {
        return *D3DXVec3Add(&result, pos, vel);
    }

    // *** Collision occured ***
    // The original destination point
    D3DXVECTOR3 destinationPoint = *D3DXVec3Add(&result, pos, vel);
    D3DXVECTOR3 newBasePoint = *pos;

    // only update if we are not already very close
    // and if so we only move very close to intersection..not
    // to the exact spot.
    if (colPackage->nearestDistance >= veryCloseDistance)
    {
        D3DXVECTOR3 V = *vel;
        Collision_SetLength(&V, colPackage->nearestDistance - veryCloseDistance);
        newBasePoint = *D3DXVec3Add(&result, &colPackage->basePoint, &V);

        // Adjust polygon intersection point (so sliding
        // plane will be unaffected by the fact that we
        // move slightly less than collision tells us)
        V = *D3DXVec3Normalize(&result, &V);
        colPackage->intersectionPoint = *D3DXVec3Subtract(&result, &colPackage->intersectionPoint, D3DXVec3Scale(&result, &V, veryCloseDistance));
    }

    // Determine the sliding plane
    D3DXVECTOR3 slidePlaneOrigin = colPackage->intersectionPoint;
    D3DXVECTOR3 slidePlaneNormal = *D3DXVec3Subtract(&result, &newBasePoint, &colPackage->intersectionPoint);
    slidePlaneNormal = *D3DXVec3Normalize(&result, &slidePlaneNormal);
    Plane *slidingPlane = Plane_New(&slidePlaneOrigin, &slidePlaneNormal);

    // Again, sorry about formatting.. but look carefully ;)
    D3DXVECTOR3 newDestinationPoint = *D3DXVec3Subtract(&result, &destinationPoint, D3DXVec3Scale(&result, &slidePlaneNormal, Plane_SignedDistanceTo(slidingPlane, &destinationPoint)));

    // Generate the slide vector, which will become our new
    // velocity vector for the next iteration
    D3DXVECTOR3 newVelocityVector = *D3DXVec3Subtract(&result, &newDestinationPoint, &colPackage->intersectionPoint);

    // Recurse:
    // dont recurse if the new velocity is very small
    if (sqrt(D3DXVec3LengthSq(&newVelocityVector)) < veryCloseDistance)
    {
        HeapFree(GetProcessHeap(), 0, slidingPlane);
        return newBasePoint;
    }

    HeapFree(GetProcessHeap(), 0, slidingPlane);

    return Collision_CollideWithWorld(colPackage, &newBasePoint, &newVelocityVector, ++collisionRecursionDepth);
}

D3DXVECTOR3 Collision_Divide(const D3DXVECTOR3 *lhs, const D3DXVECTOR3 *rhs)
{
    D3DXVECTOR3 result;
    result.x = lhs->x / rhs->x;
    result.y = lhs->y / rhs->y;
    result.z = lhs->z / rhs->z;
    return result;
}

D3DXVECTOR3 Collision_Multiply(const D3DXVECTOR3 *lhs, const D3DXVECTOR3 *rhs)
{
    D3DXVECTOR3 result;
    result.x = lhs->x * rhs->x;
    result.y = lhs->y * rhs->y;
    result.z = lhs->z * rhs->z;
    return result;
}

void Collision_SetLength(D3DVECTOR *v, float l)
{
    float len = sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
    v->x *= l / len;
    v->y *= l / len;
    v->z *= l / len;
}
