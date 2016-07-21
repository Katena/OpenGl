#pragma once

#include <gl/glu.h>


namespace OpenGL
{

class CGlQuadric
{
private:
  GLUquadricObj *m_pQuadric;

public:
  CGlQuadric(void)
  {
    m_pQuadric = gluNewQuadric();
  }

  ~CGlQuadric(void)
  {
    gluDeleteQuadric(m_pQuadric);
  }

  operator GLUquadricObj*() { return m_pQuadric;  }
};



} //  namespace OpenGL

