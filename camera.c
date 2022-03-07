#include "camera.h"
#include "graphics.h"
#include "srandom.h"
#include "vec3.h"

void InitCamera() {
  cameraRight = (D3DXVECTOR3){1, 0, 0};
  cameraUp = (D3DXVECTOR3){0, 1, 0};
  cameraForward = (D3DXVECTOR3){0, 0, 1};
  cameraForwardNoPitch = (D3DXVECTOR3){0, 0, 1};
  cameraPos = (D3DXVECTOR3){0, 0, 0};

  const float aspect = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;
  const float fov = D3DX_PI / 2;
  D3DXMatrixPerspectiveFovLH(&viewMat, fov, aspect, 0.1f, 1000.0f);
}

D3DXMATRIX CameraViewMatrix() {
  D3DXVec3Normalize(&cameraForward, &cameraForward);
  D3DXVec3Cross(&cameraUp, &cameraForward, &cameraRight);
  D3DXVec3Normalize(&cameraUp, &cameraUp);
  D3DXVec3Cross(&cameraRight, &cameraUp, &cameraForward);
  D3DXVec3Normalize(&cameraRight, &cameraRight);

  D3DXMATRIX view;
  view.m[0][0] = cameraRight.x;
  view.m[0][1] = cameraUp.x;
  view.m[0][2] = cameraForward.x;
  view.m[0][3] = 0.0f;
  view.m[1][0] = cameraRight.y;
  view.m[1][1] = cameraUp.y;
  view.m[1][2] = cameraForward.y;
  view.m[1][3] = 0.0f;
  view.m[2][0] = cameraRight.z;
  view.m[2][1] = cameraUp.z;
  view.m[2][2] = cameraForward.z;
  view.m[2][3] = 0.0f;
  view.m[3][0] = -D3DXVec3Dot(&cameraRight, &cameraPos);
  view.m[3][1] = -D3DXVec3Dot(&cameraUp, &cameraPos);
  view.m[3][2] = -D3DXVec3Dot(&cameraForward, &cameraPos);
  view.m[3][3] = 1.0f;
  return view;
}

void CameraPosition(D3DXVECTOR3 position) { cameraPos = position; }

void CameraWalk(float units) {
  cameraPos.x += cameraForward.x * units;
  cameraPos.y += cameraForward.y * units;
  cameraPos.z += cameraForward.z * units;
}

void CameraYaw(float angle) {
  D3DXMATRIX T;
  D3DXMatrixRotationY(&T, angle);
  D3DXVec3TransformCoord(&cameraRight, &cameraRight, &T);
  D3DXVec3TransformCoord(&cameraForward, &cameraForward, &T);
  D3DXVec3TransformCoord(&cameraForwardNoPitch, &cameraForwardNoPitch, &T);
}

void CameraPitch(float angle, int limit) {
  D3DXMATRIX T;
  D3DXMatrixRotationAxis(&T, &cameraRight, angle);
  D3DXVec3TransformCoord(&cameraUp, &cameraUp, &T);

  D3DXVECTOR3 oldCameraForward = cameraForward;
  D3DXVec3TransformCoord(&cameraForward, &cameraForward, &T);

  if (limit) {
    // Limit how for the camera can look up or down.
    const float dot = Vec3_Dot(&cameraForwardNoPitch, &cameraForward);
    if (dot < 0.36f) {
      cameraForward = oldCameraForward;
    }
  }
}

void CameraFly(float units) { cameraPos.y += units; }
