#include "audio.h"
#include "Log.h"
#include <stdio.h>

void InitAudio()
{
  if(FAILED(XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR)))
  {
    Log("Failed to create XAudio2 instance\n");
    return;
  }

  if(FAILED(xAudio2->lpVtbl->CreateMasteringVoice(xAudio2, &xMasterVoice,
    XAUDIO2_DEFAULT_CHANNELS, XAUDIO2_DEFAULT_SAMPLERATE, 0, NULL, NULL,
    AudioCategory_GameEffects)))
  {
    Log("Failed to create XAudio2 mastering voice\n");
    return;
  }

  LoadTestAudio();
}

HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD *dwChunkSize,
  DWORD *dwChunkDataPosition)
{
  HRESULT hr = S_OK;
  if(INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
    return HRESULT_FROM_WIN32(GetLastError());

  DWORD dwChunkType;
  DWORD dwChunkDataSize;
  DWORD dwRIFFDataSize = 0;
  DWORD dwFileType;
  DWORD bytesRead = 0;
  DWORD dwOffset = 0;

  while(hr == S_OK)
  {
    DWORD dwRead;
    if(0 == ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL))
      hr = HRESULT_FROM_WIN32(GetLastError());

    if(0 == ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL))
      hr = HRESULT_FROM_WIN32(GetLastError());

    switch(dwChunkType)
    {
      case fourccRIFF:
        dwRIFFDataSize = dwChunkDataSize;
        dwChunkDataSize = 4;
        if(0 == ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL))
          hr = HRESULT_FROM_WIN32(GetLastError());
        break;
      default:
        if(INVALID_SET_FILE_POINTER == SetFilePointer(hFile, dwChunkDataSize,
          NULL, FILE_CURRENT))
          return HRESULT_FROM_WIN32(GetLastError());
    }

    dwOffset += sizeof(DWORD) * 2;

    if(dwChunkType == fourcc)
    {
      *dwChunkSize = dwChunkDataSize;
      *dwChunkDataPosition = dwOffset;
      return S_OK;
    }

    dwOffset += dwChunkDataSize;

    if (bytesRead >= dwRIFFDataSize) return S_FALSE;
  }
  return S_OK;
}

HRESULT ReadChunkData(HANDLE hFile, void *buffer, DWORD buffersize,
  DWORD bufferoffset)
{
  HRESULT hr = S_OK;
  if(INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferoffset, NULL,
    FILE_BEGIN))
        return HRESULT_FROM_WIN32( GetLastError() );
  DWORD dwRead;
  if(0 == ReadFile(hFile, buffer, buffersize, &dwRead, NULL))
    hr = HRESULT_FROM_WIN32(GetLastError());
  return hr;
}

HRESULT LoadTestAudio()
{
  WAVEFORMATEXTENSIBLE wfx = {0};
  XAUDIO2_BUFFER buffer = {0};
  HANDLE hFile = CreateFile(TEXT("wind.wav"), GENERIC_READ, FILE_SHARE_READ,
    NULL, OPEN_EXISTING, 0, NULL);

  if(INVALID_HANDLE_VALUE == hFile)
    return HRESULT_FROM_WIN32(GetLastError());

  if(INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
    return HRESULT_FROM_WIN32(GetLastError());

  DWORD dwChunkSize;
  DWORD dwChunkPosition;
  DWORD filetype;
  FindChunk(hFile, fourccRIFF, &dwChunkSize, &dwChunkPosition);
  ReadChunkData(hFile, &filetype, sizeof(DWORD), dwChunkPosition);
  if(filetype != fourccWAVE) return S_FALSE;

  FindChunk(hFile, fourccFMT, &dwChunkSize, &dwChunkPosition);
  ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);

  FindChunk(hFile, fourccDATA, &dwChunkSize, &dwChunkPosition);
  BYTE *pDataBuffer = HeapAlloc(GetProcessHeap(), 0, dwChunkSize);
  ReadChunkData(hFile, pDataBuffer, dwChunkSize, dwChunkPosition);

  buffer.AudioBytes = dwChunkSize;
  buffer.pAudioData = pDataBuffer;
  buffer.Flags = XAUDIO2_END_OF_STREAM;

  if(FAILED(xAudio2->lpVtbl->CreateSourceVoice(xAudio2, &xSourceVoice,
    (WAVEFORMATEX*)&wfx, 0, XAUDIO2_DEFAULT_FREQ_RATIO, NULL, NULL, NULL)))
    return S_FALSE;

  if(FAILED(xSourceVoice->lpVtbl->SubmitSourceBuffer(xSourceVoice,
    &buffer, NULL)))
    return S_FALSE;

  if(FAILED(xSourceVoice->lpVtbl->Start(xSourceVoice, 0, XAUDIO2_COMMIT_NOW)))
    return S_FALSE;

  return S_OK;
}

void CleanAudio()
{
  xSourceVoice->lpVtbl->DestroyVoice(xSourceVoice);
  xMasterVoice->lpVtbl->DestroyVoice(xMasterVoice);
  xAudio2->lpVtbl->Release(xAudio2);
}
