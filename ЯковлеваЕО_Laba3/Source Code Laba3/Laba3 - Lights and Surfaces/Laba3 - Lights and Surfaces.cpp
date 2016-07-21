
#include "stdafx.h"
#include "Laba3 - Lights and Surfaces.h"
#include "MAINWINDOW.h"



int APIENTRY _tWinMain(HINSTANCE hInstance,
  HINSTANCE hPrevInstance,
  LPTSTR    lpCmdLine,
  int       nCmdShow)


{
  //  ������� ������
  CMainWindow wndMain;
  if (wndMain.Create(NULL) == NULL)
  {
    DWORD dwErr = GetLastError();
    return dwErr;
  }

  //  ���������� ���
  wndMain.ShowWindow(nCmdShow);

  // ��������� ���� ���������
  WTL::CMessageLoop loop;
  int res = loop.Run();

  return res;
}