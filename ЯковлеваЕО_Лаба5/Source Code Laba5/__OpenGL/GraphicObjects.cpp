#include "stdafx.h"
#include "GraphicObjects.h"



OpenGL::CGraphicObjects::CGraphicObjects(void)
  : m_mutex(FALSE)
{
}


OpenGL::CGraphicObjects::~CGraphicObjects(void)
{
  Clear();
}


void OpenGL::CGraphicObjects::Clear()
{
  Lock();
  {
    while(!m_lObjects.empty())
    {
      delete m_lObjects.front();
      m_lObjects.pop_front();
    }
  }
  Unlock();
}


void OpenGL::CGraphicObjects::Add(IGraphicsObject *pObject)
{
  Lock();
  {
    m_lObjects.push_back(pObject);
  }
  Unlock();
}


void OpenGL::CGraphicObjects::TimerTick()
{
  Lock();
  {
    TObjectsList::iterator i, iend = m_lObjects.end();
    for(i = m_lObjects.begin(); i != iend; i++)
      (*i)->TimerTick();
  }
  Unlock();
}


void OpenGL::CGraphicObjects::Draw()
{
  Lock();
  {
    TObjectsList::iterator i, iend = m_lObjects.end();
    for(i = m_lObjects.begin(); i != iend; i++)
      (*i)->Draw();
  }
  Unlock();
}


void OpenGL::CGraphicObjects::Lock()
{
  WaitForSingleObject(m_mutex, INFINITE);
}


void OpenGL::CGraphicObjects::Unlock()
{ 
  m_mutex.Release();
}
