#pragma once


#include "IGraphicsObject.h"
#include <deque>
#include <atlsync.h>



namespace OpenGL
{


class CGraphicObjects :
  public IGraphicsObject
{
private:
  typedef std::deque<IGraphicsObject*> TObjectsList;

  TObjectsList m_lObjects;
  ATL::CMutex m_mutex;

  void Lock();
  void Unlock();

public:
  CGraphicObjects(void);
  ~CGraphicObjects(void);

  void Clear();
  void Add(IGraphicsObject *pObject);

  //  IGraphicsObject
  virtual void TimerTick();
  virtual void Draw();
};


} //  namespace OpenGL
