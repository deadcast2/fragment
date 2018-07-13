#include <windows.h>
#include "input.h"
#include "camera.h"

void HandleInput(float deltaTime)
{
  if(GetAsyncKeyState('W')) CameraWalk(3.0f * deltaTime);
  if(GetAsyncKeyState('S')) CameraWalk(-3.0f * deltaTime);
  if(GetAsyncKeyState('A')) CameraYaw(-2.0f * deltaTime);
  if(GetAsyncKeyState('D')) CameraYaw(2.0f * deltaTime);
}
