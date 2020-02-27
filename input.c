#include <windows.h>
#include <stdio.h>
#include "scene.h"
#include "input.h"
#include "camera.h"
#include "collision.h"

void HandleInput(float deltaTime)
{
  D3DXVECTOR3 prevCameraPos = cameraPos;

  if (GetAsyncKeyState('W')) 
  {
    CameraWalk(2.0f * deltaTime);
    
    for (int i = 0; i < actors[0]->vertexCount; i += 3)
    {
      Vertex a = AddVertex(actors[0]->vertices[i + 0], actors[0]->position);
      Vertex b = AddVertex(actors[0]->vertices[i + 1], actors[0]->position);
      Vertex c = AddVertex(actors[0]->vertices[i + 2], actors[0]->position);
      D3DXVECTOR3 p;

      if (TestSphereTriangle(cameraPos, 0.5f, (D3DXVECTOR3) { a.x, a.y, a.z }, 
        (D3DXVECTOR3) { b.x, b.y, b.z }, (D3DXVECTOR3) { c.x, c.y, c.z }, &p))
      {
        p.y = cameraPos.y;

        D3DXVECTOR3 norm;
        D3DXVec3Subtract(&norm, &cameraPos, &p);
        D3DXVec3Normalize(&norm, &norm);

        D3DXVECTOR3 dir;
        D3DXVec3Subtract(&dir, &cameraForward, &norm);
        D3DXVec3Normalize(&dir, &dir);

        D3DXVECTOR3 oldNewDir;
        D3DXVec3Subtract(&oldNewDir, &cameraPos, &prevCameraPos);

        float scalar = sqrt(D3DXVec3Dot(&oldNewDir, &oldNewDir));    
        cameraPos.x -= dir.x * scalar;
        cameraPos.y -= dir.y * scalar;
        cameraPos.z -= dir.z * scalar;
        break;
      }
    } 
  }

  if (GetAsyncKeyState('S')) CameraWalk(-2.0f * deltaTime);
  if (GetAsyncKeyState('A')) CameraYaw(-2.0f * deltaTime);
  if (GetAsyncKeyState('D')) CameraYaw(2.0f * deltaTime);
}
