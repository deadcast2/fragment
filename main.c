#include <windows.h>
#include "log.h"
#include "graphics.h"
#include "scene.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  switch(msg)
  {
    case WM_CREATE:
      InitGraphics(hWnd);
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
  char className[2] = {'f'};
  WNDCLASS wndClass = {
    .style = CS_OWNDC,
    .lpfnWndProc = WndProc,
    .lpszClassName = className,
    .hCursor = LoadCursor(0, IDC_ARROW),
    .hIcon = LoadIcon(0, IDI_INFORMATION)
  };
  RegisterClass(&wndClass);
  HWND hWnd = CreateWindow(className, "fragment",
    WS_EX_TOPMOST | WS_POPUP,
    0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 0, 0);
  return hWnd;
}

int WINAPI WinMainCRTStartup()
{
  MSG msg;
  InitWin();
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
    }
  }
  CleanGraphics();
  ExitProcess(0);
  return 0;
}
