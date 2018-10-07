#include "actor.h"

struct actor *CreateActor(struct actorProps props)
{
  struct actor *newActor = HeapAlloc(GetProcessHeap(), 0, sizeof(struct actor));
  if (!newActor) return newActor;

  newActor->vertexBuffer = 0;
  newActor->d3dTexture = 0;
  newActor->position = props.position;
  newActor->rotation = props.rotation;
  newActor->scale = props.scale;

  if (props.modelName)
  {
    struct vertex *vertices = LoadModel(props.modelName, &newActor->vertexCount);
    d3ddev->lpVtbl->CreateVertexBuffer(
      d3ddev, newActor->vertexCount * sizeof(struct vertex), 0,
      CUSTOMFVF, D3DPOOL_MANAGED, &newActor->vertexBuffer, 0);

    VOID *pVoid;
    newActor->vertexBuffer->lpVtbl->Lock(newActor->vertexBuffer, 0,
      0, (void**)&pVoid, 0);
    CopyMemory(pVoid, vertices, newActor->vertexCount * sizeof(struct vertex));
    newActor->vertexBuffer->lpVtbl->Unlock(newActor->vertexBuffer);
    HeapFree(GetProcessHeap(), 0, vertices);
  }

  if (props.textureName)
  {
    LoadTexture(props.textureName, &newActor->d3dTexture);
  }

  return newActor;
}

void DeleteActor(struct actor *actor)
{
  if (!actor) return;
  if (actor->vertexBuffer) actor->vertexBuffer->lpVtbl->Release(actor->vertexBuffer);
  if (actor->d3dTexture) actor->d3dTexture->lpVtbl->Release(actor->d3dTexture);
  HeapFree(GetProcessHeap(), 0, actor);
}
