#pragma once


#include "../__OpenGL/IGraphicsObject.h"



class CLab4 :
  public OpenGL::IGraphicsObject
{
private:
  float Room_size;

private:
  void initializeGL();
  void genTexture();

  void drawObjects(int k);
  void drawRoom();
  void SetShadowMatrix(float fDestMat[16],float fLightPos[4],float fPlane[4]);
  void flipLight();

  void drawFloor(float Size, int k, int textureId);
  void drawWall_left(float Size, int k, int textureId);
  void drawWall_straight(float Size, int textureId);

  void drawCube(float box_size, int textureId);
  void drawSphere(GLdouble sphere_size, GLint slices, GLint stacks, int textureId);


public:
  CLab4(void);
  ~CLab4(void);

  //  IGraphicsObject
  virtual void TimerTick();
  virtual void Draw();

  void keyPressEvent(UINT vKeyCode, bool *Keys);
};

