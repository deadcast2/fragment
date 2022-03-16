#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <d3dx9.h>

D3DXVECTOR3 cameraRight;
D3DXVECTOR3 cameraUp;
D3DXVECTOR3 cameraForward;
D3DXVECTOR3 cameraForwardNoPitch;
D3DXVECTOR3 cameraPos;
D3DXMATRIX viewMat;

void InitCamera();
void SetNearClip(float value);
D3DXMATRIX CameraViewMatrix();
void CameraPosition(D3DXVECTOR3 position);
void CameraYaw(float angle);
void CameraPitch(float angle, int limit);
void CameraFly(float units);

#endif
