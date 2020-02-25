#include <windows.h>
#include <stdio.h>
#include "scene.h"
#include "input.h"
#include "camera.h"
#include "collision.h"

void HandleInput(float deltaTime)
{
  if (GetAsyncKeyState('W')) 
  {
    for (int i = 0; i < actors[0]->vertexCount; i += 3)
    {
      Vertex a = AddVertex(actors[0]->vertices[i + 0], actors[0]->position);
      Vertex b = AddVertex(actors[0]->vertices[i + 1], actors[0]->position);
      Vertex c = AddVertex(actors[0]->vertices[i + 2], actors[0]->position);

      if (TestSphereTriangle(cameraPos, 0.5f, (D3DXVECTOR3) { a.x, a.y, a.z }, 
        (D3DXVECTOR3) { b.x, b.y, b.z }, (D3DXVECTOR3) { c.x, c.y, c.z }))
      {
        return;
      }
    }

    CameraWalk(2.5f * deltaTime);
  }

  if (GetAsyncKeyState('S')) CameraWalk(-2.5f * deltaTime);
  if (GetAsyncKeyState('A')) CameraYaw(-2.0f * deltaTime);
  if (GetAsyncKeyState('D')) CameraYaw(2.0f * deltaTime);
}
