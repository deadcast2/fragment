#include <math.h>
#include "collision.h"
#include "plane.h"
#include "srandom.h"
#include "scene.h"
#include "vec3.h"

int Collision_CheckPointInTriangle(const D3DXVECTOR3 *point, const D3DXVECTOR3 *a, const D3DXVECTOR3 *b, const D3DXVECTOR3 *c)
{
    D3DXVECTOR3 ap = Vec3_Sub(point, a);
    D3DXVECTOR3 bp = Vec3_Sub(point, b);
    D3DXVECTOR3 cp = Vec3_Sub(point, c);

    float ab = Vec3_Dot(&ap, &bp);
    float ac = Vec3_Dot(&ap, &cp);
    float bc = Vec3_Dot(&bp, &cp);
    float cc = Vec3_Dot(&cp, &cp);

    if (bc * ac - cc * ab < 0.0f)
        return 0;

    float bb = Vec3_Dot(&bp, &bp);

    if (ab * bc - ac * bb < 0.0f)
        return 0;

    return 1;
}

int Collision_GetLowestRoot(float a, float b, float c, float maxR, float *root)
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
        double signedDistToTrianglePlane = Plane_SignedDistanceTo(trianglePlane, &colPackage->basePoint);

        // cache this as we’re going to use it a few times below:
        float normalDotVelocity = D3DXVec3Dot(&trianglePlane->normal, &colPackage->velocity);

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
            D3DXVECTOR3 sub = Vec3_Sub(&colPackage->basePoint, &trianglePlane->normal);
            D3DXVECTOR3 scale = Vec3_Scale(&colPackage->velocity, t0);
            D3DXVECTOR3 planeIntersectionPoint = Vec3_Add(&sub, &scale);

            if (Collision_CheckPointInTriangle(&planeIntersectionPoint, p1, p2, p3) == 1)
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
            D3DXVECTOR3 velocity = colPackage->velocity;
            D3DXVECTOR3 base = colPackage->basePoint;
            float velocitySquaredLength = Vec3_LenSq(&velocity);
            float a, b, c; // Params for equation
            float newT;

            // For each vertex or edge a quadratic equation have to
            // be solved. We parameterize this equation as
            // a*t^2 + b*t + c = 0 and below we calculate the
            // parameters a,b and c for each test.
            // Check against points:
            a = velocitySquaredLength;

            // P1
            D3DXVECTOR3 baseP1 = Vec3_Sub(&base, p1);
            D3DXVECTOR3 p1Base = Vec3_Sub(p1, &base);
            b = 2.0 * Vec3_Dot(&velocity, &baseP1);
            c = Vec3_LenSq(&p1Base) - 1.0;
            if (Collision_GetLowestRoot(a, b, c, t, &newT))
            {
                t = newT;
                foundCollison = 1;
                collisionPoint = *p1;
            }

            // P2
            D3DXVECTOR3 baseP2 = Vec3_Sub(&base, p2);
            D3DXVECTOR3 p2Base = Vec3_Sub(p2, &base);
            b = 2.0 * Vec3_Dot(&velocity, &baseP2);
            c = Vec3_LenSq(&p2Base) - 1.0;
            if (Collision_GetLowestRoot(a, b, c, t, &newT))
            {
                t = newT;
                foundCollison = 1;
                collisionPoint = *p2;
            }

            // P3
            D3DXVECTOR3 baseP3 = Vec3_Sub(&base, p3);
            D3DXVECTOR3 p3Base = Vec3_Sub(p3, &base);
            b = 2.0 * Vec3_Dot(&velocity, &baseP3);
            c = Vec3_LenSq(&p3Base) - 1.0;
            if (Collision_GetLowestRoot(a, b, c, t, &newT))
            {
                t = newT;
                foundCollison = 1;
                collisionPoint = *p3;
            }

            // Check agains edges:
            // p1 -> p2:
            D3DXVECTOR3 edge = Vec3_Sub(p2, p1);
            D3DXVECTOR3 baseToVertex = Vec3_Sub(p1, &base);
            float edgeSquaredLength = Vec3_LenSq(&edge);
            float edgeDotVelocity = Vec3_Dot(&edge, &velocity);
            float edgeDotBaseToVertex = Vec3_Dot(&edge, &baseToVertex);

            // Calculate parameters for equation
            a = edgeSquaredLength * -velocitySquaredLength + edgeDotVelocity * edgeDotVelocity;
            b = edgeSquaredLength * (2 * Vec3_Dot(&velocity, &baseToVertex)) - 2.0 * edgeDotVelocity * edgeDotBaseToVertex;
            c = edgeSquaredLength * (1 - Vec3_LenSq(&baseToVertex)) + edgeDotBaseToVertex * edgeDotBaseToVertex;

            // Does the swept sphere collide against infinite edge?
            if (Collision_GetLowestRoot(a, b, c, t, &newT))
            {
                // Check if intersection is within line segment:
                float f = (edgeDotVelocity * newT - edgeDotBaseToVertex) / edgeSquaredLength;
                if (f >= 0.0 && f <= 1.0)
                {
                    // intersection took place within segment.
                    t = newT;
                    foundCollison = 1;

                    D3DXVECTOR3 scale = Vec3_Scale(&edge, f);
                    collisionPoint = Vec3_Add(p1, &scale);
                }
            }

            // p2 -> p3:
            edge = Vec3_Sub(p3, p2);
            baseToVertex = Vec3_Sub(p2, &base);
            edgeSquaredLength = Vec3_LenSq(&edge);
            edgeDotVelocity = Vec3_Dot(&edge, &velocity);
            edgeDotBaseToVertex = Vec3_Dot(&edge, &baseToVertex);

            a = edgeSquaredLength * -velocitySquaredLength + edgeDotVelocity * edgeDotVelocity;
            b = edgeSquaredLength * (2 * Vec3_Dot(&velocity, &baseToVertex)) - 2.0 * edgeDotVelocity * edgeDotBaseToVertex;
            c = edgeSquaredLength * (1 - Vec3_LenSq(&baseToVertex)) + edgeDotBaseToVertex * edgeDotBaseToVertex;

            if (Collision_GetLowestRoot(a, b, c, t, &newT))
            {
                float f = (edgeDotVelocity * newT - edgeDotBaseToVertex) / edgeSquaredLength;
                if (f >= 0.0 && f <= 1.0)
                {
                    t = newT;
                    foundCollison = 1;

                    D3DXVECTOR3 scale = Vec3_Scale(&edge, f);
                    collisionPoint = Vec3_Add(p2, &scale);
                }
            }

            // p3 -> p1:
            edge = Vec3_Sub(p1, p3);
            baseToVertex = Vec3_Sub(p3, &base);
            edgeSquaredLength = Vec3_LenSq(&edge);
            edgeDotVelocity = Vec3_Dot(&edge, &velocity);
            edgeDotBaseToVertex = Vec3_Dot(&edge, &baseToVertex);

            a = edgeSquaredLength * -velocitySquaredLength + edgeDotVelocity * edgeDotVelocity;
            b = edgeSquaredLength * (2 * Vec3_Dot(&velocity, &baseToVertex)) - 2.0 * edgeDotVelocity * edgeDotBaseToVertex;
            c = edgeSquaredLength * (1 - Vec3_LenSq(&baseToVertex)) + edgeDotBaseToVertex * edgeDotBaseToVertex;

            if (Collision_GetLowestRoot(a, b, c, t, &newT))
            {
                float f = (edgeDotVelocity * newT - edgeDotBaseToVertex) / edgeSquaredLength;
                if (f >= 0.0 && f <= 1.0)
                {
                    t = newT;
                    foundCollison = 1;

                    D3DXVECTOR3 scale = Vec3_Scale(&edge, f);
                    collisionPoint = Vec3_Add(p3, &scale);
                }
            }
        }

        // Set result:
        if (foundCollison == 1)
        {
            // distance to collision: ’t’ is time of collision
            float distToCollision = t * Vec3_Len(&colPackage->velocity);

            // Does this triangle qualify for the closest hit?
            // it does if it’s the first hit or the closest
            if (colPackage->foundCollision == 0 || distToCollision < colPackage->nearestDistance)
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
    D3DXVECTOR3 eSpacePosition = Vec3_Div(&colPackage->R3Position, &colPackage->eRadius);
    D3DXVECTOR3 eSpaceVelocity = Vec3_Div(&colPackage->R3Velocity, &colPackage->eRadius);

    // Iterate until we have our final position.
    D3DXVECTOR3 finalPosition = Collision_CollideWithWorld(colPackage, &eSpacePosition, &eSpaceVelocity, 0);

    // Add gravity pull:
    // Set the new R3 position (convert back from eSpace to R3
    colPackage->R3Position = Vec3_Mul(&finalPosition, &colPackage->eRadius);
    colPackage->R3Velocity = *gravity;
    eSpaceVelocity = Vec3_Div(gravity, &colPackage->eRadius);
    finalPosition = Collision_CollideWithWorld(colPackage, &finalPosition, &eSpaceVelocity, 0);

    // Convert final result back to R3:
    finalPosition = Vec3_Mul(&finalPosition, &colPackage->eRadius);

    // Move the entity (application specific function)
    return finalPosition;
}

D3DXVECTOR3 Collision_CollideWithWorld(CollisionPacket *colPackage, const D3DXVECTOR3 *pos, const D3DXVECTOR3 *vel, int collisionRecursionDepth)
{
    // All hard-coded distances in this function is
    // scaled to fit the setting above..
    const float unitsPerMeter = 100.0f;
    float unitScale = unitsPerMeter / 100.0f;
    float veryCloseDistance = 0.005f * unitScale;

    // do we need to worry?
    if (collisionRecursionDepth > 5)
        return *pos;

    // Ok, we need to worry:
    colPackage->velocity = *vel;
    colPackage->normalizedVelocity = Vec3_Norm(vel);
    colPackage->basePoint = *pos;
    colPackage->foundCollision = 0;

    // Check for collision (calls the collision routines)
    // Application specific!!
    for (int i = 0; i < actors[14]->vertexCount; i += 3)
    {
        const Vertex a = AddVertex(actors[14]->vertices[i + 0], actors[14]->position);
        const Vertex b = AddVertex(actors[14]->vertices[i + 1], actors[14]->position);
        const Vertex c = AddVertex(actors[14]->vertices[i + 2], actors[14]->position);

        D3DXVECTOR3 aV = (D3DXVECTOR3){a.x, a.y, a.z};
        D3DXVECTOR3 bV = (D3DXVECTOR3){b.x, b.y, b.z};
        D3DXVECTOR3 cV = (D3DXVECTOR3){c.x, c.y, c.z};

        D3DXVECTOR3 aVd = Vec3_Div(&aV, &colPackage->eRadius);
        D3DXVECTOR3 bVd = Vec3_Div(&bV, &colPackage->eRadius);
        D3DXVECTOR3 cVd = Vec3_Div(&cV, &colPackage->eRadius);

        Collision_CheckTriangle(colPackage, &aVd, &bVd, &cVd);
    }

    for (int i = 0; i < actors[15]->vertexCount; i += 3)
    {
        const Vertex a = AddVertex(actors[15]->vertices[i + 0], actors[15]->position);
        const Vertex b = AddVertex(actors[15]->vertices[i + 1], actors[15]->position);
        const Vertex c = AddVertex(actors[15]->vertices[i + 2], actors[15]->position);

        D3DXVECTOR3 aV = (D3DXVECTOR3){a.x, a.y, a.z};
        D3DXVECTOR3 bV = (D3DXVECTOR3){b.x, b.y, b.z};
        D3DXVECTOR3 cV = (D3DXVECTOR3){c.x, c.y, c.z};

        D3DXVECTOR3 aVd = Vec3_Div(&aV, &colPackage->eRadius);
        D3DXVECTOR3 bVd = Vec3_Div(&bV, &colPackage->eRadius);
        D3DXVECTOR3 cVd = Vec3_Div(&cV, &colPackage->eRadius);

        Collision_CheckTriangle(colPackage, &aVd, &bVd, &cVd);
    }

    // If no collision we just move along the velocity
    if (colPackage->foundCollision == 0)
    {
        return Vec3_Add(pos, vel);
    }

    // *** Collision occured ***
    // The original destination point
    D3DXVECTOR3 destinationPoint = Vec3_Add(pos, vel);
    D3DXVECTOR3 newBasePoint = *pos;

    // only update if we are not already very close
    // and if so we only move very close to intersection..not
    // to the exact spot.
    if (colPackage->nearestDistance >= veryCloseDistance)
    {
        D3DXVECTOR3 V = *vel;
        Vec3_SetLen(&V, colPackage->nearestDistance - veryCloseDistance);
        newBasePoint = Vec3_Add(&colPackage->basePoint, &V);

        // Adjust polygon intersection point (so sliding
        // plane will be unaffected by the fact that we
        // move slightly less than collision tells us)
        V = Vec3_Norm(&V);

        D3DXVECTOR3 scale = Vec3_Scale(&V, veryCloseDistance);
        colPackage->intersectionPoint = Vec3_Sub(&colPackage->intersectionPoint, &scale);
    }

    // Determine the sliding plane
    D3DXVECTOR3 slidePlaneOrigin = colPackage->intersectionPoint;
    D3DXVECTOR3 slidePlaneNormal = Vec3_Sub(&newBasePoint, &colPackage->intersectionPoint);
    slidePlaneNormal = Vec3_Norm(&slidePlaneNormal);
    Plane *slidingPlane = Plane_New(&slidePlaneOrigin, &slidePlaneNormal);

    // Again, sorry about formatting.. but look carefully ;)
    D3DXVECTOR3 scale = Vec3_Scale(&slidePlaneNormal, Plane_SignedDistanceTo(slidingPlane, &destinationPoint));
    D3DXVECTOR3 newDestinationPoint = Vec3_Sub(&destinationPoint, &scale);

    // Generate the slide vector, which will become our new
    // velocity vector for the next iteration
    D3DXVECTOR3 newVelocityVector = Vec3_Sub(&newDestinationPoint, &colPackage->intersectionPoint);

    // Recurse:
    // dont recurse if the new velocity is very small
    if (Vec3_Len(&newVelocityVector) < veryCloseDistance)
    {
        HeapFree(GetProcessHeap(), 0, slidingPlane);
        return newBasePoint;
    }

    HeapFree(GetProcessHeap(), 0, slidingPlane);

    return Collision_CollideWithWorld(colPackage, &newBasePoint, &newVelocityVector, ++collisionRecursionDepth);
}
