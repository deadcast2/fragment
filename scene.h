#ifndef _SCENE_H_
#define _SCENE_H_

#include "actor.h"
#include "audio.h"
#include "camera.h"
#include "collision.h"
#include "graphics.h"
#include "model.h"
#include "texture.h"
#include <float.h>
#include <math.h>

#define ACTOR_COUNT 18

Actor *actors[ACTOR_COUNT];

void InitScene();
void RenderScene(float deltaTime);
void CleanScene();

#endif
