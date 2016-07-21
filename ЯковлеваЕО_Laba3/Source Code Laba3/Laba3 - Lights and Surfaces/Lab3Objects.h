#pragma once


#include "../__OpenGL/IGraphicsObject.h"


class CLab3Objects :
  public OpenGL::IGraphicsObject
{
private:
  int scl;
  double rate;
  int ang_light_X, ang_light_Y;
  int axis_select;


private:
  void initializeGL();
  void genTexture();

  void glutSolidCylinder(GLint radius, GLint height, GLint slices, GLint stacks);

public:
  CLab3Objects(void);
  ~CLab3Objects(void);

  //  IGraphicsObject
  virtual void TimerTick();
  virtual void Draw();

  void keyPressEvent(UINT vKeyCode, bool *Keys);
};

