#ifndef _SCENE_H_
#define _SCENE_H_

#include "graphics.h"
#include "camera.h"
#include "model.h"
#include "texture.h"
#include "actor.h"

#define ACTOR_COUNT 2

struct actor *actors[ACTOR_COUNT];

void InitScene();
void RenderScene();
void CleanScene();

#endif
