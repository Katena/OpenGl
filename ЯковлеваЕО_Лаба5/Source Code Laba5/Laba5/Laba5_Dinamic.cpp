#include "StdAfx.h"
#include "Laba5_Dinamic.h"


CLaba5_Dinamic::CLaba5_Dinamic(void)
{
  for(int i = 0; i < PARTICLES_COUNT; i++)
    m_Particles[i].Init(&m_AttractionCylinder);
}


CLaba5_Dinamic::~CLaba5_Dinamic(void)
{
}


void CLaba5_Dinamic::TimerTick()
{
  for(int i = 0; i < PARTICLES_COUNT; i++)
    m_Particles[i].TimerTick();
}


void CLaba5_Dinamic::Draw()
{
  m_AttractionCylinder.Draw();

  glDisable(GL_DEPTH_TEST);
  {
    glColor4d(1.0, 0.8, 0.3, 1.0);
    glutSolidSphere(SPHERE_RADIUS, 50, 50);

    for(int i = 0; i < PARTICLES_COUNT; i++)
      m_Particles[i].Draw();
  }
  glEnable(GL_DEPTH_TEST);
}
