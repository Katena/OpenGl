#pragma once




namespace OpenGL
{


class IGraphicsObject
{
public:
  virtual void TimerTick() = 0;
  virtual void Draw() = 0;
};



} //namespace OpenGL