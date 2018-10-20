#include "actor.h"

Actor *CreateActor(ActorParams params)
{
  Actor *newActor = HeapAlloc(GetProcessHeap(), 0, sizeof(Actor));
  if (!newActor) return newActor;

  newActor->vertexBuffer = 0;
  newActor->d3dTexture = 0;
  newActor->audioSource = 0;
  newActor->audioBuffer = 0;
  newActor->position = params.position;
  newActor->rotation = params.rotation;
  newActor->scale = params.scale;
  newActor->Update = 0;

  if (params.modelName) LoadModel(params.modelName, &newActor->vertexCount, &newActor->vertexBuffer);
  if (params.textureName) LoadTexture(params.textureName, &newActor->d3dTexture);
  if (params.audioName) LoadAudio(params.audioName, &newActor->audioSource,
     &newActor->audioBuffer, params.audioParams);
  if (params.Update) newActor->Update = params.Update;
  if (params.Start) params.Start(newActor);

  return newActor;
}

void DeleteActor(Actor *actor)
{
  if (!actor) return;
  if (actor->vertexBuffer) actor->vertexBuffer->lpVtbl->Release(actor->vertexBuffer);
  if (actor->d3dTexture) actor->d3dTexture->lpVtbl->Release(actor->d3dTexture);
  if (actor->audioSource) actor->audioSource->lpVtbl->DestroyVoice(actor->audioSource);
  if (actor->audioBuffer) HeapFree(GetProcessHeap(), 0, actor->audioBuffer);
  HeapFree(GetProcessHeap(), 0, actor);
}

void DrawActor(Actor *actor, ID3DXMatrixStack *stack,
  LPDIRECT3DDEVICE9 d3ddev, LPD3DXEFFECT effect, D3DXHANDLE texture, float deltaTime)
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
  effect->lpVtbl->SetMatrix(effect, "World", &actorMat);
  //d3ddev->lpVtbl->SetTransform(d3ddev, D3DTS_VIEW, stack->lpVtbl->GetTop(stack));
  effect->lpVtbl->SetTexture(effect, texture, (IDirect3DBaseTexture9*)actor->d3dTexture);
  //d3ddev->lpVtbl->SetFVF(d3ddev, CUSTOMFVF);
  d3ddev->lpVtbl->SetStreamSource(d3ddev, 0, actor->vertexBuffer,
    0, sizeof(Vertex));

  effect->lpVtbl->Begin(effect, 0, 0);
  effect->lpVtbl->BeginPass(effect, 0);

  d3ddev->lpVtbl->DrawPrimitive(d3ddev, D3DPT_TRIANGLELIST, 0,
    actor->vertexCount / 3);


  effect->lpVtbl->EndPass(effect);
  effect->lpVtbl->End(effect);

  stack->lpVtbl->Pop(stack);

  if (actor->Update) actor->Update(actor, deltaTime);
}
