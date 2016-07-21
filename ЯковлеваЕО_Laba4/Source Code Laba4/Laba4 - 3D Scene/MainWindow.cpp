#include "stdafx.h"
#include "MainWindow.h"
#include "Lab4.h"


#define CONST_GRAPH_SCENE_SIZE      100.0 
#define ZOOM_SPEED                  0.05


CMainWindow::CMainWindow(void)
{  
  m_hRC = NULL;
  m_bCaptureChanged = FALSE;
  m_wXMouse = m_wYMouse = 0;
}


CMainWindow::~CMainWindow(void)
{
}


LRESULT CMainWindow::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, 
                                  BOOL& /*bHandled*/)
{
  //  настройка OpenGL

  HDC hDC = GetDC();

  if(!SelectPixelFormat(hDC))
  {
    MessageBox(_T("Cannot select an appropriate pixel format."), _T("Error"), MB_ICONWARNING);
    return -1;
  }

  if(!(m_hRC = wglCreateContext(hDC))) 
  {
    MessageBox(_T("Cannot create rendering context."), _T("Error"), MB_ICONWARNING);
    return -1;
  }

  wglMakeCurrent(hDC, m_hRC); 
  InitGraphics(); 

  ReleaseDC(hDC);

  return 0;
}


LRESULT CMainWindow::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
  wglMakeCurrent(NULL, NULL); 
  wglDeleteContext(m_hRC); 
  
  PostQuitMessage(0);
  return 0;
}


LRESULT CMainWindow::OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
  OnSize(LOWORD(lParam), HIWORD(lParam)); 
  return 0;
}


LRESULT CMainWindow::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
 
  OnPaint(); 
  HDC hDC = GetDC();
  SwapBuffers(hDC); 

  ValidateRect(NULL); 
  return 0;
}


LRESULT CMainWindow::OnCaptureChanged(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
  m_bCaptureChanged = TRUE;
  return 0;
}


LRESULT CMainWindow::OnMouseMove(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
  WORD wX = LOWORD(lParam), wY = HIWORD(lParam);

  if(m_bCaptureChanged && wParam) 
    m_wXMouse = m_wYMouse = 0;
  else
  {
    m_wXMouse -= wX;
    m_wYMouse -= wY;
  }

  if((wParam & MK_RBUTTON) == MK_RBUTTON) 
  {
    
    m_pScene->ZoomChange((float)m_wYMouse * ZOOM_SPEED);
  }
  else if((wParam & MK_LBUTTON) == MK_LBUTTON)
  {
    
    m_pScene->RotationChange((float)m_wYMouse, (float)m_wXMouse);
  }

  m_wXMouse = wX;
  m_wYMouse = wY;

  m_bCaptureChanged = FALSE;

  InvalidateRect(NULL, FALSE);

  return 0;
}


LRESULT CMainWindow::OnKeyDown(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
  m_Keys[wParam] = true;								

  m_pLab4->keyPressEvent(wParam, m_Keys);
  InvalidateRect(NULL, FALSE);
  return 0;
}


LRESULT CMainWindow::OnKeyUp(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
  m_Keys[wParam] = false;								

  m_pLab4->keyPressEvent(wParam, m_Keys);
  InvalidateRect(NULL, FALSE);
  return 0;
}



LRESULT CMainWindow::OnCancelBtnClick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
  DestroyWindow();
  return 0;
}





//---------------------------------------------------------------------------

BOOL CMainWindow::SelectPixelFormat(HDC hDC)
{
  DWORD iPixelFormat;
  PIXELFORMATDESCRIPTOR pfd = {
    sizeof(PIXELFORMATDESCRIPTOR),		  
    1,									  
    PFD_DRAW_TO_WINDOW |					
    PFD_SUPPORT_OPENGL |					
    PFD_DOUBLEBUFFER,					   
    PFD_TYPE_RGBA,						  
    24,									 
    0,0,0,0,0,0,						
    0,0,								
    0,0,0,0,0,							
    32,									
    0,									 
    0,									 
    PFD_MAIN_PLANE,						
    0,									
    0,0,0};								 

    if((iPixelFormat = ChoosePixelFormat(hDC, &pfd)) == 0) 
      return false;

    return SetPixelFormat(hDC, iPixelFormat, &pfd);
}


//---------------------------------------------------------------------------

void CMainWindow::InitGraphics()
{
  glEnable(GL_COLOR_MATERIAL); 
  //glEnable(GL_DEPTH_TEST); // запрещает видеть за полупрозрачными объектами
  glShadeModel(GL_SMOOTH);						// Включается плавное затенение

  // цвет фона
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f );

  m_pScene = new CScene();
  m_GraphicsObjects.Add(m_pScene);
  m_pLab4 = new CLab4();
  m_GraphicsObjects.Add(m_pLab4);

  //m_GraphicsObjects.Add(new CSun());  
}


//---------------------------------------------------------------------------

void CMainWindow::OnSize(int w, int h)
{
  GLfloat fAspect;

  glViewport(0, 0, w, h); 


  fAspect = 1;

  glMatrixMode(GL_PROJECTION); 
  glLoadIdentity();
  gluPerspective(90.0f, fAspect, 1.0, CONST_GRAPH_SCENE_SIZE);

  glMatrixMode(GL_MODELVIEW);
}


//---------------------------------------------------------------------------

void CMainWindow::OnPaint()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
  glLoadIdentity();

  m_GraphicsObjects.Draw();
 
  
  glFlush();
}
//---------------------------------------------------------------------------


