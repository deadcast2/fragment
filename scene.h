#ifndef _SCENE_H_
#define _SCENE_H_

#include <float.h>
#include "audio.h"
#include "graphics.h"
#include "camera.h"
#include "model.h"
#include "texture.h"
#include "actor.h"
#include "srandom.h"

#define ACTOR_COUNT 3

Actor *actors[ACTOR_COUNT];

void InitScene();
void RenderScene(float deltaTime);
void CleanScene();

#endif
