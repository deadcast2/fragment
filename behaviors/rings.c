#include "..\scene.h"
#include "rings.h"

void RingStart(Actor *self)
{
    self->position.z = 2.5f;
    self->scale.x *= 0.01f;
    self->scale.y *= 0.007f;
    self->scale.z *= 0.01f;
    self->effect->lpVtbl->SetBool(self->effect, "_IgnoreTexture", TRUE);

    if (strcmp(self->name, "ring 1") == 0)
    {
        self->effect->lpVtbl->SetVector(self->effect, "_Color", &(D3DXVECTOR4){1, 0, 0, 1});
    }
    else if (strcmp(self->name, "ring 2") == 0)
    {
        self->effect->lpVtbl->SetVector(self->effect, "_Color", &(D3DXVECTOR4){0, 1, 0, 1});
    }
    else if (strcmp(self->name, "ring 3") == 0)
    {
        self->effect->lpVtbl->SetVector(self->effect, "_Color", &(D3DXVECTOR4){0, 0, 1, 1});
    }
    else if (strcmp(self->name, "ring 4") == 0)
    {
        self->effect->lpVtbl->SetVector(self->effect, "_Color", &(D3DXVECTOR4){1, 1, 0, 1});
    }
    else if (strcmp(self->name, "ring 5") == 0)
    {
        self->effect->lpVtbl->SetVector(self->effect, "_Color", &(D3DXVECTOR4){1, 0, 1, 1});
    }
}

void Ring1Update(Actor *self, float deltaTime)
{
    // Show the title for a little bit.
    static float titleDelay = 5.8f;

    if (titleDelay < 0 && titleDelay > -5.0f)
    {
        static RECT textbox;
        SetRect(&textbox, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        d3dFont->lpVtbl->DrawTextA(d3dFont, NULL, "f r a g m e n t", 15, &textbox, DT_CENTER | DT_VCENTER,
                                   D3DCOLOR_ARGB(255, 255, 255, 255));
    }

    titleDelay -= deltaTime;

    static float delay = 0.0f;
    static int cycles = CYCLE_COUNT;

    RingDelayHelper(self, &delay, &cycles, deltaTime);
}

void Ring2Update(Actor *self, float deltaTime)
{
    static float delay = 0.5f;
    static int cycles = CYCLE_COUNT;

    RingDelayHelper(self, &delay, &cycles, deltaTime);
}

void Ring3Update(Actor *self, float deltaTime)
{
    static float delay = 1.0f;
    static int cycles = CYCLE_COUNT;

    RingDelayHelper(self, &delay, &cycles, deltaTime);
}

void Ring4Update(Actor *self, float deltaTime)
{
    static float delay = 1.5f;
    static int cycles = CYCLE_COUNT;

    RingDelayHelper(self, &delay, &cycles, deltaTime);
}

void Ring5Update(Actor *self, float deltaTime)
{
    static float delay = 2.0f;
    static int cycles = CYCLE_COUNT;

    if (RingDelayHelper(self, &delay, &cycles, deltaTime))
    {
        // Intro finished so activate all other actors.
        for (int i = 0; i < ACTOR_COUNT; i++)
        {
            actors[i]->enabled = strstr(actors[i]->name, "ring") == NULL;

            if (actors[i]->Start && actors[i]->enabled)
            {
                actors[i]->Start(actors[i]);
            }
        }
    }
}

void RingGongUpdate(Actor *self, float deltaTime)
{
    static float nextGong = 0;

    if (nextGong < 0)
    {
        PlayAudio(self->audioSource, self->audioBuffer, 1.0f);

        nextGong = 5;
    }

    nextGong -= deltaTime;
}

BOOL RingDelayHelper(Actor *actor, float *delay, int *cycles, float deltaTime)
{
    static float sinX = 0;

    if (*delay <= 0)
    {
        actor->position.x -= actor->position.x * deltaTime;
        actor->position.y -= actor->position.y * deltaTime;
        actor->position.z -= 0.5f * deltaTime;
    }

    *delay -= 0.5f * deltaTime;

    if (actor->position.z < 0)
    {
        if (*cycles > 1)
        {
            actor->position.z = 2.5f;
            actor->position.x = -sin((double)sinX);
            actor->position.y = sin((double)sinX);
            sinX += 1.0f * deltaTime;
        }

        *cycles -= 1;
    }

    return *cycles == 0;
}
