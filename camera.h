#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <d3dx9.h>

D3DXVECTOR3 cameraRight;
D3DXVECTOR3 cameraUp;
D3DXVECTOR3 cameraForward;
D3DXVECTOR3 cameraPos;
D3DXMATRIX viewMat;

void InitCamera();
D3DXMATRIX CameraViewMatrix();
void CameraPosition(D3DXVECTOR3 position);
void CameraWalk(float units);
void CameraYaw(float angle);
void CameraPitch(float angle);
void CameraFly(float units);

#endif
