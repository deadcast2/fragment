#include "model.h"

void LoadModel(const char *name, int *vertexCount, IDirect3DVertexBuffer9 **vertexBuffer)
{
  HANDLE resource = FindResource(NULL, name, "SOS");
  if(resource == NULL) return;
  HGLOBAL loadedResource = LoadResource(NULL, resource);
  if(loadedResource == NULL) return;
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

  int _vertexCount = 0;
  char *context;
  char *line = strtok_r((char *)decompressedData, "\n", &context);
  while(line != NULL)
  {
    _vertexCount++;
    line = strtok_r(NULL, "\n", &context);
  }
  HeapFree(GetProcessHeap(), 0, decompressedData);

  line = strtok_r((char *)decompressedDataCopy, "\n", &context);
  Vertex *vertices = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
    sizeof(Vertex) * _vertexCount);
  int index = 0;
  while(index < _vertexCount)
  {
    int x, y, z, nx, ny, nz, r, g, b, t, v;
    sscanf(line, "%x %x %x %x %x %x %x %x %x %x %x",
      &x, &y, &z, &nx, &ny, &nz, &r, &g, &b, &t, &v);
    vertices[index] = (Vertex) {
      .x = x / SCALE_FACTOR,
      .y = y / SCALE_FACTOR,
      .z = z / SCALE_FACTOR,
      .nx = nx / SCALE_FACTOR,
      .ny = ny / SCALE_FACTOR,
      .nz = nz / SCALE_FACTOR,
      .color = D3DCOLOR_ARGB(255, (int)((r / SCALE_FACTOR) * 255),
        (int)((g / SCALE_FACTOR) * 255), (int)((b / SCALE_FACTOR) * 255)),
      .t = t / SCALE_FACTOR,
      .v = v / SCALE_FACTOR
    };
    line = strtok_r(NULL, "\n", &context);
    index++;
  }
  HeapFree(GetProcessHeap(), 0, decompressedDataCopy);

  d3ddev->lpVtbl->CreateVertexBuffer(d3ddev, _vertexCount * sizeof(Vertex), 0,
    0, D3DPOOL_MANAGED, vertexBuffer, 0);
  VOID *pVoid;
  (*vertexBuffer)->lpVtbl->Lock(*vertexBuffer, 0, 0, (void**)&pVoid, 0);
  CopyMemory(pVoid, vertices, _vertexCount * sizeof(Vertex));
  (*vertexBuffer)->lpVtbl->Unlock(*vertexBuffer);
  HeapFree(GetProcessHeap(), 0, vertices);

  *vertexCount = _vertexCount;
}
