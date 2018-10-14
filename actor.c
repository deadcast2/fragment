#include "actor.h"

struct actor *CreateActor(struct actorProps props)
{
  struct actor *newActor = HeapAlloc(GetProcessHeap(), 0, sizeof(struct actor));
  if (!newActor) return newActor;

  newActor->vertexBuffer = 0;
  newActor->d3dTexture = 0;
  newActor->audioSource = 0;
  newActor->position = props.position;
  newActor->rotation = props.rotation;
  newActor->scale = props.scale;
  newActor->update = 0;

  if (props.modelName) LoadModel(props.modelName, &newActor->vertexCount, &newActor->vertexBuffer);
  if (props.textureName) LoadTexture(props.textureName, &newActor->d3dTexture);
  if (props.audioName) LoadAudio(props.audioName, &newActor->audioSource, props.audioProps);
  if (props.update) newActor->update = props.update;
  if (props.start) props.start(newActor);

  return newActor;
}

void DeleteActor(struct actor *actor)
{
  if (!actor) return;
  if (actor->vertexBuffer) actor->vertexBuffer->lpVtbl->Release(actor->vertexBuffer);
  if (actor->d3dTexture) actor->d3dTexture->lpVtbl->Release(actor->d3dTexture);
  if (actor->audioSource) actor->audioSource->lpVtbl->DestroyVoice(actor->audioSource);
  HeapFree(GetProcessHeap(), 0, actor);
}

void DrawActor(struct actor *actor, ID3DXMatrixStack *stack,
  LPDIRECT3DDEVICE9 d3ddev, float deltaTime)
{
  D3DXMATRIX translation;
  D3DXMatrixTranslation(&translation, actor->position.x,
    actor->position.y, actor->position.z);

  D3DXMATRIX rotation;
  D3DXMatrixRotationYawPitchRoll(&rotation, actor->rotation.x,
    actor->rotation.y, actor->rotation.z);

  D3DXMATRIX scale;
  D3DXMatrixScaling(&scale, actor->scale.x, actor->scale.y, actor->scale.z);

  D3DXMATRIX actorMat;
  D3DXMatrixMultiply(&actorMat, &scale, &rotation);
  D3DXMatrixMultiply(&actorMat, &actorMat, &translation);

  stack->lpVtbl->Push(stack);
  stack->lpVtbl->MultMatrixLocal(stack, &actorMat);
  d3ddev->lpVtbl->SetTransform(d3ddev, D3DTS_VIEW, stack->lpVtbl->GetTop(stack));
  d3ddev->lpVtbl->SetTexture(d3ddev, 0, (IDirect3DBaseTexture9*)actor->d3dTexture);
  d3ddev->lpVtbl->SetFVF(d3ddev, CUSTOMFVF);
  d3ddev->lpVtbl->SetStreamSource(d3ddev, 0, actor->vertexBuffer,
    0, sizeof(struct vertex));
  d3ddev->lpVtbl->DrawPrimitive(d3ddev, D3DPT_TRIANGLELIST, 0,
    actor->vertexCount / 3);
  stack->lpVtbl->Pop(stack);

  if (actor->update) actor->update(actor, deltaTime);
}
