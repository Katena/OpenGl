#pragma once


#include "../__OpenGL/IGraphicsObject.h"


class CScene : 
  public OpenGL::IGraphicsObject
{
private:
  void DrawCoordinateLines();

public:
  GLfloat m_xPos,
          m_yPos,
          m_yZoom;

public:

  CScene(void);
  ~CScene(void);

  virtual void TimerTick();
  virtual void Draw();

  void RotationChange(float fdX, float fdY);
  void ZoomChange(float fdZoom);
};
