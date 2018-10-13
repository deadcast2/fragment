#ifndef _SCENE_H_
#define _SCENE_H_

#include "graphics.h"
#include "camera.h"
#include "model.h"
#include "texture.h"
#include "actor.h"
#include "srandom.h"

#define ACTOR_COUNT 2

struct actor *actors[ACTOR_COUNT];
int randomFogEnd, lastFogEnd;
float fogTime, fogStep, fogSpeed;

void InitScene();
void RenderScene(float deltaTime);
void CleanScene();

#endif
