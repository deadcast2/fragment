#ifndef _SCENE_H_
#define _SCENE_H_

#include "graphics.h"
#include "camera.h"
#include "model.h"

IDirect3DVertexBuffer9 *testVertexBuffer;
int testVertexCount;
struct vertex *testVertices;

void InitScene();
void RenderScene();
void CleanScene();

#endif
