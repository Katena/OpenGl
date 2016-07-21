#pragma once

#include "resource.h"

#include "../__OpenGL/GraphicObjects.h"
#include "Scene.h"



class CMainWindow : 

  public CWindowImpl<CMainWindow, CWindow, CFrameWinTraits>
{
public:

  CMainWindow(void);
  ~CMainWindow(void);

  //enum {IDD = IDD_MAINWINDOW};

  DECLARE_WND_CLASS(_T("CMainWindow class :)"))

private:

  HGLRC m_hRC; 
  CScene *m_pScene;
  OpenGL::CGraphicObjects m_GraphicsObjects;

  short m_wXMouse, m_wYMouse;
  BOOL m_bCaptureChanged;     

private:

  BOOL SelectPixelFormat(HDC hDC);
  void InitGraphics();
  void OnSize(int w, int h);
  void OnPaint();
  void SetMenuLineType(WORD wID);

  void DrawOtrezok();
  void DrawDuga();
  void DrawCurvBezie();
  void DrawRotationLine();

public:

  //  Здесь регистрируем обработчики событий (привязываем функцию-обработчик к событию):

  BEGIN_MSG_MAP(CMainWindow)
    //MESSAGE_HANDLER(WM_INITDIALOG, OnCreate)
    MESSAGE_HANDLER(WM_CREATE, OnCreate)
    MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
    MESSAGE_HANDLER(WM_SIZE, OnSize)
    MESSAGE_HANDLER(WM_PAINT, OnPaint)
    MESSAGE_HANDLER(WM_CAPTURECHANGED, OnCaptureChanged)
    MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)    
    
    COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnCancelBtnClick)
  END_MSG_MAP()


  //  Сами обработчики событий
 
  LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, 
    BOOL& /*bHandled*/);
  LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
  LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
  LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
  LRESULT OnCaptureChanged(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
  LRESULT OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
  
  LRESULT OnCancelBtnClick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
};
