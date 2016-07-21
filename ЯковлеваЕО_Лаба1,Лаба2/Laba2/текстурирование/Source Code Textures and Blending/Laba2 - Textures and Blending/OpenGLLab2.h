#pragma once


#include "../__OpenGL/IGraphicsObject.h"

class COpenGLLab2 :
  public OpenGL::IGraphicsObject
{
public:
  COpenGLLab2(void);
  ~COpenGLLab2(void);


  virtual void TimerTick();
  virtual void Draw();

};

