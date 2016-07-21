#include "stdafx.h"
#include "Scene.h"


CScene::CScene(void)
{
  m_xPos = 0.0f;
  m_yPos = 0.0f;
  m_yZoom = 0.0;
}

CScene::~CScene(void)
{
}


void CScene::DrawCoordinateLines()
{
 
  glBegin(GL_LINES);
  {
    glColor3f(1.0, 0.5, 0.5); //x
    glVertex3f(-100.0, 0.0, 0.0);
    glVertex3f(100.0, 0.0, 0.0);

    glColor3f(0.5, 1.0, 0.5); //y
    glVertex3f(0.0, -100.0, 0.0);
    glVertex3f(0.0, 100.0, 0.0);

    glColor3f(0.5, 0.5, 1.0); //z
    glVertex3f(0.0, 0.0, -100.0);
    glVertex3f(0.0, 0.0, 100.0);
  }
  glEnd();

 
  glColor3f(1.0, 1.0, 1.0);
  glBegin(GL_QUADS);
  {
    glVertex3f(-5.0, 0.0, -5.0);
    glVertex3f(-5.0, 0.0, 5.0);
    glVertex3f(5.0, 0.0, 5.0);
    glVertex3f(5.0, 0.0, -5.0);
  }
  glEnd();
}


void CScene::TimerTick()
{
  //  empty
}   


void CScene::Draw()
{
  

  glTranslatef(0.0, 0.0, -5.0f); 
  glTranslatef(0.0, 0.0, m_yZoom); 

  //  выбираем изначальный обзор
  glRotatef(0, 1.0f, 0.0f, 0.0f);
  

  //  корректируем угол мышкой
  glRotatef(m_xPos, 1.0f, 0.0f, 0.0f); 
  glRotatef(m_yPos, 0.0f, 1.0f, 0.0f);

  DrawCoordinateLines();
}


void CScene::RotationChange(float fdX, float fdY)
{
  m_xPos -= fdX;
  m_xPos = (GLfloat)((const int)m_xPos % 360);

  m_yPos -= fdY;
  m_yPos = (GLfloat)((const int)m_yPos % 360);
}


void CScene::ZoomChange(float fdZoom)
{
  m_yZoom += fdZoom;
}
