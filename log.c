#include "log.h"

void Log(const char *msg)
{
  HANDLE stdout = GetStdHandle(STD_OUTPUT_HANDLE);
  WriteFile(stdout, msg, lstrlen(msg), (DWORD[]){0}, NULL);
}
