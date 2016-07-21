
#include "stdafx.h"
#include "Laba3 - Lights and Surfaces.h"
#include "MAINWINDOW.h"



int APIENTRY _tWinMain(HINSTANCE hInstance,
  HINSTANCE hPrevInstance,
  LPTSTR    lpCmdLine,
  int       nCmdShow)


{
  //  Создаем окошко
  CMainWindow wndMain;
  if (wndMain.Create(NULL) == NULL)
  {
    DWORD dwErr = GetLastError();
    return dwErr;
  }

  //  Показываем его
  wndMain.ShowWindow(nCmdShow);

  // Запускаем цикл сообщений
  WTL::CMessageLoop loop;
  int res = loop.Run();

  return res;
}