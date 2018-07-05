#include "audio.h"

void InitAudio(HWND hWnd)
{
  DirectSoundCreate8(NULL, &dSound, NULL);
  dSound->lpVtbl->SetCooperativeLevel(dSound, hWnd, DSSCL_PRIORITY);
  DSBUFFERDESC bufferDesc = {
    .dwSize = sizeof(DSBUFFERDESC),
    .dwFlags = DSBCAPS_PRIMARYBUFFER,
    .dwBufferBytes = 0,
    .lpwfxFormat = NULL
  };
  dSound->lpVtbl->CreateSoundBuffer(dSound, &bufferDesc, &dSoundBuffer, NULL);

  WAVEFORMATEX wfx = {
    .wFormatTag = WAVE_FORMAT_PCM,
    .nChannels = 2,
    .nSamplesPerSec = 22050,
    .wBitsPerSample = 16,
    .nBlockAlign = wfx.wBitsPerSample / 8 * wfx.nChannels,
    .nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign
  };
  dSoundBuffer->lpVtbl->SetFormat(dSoundBuffer, &wfx);
}

void LoadTestAudio()
{
  HRSRC hResInfo = FindResource(NULL, "IDR_WAVE1", "WAV");
  HGLOBAL hResData = LoadResource(NULL, hResInfo);
  DWORD dwSize = SizeofResource(NULL, hResInfo);
  VOID *pvRes = LockResource(hResData);
  HANDLE heap = HeapCreate(0, 0, 0);
  VOID *heapBlock = HeapAlloc(heap, HEAP_ZERO_MEMORY, dwSize);
  memcpy(heapBlock, pvRes, dwSize);
  MMIOINFO mmioInfo = {
    .fccIOProc = FOURCC_MEM,
    .cchBuffer = dwSize,
    .pchBuffer = heapBlock
  };
  HMMIO mmio = mmioOpen(NULL, &mmioInfo, MMIO_ALLOCBUF | MMIO_READ);

  MMCKINFO ckRiff;
  mmioDescend(mmio, &ckRiff, NULL, 0);
  if(ckRiff.fccType == mmioFOURCC('W', 'A','V', 'E'))
  {
    Log("reading valid wav file\n");
  }

  mmioClose(mmio, 0);
  HeapFree(heap, 0, heapBlock);
  Log("cleaned up audio test\n");
}

void CleanAudio()
{
  dSoundBuffer->lpVtbl->Release(dSoundBuffer);
  dSound->lpVtbl->Release(dSound);
}
