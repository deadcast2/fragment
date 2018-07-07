#ifndef _XAUDIO2_H_
#define _XAUDIO2_H_

#include <windows.h>
#include <mmreg.h>

#define XAUDIO2_DEFAULT_CHANNELS 0
#define XAUDIO2_DEFAULT_SAMPLERATE 0
#define FOURCCRIFF 'RIFF'
#define FOURCCWAVE 'WAVE'

typedef enum XAUDIO2_DEVICE_ROLE
{
  NotDefaultDevice = 0x0,
  DefaultConsoleDevice = 0x1,
  DefaultMultimediaDevice = 0x2,
  DefaultCommunicationsDevice = 0x4,
  DefaultGameDevice = 0x8,
  GlobalDefaultDevice = 0xf,
  InvalidDeviceRole = ~GlobalDefaultDevice
} XAUDIO2_DEVICE_ROLE;

typedef struct XAUDIO2_DEVICE_DETAILS
{
  WCHAR DeviceID[256];
  WCHAR DisplayName[256];
  XAUDIO2_DEVICE_ROLE Role;
  WAVEFORMATEXTENSIBLE OutputFormat;
} XAUDIO2_DEVICE_DETAILS;

typedef enum XAUDIO2_WINDOWS_PROCESSOR_SPECIFIER
{
  XAUDIO2_ANY_PROCESSOR = 0xffffffff,
  XAUDIO2_DEFAULT_PROCESSOR = XAUDIO2_ANY_PROCESSOR
} XAUDIO2_WINDOWS_PROCESSOR_SPECIFIER, XAUDIO2_PROCESSOR;

typedef struct XAUDIO2_EFFECT_DESCRIPTOR
{
  IUnknown *pEffect;
  BOOL InitialState;
  UINT32 OutputChannels;
} XAUDIO2_EFFECT_DESCRIPTOR;

typedef struct XAUDIO2_EFFECT_CHAIN
{
  UINT32 EffectCount;
  XAUDIO2_EFFECT_DESCRIPTOR *pEffectDescriptors;
} XAUDIO2_EFFECT_CHAIN;

typedef struct IXAudio2MasteringVoice
{
    struct IXAudio2MasteringVoice *lpVtbl;
} IXAudio2MasteringVoice;
typedef struct IXAudio2MasteringVoiceVtbl IXAudio2MasteringVoiceVtbl;
struct IXAudio2MasteringVoiceVtbl
{
};

typedef struct IXAudio2
{
    struct IXAudio2Vtbl *lpVtbl;
} IXAudio2;
typedef struct IXAudio2Vtbl IXAudio2Vtbl;
struct IXAudio2Vtbl
{
  ULONG (__stdcall *Release)(IXAudio2 FAR *This);
  HRESULT (__stdcall *Initialize)(IXAudio2 FAR *This, UINT32 Flags,
    XAUDIO2_PROCESSOR XAudio2Processor);
  HRESULT (__stdcall *CreateMasteringVoice)(IXAudio2 FAR *This,
    IXAudio2MasteringVoice **ppMasteringVoice, UINT32 InputChannels,
    UINT32 InputSampleRate, UINT32 Flags, UINT32 DeviceIndex,
    const XAUDIO2_EFFECT_CHAIN *pEffectChain);
};

HRESULT XAudio2Create(IXAudio2 **ppXAudio2);
HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD *dwChunkSize,
  DWORD *dwChunkDataPosition);
HRESULT ReadChunkData(HANDLE hFile, void *buffer, DWORD buffersize,
  DWORD bufferoffset);

#endif
