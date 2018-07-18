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

  BYTE *decompressedData = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
    uncompressedSize);
  fastlz_decompress(resourceData, resourceSize - sizeof(int),
    decompressedData, uncompressedSize);

  BYTE *decompressedDataCopy = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
    uncompressedSize);
  CopyMemory(decompressedDataCopy, decompressedData, uncompressedSize);

  int vertexCount = -1;
  char *context;
  char *line = strtok_r((char *)decompressedData, "\n", &context);
  while(line != NULL)
  {
    vertexCount++;
    line = strtok_r(NULL, "\n", &context);
  }
  HeapFree(GetProcessHeap(), 0, decompressedData);

  line = strtok_r((char *)decompressedDataCopy, "\n", &context);
  struct vertex *vertices = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
    sizeof(struct vertex) * vertexCount);
  int index = 0;
  while(index < vertexCount)
  {
    unsigned int x, y, z, t, v;
    sscanf(line, "%x %x %x %x %x", &x, &y, &z, &t, &v);
    vertices[index] = (struct vertex){
      .x = x / SCALE_FACTOR,
      .y = y / SCALE_FACTOR,
      .z = z / SCALE_FACTOR,
      .t = t / SCALE_FACTOR,
      .v = v / SCALE_FACTOR,
    };
    line = strtok_r(NULL, "\n", &context);
    index++;
  }
  HeapFree(GetProcessHeap(), 0, vertices);
  HeapFree(GetProcessHeap(), 0, decompressedDataCopy);
}
