#include "xaudio2.h"
#include "log.h"
#include <stdio.h>

HRESULT XAudio2Create(IXAudio2 **ppXAudio2)
{
    const GUID CLSID_XAudio2 = {0x5a508685, 0xa254, 0x4fba, {0x9b, 0x82, 0x9a, 0x24, 0xb0, 0x03, 0x06, 0xaf}};
    const GUID IID_IXAudio2 = {0x8bcf1f58, 0x9fe7, 0x4583, {0x8a, 0xc6, 0xe2, 0xad, 0xc4, 0x65, 0xc8, 0xbb}};

    IXAudio2 *pXAudio2;
    HRESULT hr = CoCreateInstance(&CLSID_XAudio2,
      NULL, CLSCTX_INPROC_SERVER, &IID_IXAudio2, (void**)&pXAudio2);
    if(SUCCEEDED(hr))
    {
      Log("ix co created\n");
      hr = pXAudio2->lpVtbl->Initialize(pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
      if(SUCCEEDED(hr))
      {
          Log("hell yeah\n");
          *ppXAudio2 = pXAudio2;
      }
      else
      {
          pXAudio2->lpVtbl->Release(pXAudio2);
      }
  }
  return hr;
}

HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD *dwChunkSize,
  DWORD *dwChunkDataPosition)
{
  HRESULT hr = S_OK;
  if(SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
  {
    char buffer[128];
    sprintf(buffer, "err code:%lu\n", GetLastError());
    Log(buffer);
    return E_FAIL;
  }

  DWORD dwChunkType;
  DWORD dwChunkDataSize;
  DWORD dwRIFFDataSize = 0;
  DWORD dwFileType;
  DWORD bytesRead = 0;
  DWORD dwOffset = 0;

  while(hr == S_OK)
  {
    DWORD dwRead;
    if(ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
      hr = HRESULT_FROM_WIN32(GetLastError());

    if(ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
      hr = HRESULT_FROM_WIN32(GetLastError());

    switch (dwChunkType)
    {
      case FOURCCRIFF:
        dwRIFFDataSize = dwChunkDataSize;
        dwChunkDataSize = 4;
        if(ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
          hr = HRESULT_FROM_WIN32(GetLastError());
        break;
      default:
        if(SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT)
          == INVALID_SET_FILE_POINTER)
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
    if(bytesRead >= dwRIFFDataSize) return S_FALSE;
  }
  return S_OK;
}

HRESULT ReadChunkData(HANDLE hFile, void *buffer, DWORD buffersize,
  DWORD bufferoffset)
{
  HRESULT hr = S_OK;
  if(SetFilePointer(hFile, bufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
    return HRESULT_FROM_WIN32(GetLastError());
  DWORD dwRead;
  if(ReadFile(hFile, buffer, buffersize, &dwRead, NULL) == 0)
    hr = HRESULT_FROM_WIN32(GetLastError());
  return hr;
}
