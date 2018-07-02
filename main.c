#include <windows.h>
#include "log.h"
#include "graphics.h"
#include "scene.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  switch(msg)
  {
    case WM_CREATE:
      InitDirectX(hWnd);
      InitScene();
      break;
    case WM_KEYDOWN:
      if(wParam == VK_ESCAPE) PostMessage(hWnd, WM_QUIT, wParam, lParam);
      break;
    default:
      return DefWindowProc(hWnd, msg, wParam, lParam);
  }
  return 0;
}

HWND InitWin()
{
  char className[2] = {'t'};
  WNDCLASS wndClass = {
    .style = CS_OWNDC,
    .lpfnWndProc = WndProc,
    .lpszClassName = className,
    .hCursor = LoadCursor(0, IDC_ARROW),
    .hIcon = LoadIcon(0, IDI_INFORMATION)
  };
  RegisterClass(&wndClass);
  HWND hWnd = CreateWindow(className, "", WS_OVERLAPPED | WS_VISIBLE,
    0, 0, 800, 600, 0, 0, 0, 0);
  DEVMODE screen = {
    .dmSize = sizeof(screen),
    .dmPelsWidth = 800,
    .dmPelsHeight = 600,
    .dmBitsPerPel = 32,
    .dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT
  };
  ChangeDisplaySettings(&screen, CDS_FULLSCREEN);
  return hWnd;
}

int WINAPI WinMainCRTStartup()
{
  MSG msg;
  HDC hdc = GetDC(InitWin());
  while(msg.message != WM_QUIT)
  {
    if(PeekMessage(&msg, 0, 0, 0, TRUE))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    else
    {
      RenderScene();
      SwapBuffers(hdc);
    }
  }
  ExitProcess(0);
  return 0;
}
