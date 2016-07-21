#pragma once



#include "../__OpenGL/IGraphicsObject.h"
#include "Particle.h"



#define PARTICLES_COUNT   250


class CLaba5_Dinamic :
  public OpenGL::IGraphicsObject
{
public:
  CAttractorCylynder m_AttractionCylinder;
  CParticle m_Particles[PARTICLES_COUNT];
  

public:
  CLaba5_Dinamic(void);
  ~CLaba5_Dinamic(void);

 
  virtual void TimerTick();
  virtual void Draw();
};

