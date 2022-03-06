#include "actor.h"

Actor *CreateActor(ActorParams params) {
  Actor *newActor =
      HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(Actor));
  if (!newActor)
    return newActor;

  newActor->bufferType = params.bufferType;
  newActor->vertexBuffer = 0;
  newActor->d3dTexture = 0;
  newActor->audioSource = 0;
  newActor->audioBuffer = 0;
  newActor->effect = 0;
  newActor->position = params.position;
  newActor->rotation = params.rotation;
  newActor->scale = params.scale;
  newActor->enabled = params.enabled;
  newActor->name = params.name;
  newActor->Start = 0;
  newActor->Update = 0;
  newActor->screenSpaceOnly = params.screenSpaceOnly;

  if (params.modelName)
    LoadModel(params.modelName, &newActor->vertexCount, &newActor->vertices,
              &newActor->vertexBuffer);

  if (params.textureName)
    LoadTexture(params.textureName, &newActor->d3dTexture);

  if (params.audioName)
    LoadAudio(params.audioName, &newActor->audioSource, &newActor->audioBuffer,
              params.audioParams);

  if (params.effectName)
    LoadEffect(params.effectName, &newActor->effect);

  if (params.Update)
    newActor->Update = params.Update;

  if (params.Start)
    newActor->Start = params.Start;

  if (params.Start && params.enabled)
    newActor->Start(newActor);

  return newActor;
}

void DeleteActor(Actor *actor) {
  if (!actor)
    return;

  if (actor->vertices)
    HeapFree(GetProcessHeap(), 0, actor->vertices);

  if (actor->vertexBuffer)
    actor->vertexBuffer->lpVtbl->Release(actor->vertexBuffer);

  if (actor->d3dTexture)
    actor->d3dTexture->lpVtbl->Release(actor->d3dTexture);

  if (actor->audioSource)
    actor->audioSource->lpVtbl->DestroyVoice(actor->audioSource);

  if (actor->audioBuffer) {
    HeapFree(GetProcessHeap(), 0, (void *)actor->audioBuffer->pAudioData);
    HeapFree(GetProcessHeap(), 0, actor->audioBuffer);
  }

  if (actor->effect)
    actor->effect->lpVtbl->Release(actor->effect);

  HeapFree(GetProcessHeap(), 0, actor);
}

void DrawActor(Actor *actor, LPDIRECT3DDEVICE9 d3ddev, float deltaTime) {
  if (!actor->enabled)
    return;

  D3DXMATRIX translation;
  D3DXMatrixTranslation(&translation, actor->position.x, actor->position.y,
                        actor->position.z);

  D3DXMATRIX rotation;
  D3DXMatrixRotationYawPitchRoll(&rotation, actor->rotation.x,
                                 actor->rotation.y, actor->rotation.z);

  D3DXMATRIX scale;
  D3DXMatrixScaling(&scale, actor->scale.x, actor->scale.y, actor->scale.z);

  D3DXMATRIX actorMat;
  D3DXMatrixMultiply(&actorMat, &scale, &rotation);
  D3DXMatrixMultiply(&actorMat, &actorMat, &translation);

  if (actor->Update)
    actor->Update(actor, deltaTime);

  if (actor->effect) {
    D3DXMATRIX worldMat;
    D3DXMatrixIdentity(&worldMat);

    if (!actor->screenSpaceOnly) {
      D3DXMATRIX camMat = CameraViewMatrix();
      D3DXMatrixMultiply(&worldMat, &actorMat, &camMat);
      D3DXMatrixMultiply(&worldMat, &worldMat, &viewMat);
    } else {
      D3DXMatrixMultiply(&worldMat, &worldMat, &actorMat);
    }

    actor->effect->lpVtbl->SetMatrix(actor->effect, "_ObjectToWorld",
                                     &worldMat);
    actor->effect->lpVtbl->SetTexture(
        actor->effect, "_Texture", (IDirect3DBaseTexture9 *)actor->d3dTexture);
    actor->effect->lpVtbl->SetVector(
        actor->effect, "_CameraPos",
        &(D3DXVECTOR4){cameraPos.x, cameraPos.y, cameraPos.z, 1});
    actor->effect->lpVtbl->SetVector(actor->effect, "_ObjectPos",
                                     &(D3DXVECTOR4){actor->position.x,
                                                    actor->position.y,
                                                    actor->position.z, 1});
    actor->effect->lpVtbl->SetFloat(actor->effect, "_FogStart",
                                    RenderSettings.fogStart);
    actor->effect->lpVtbl->SetFloat(actor->effect, "_FogEnd",
                                    RenderSettings.fogEnd);
    actor->effect->lpVtbl->SetVector(
        actor->effect, "_FogColor",
        &(D3DXVECTOR4){RenderSettings.fogColor[0], RenderSettings.fogColor[1],
                       RenderSettings.fogColor[2], 1});
    actor->effect->lpVtbl->SetFloat(
        actor->effect, "_Time", (startupTime - (double)timeGetTime()) * 0.001f);
    actor->effect->lpVtbl->Begin(actor->effect, 0, 0);
    actor->effect->lpVtbl->BeginPass(actor->effect, 0);
  }

  if (actor->vertexBuffer) {
    d3ddev->lpVtbl->SetStreamSource(d3ddev, 0, actor->vertexBuffer, 0,
                                    sizeof(Vertex));
    d3ddev->lpVtbl->SetVertexDeclaration(d3ddev, vertDeclaration);

    if (actor->bufferType == Triangle) {
      d3ddev->lpVtbl->DrawPrimitive(d3ddev, D3DPT_TRIANGLELIST, 0,
                                    actor->vertexCount / 3);
    } else {
      d3ddev->lpVtbl->DrawPrimitive(d3ddev, D3DPT_LINELIST, 0,
                                    actor->vertexCount / 2);
    }
  }

  if (actor->effect) {
    actor->effect->lpVtbl->EndPass(actor->effect);
    actor->effect->lpVtbl->End(actor->effect);
  }
}
