#ifndef _SCENE_H_
#define _SCENE_H_

#include "graphics.h"
#include "camera.h"
#include "model.h"
#include "texture.h"
#include "actor.h"

struct actor *testActor;

void InitScene();
void RenderScene();
void CleanScene();

#endif
