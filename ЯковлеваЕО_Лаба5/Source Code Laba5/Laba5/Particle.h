#pragma once

#include "../__OpenGL/GlQuadric.h"
#include <deque>


#define SPHERE_RADIUS     0.2
#define CYLINDER_RADIUS   SPHERE_RADIUS


//////////////////////////////////////////////////////////////////////////


class CAttractorCylynder 
{
public:
  OpenGL::CGlQuadric m_glQuadric;
  double m_dX,
         m_dY,
         m_dZ;
  double m_dRadius, 
         m_dRadiusGravity;
  double m_dGravity;

private:
  void Generate();

public:
  CAttractorCylynder ();
  void Draw();
} ;


//////////////////////////////////////////////////////////////////////////


class CParticle
{
public:
  struct TXYZ
  {
    double xyz[3];
  };

  double m_dX,
         m_dY,
         m_dZ;
  double m_dXDelta,
         m_dYDelta,
         m_dZDelta;
  double m_dLifeTime;
  double m_dLifeTimeMax;
  double m_dLifeSpeed;
  double m_dRGB[3];
  unsigned int m_nTraceLength;
  std::deque<TXYZ> m_Trace;
  CAttractorCylynder *m_pAttractionCylinder;

  //  рождение новой частицы или перерождение старой
  void Generate();

public:
  CParticle(void);
  ~CParticle(void);

  void Init(CAttractorCylynder *pAttractionCylinder);


  virtual void TimerTick();
  virtual void Draw();
};

