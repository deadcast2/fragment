#include "model.h"

void LoadModel(const char *name)
{
  HANDLE resource = FindResource(NULL, name, "SOS");
  if(resource == NULL) return Log("Resource model not found\n");
  HGLOBAL loadedResource = LoadResource(NULL, resource);
  if(loadedResource == NULL) return Log("Could not load model resource\n");
  LPVOID resourceData = LockResource(loadedResource);
  DWORD resourceSize = SizeofResource(NULL, resource);

  int uncompressedSize = 0;
  memmove(&uncompressedSize, resourceData, sizeof(int));
  memmove(resourceData, resourceData + sizeof(int), resourceSize - sizeof(int));

  BYTE *decompressedData = HeapAlloc(GetProcessHeap(), 0, uncompressedSize);
  fastlz_decompress(resourceData, resourceSize - sizeof(int),
    decompressedData, uncompressedSize);

  int vertexCount = -1;
  char *context;
  char *line = strtok_r((char *)decompressedData, "\n", &context);
  while(line != NULL)
  {
    vertexCount++;
    line = strtok_r(NULL, "\n", &context);
  }

  HeapFree(GetProcessHeap(), 0, decompressedData);
}
