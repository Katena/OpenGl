#include "StdAfx.h"
#include "Particle.h"
#include <math.h>

//////////////////////////////////////////////////////////////////////////

double Random(double dFrom, double dTo)
{
  return ( (double)rand() / RAND_MAX ) * (dTo - dFrom) + dFrom;
}

//////////////////////////////////////////////////////////////////////////


CAttractorCylynder::CAttractorCylynder()
{
  Generate();
}


void CAttractorCylynder::Generate()
{
  m_dX = 2;
  m_dY = -2;
  m_dZ = 0;

  m_dRadius = CYLINDER_RADIUS;
  m_dRadiusGravity = 4.0;

  m_dGravity = 0.001;
}


void CAttractorCylynder::Draw()
{
  glPushMatrix();
  {
    glTranslated(m_dX, m_dY, m_dZ);
    glColor4d(0.5, 0.5, 0.5, 1.0);
    gluCylinder(m_glQuadric, m_dRadius, m_dRadius, CYLINDER_RADIUS*2, 50, 50 );
  }
  glPopMatrix();
}






//////////////////////////////////////////////////////////////////////////

#define PARTICLE_SIZE     0.01

#define LIFETIME_MIN      10.
#define LIFETIME_MAX      50.
#define LIFETIME_PERIOD   (LIFETIME_MAX - LIFETIME_MIN)

#define SPEED_MIN             0.001
#define SPEED_MAX             0.1
#define SPEED_SLOWDOWN_COEFF  0.02

#define TRACE_MIN         2
#define TRACE_MAX         8

//////////////////////////////////////////////////////////////////////////

CParticle::CParticle(void)
{
  Generate();
}


CParticle::~CParticle(void)
{
}


void CParticle::Init(CAttractorCylynder *pAttractionCylinder)
{
  m_pAttractionCylinder = pAttractionCylinder;
}


void CParticle::Generate()
{
  //  случайное местоположение
  double dAngleA = Random(-180., +180.);
  double dAngleB = Random(0., +360.);
  m_dX = SPHERE_RADIUS * sin(dAngleA) * cos(dAngleB);
  m_dY = SPHERE_RADIUS * sin(dAngleA) * sin(dAngleB);
  m_dZ = SPHERE_RADIUS * cos(dAngleA);

  //  случайное направление и скорость движения
  m_dXDelta = Random(SPEED_MIN, SPEED_MAX) * sin(dAngleA) * cos(dAngleB);
  m_dYDelta = Random(SPEED_MIN, SPEED_MAX) * sin(dAngleA) * sin(dAngleB);
  m_dZDelta = Random(SPEED_MIN, SPEED_MAX) * cos(dAngleA);

  //  случайное время жизни и скорость жизни
  m_dLifeTime = m_dLifeTimeMax = Random(LIFETIME_MIN, LIFETIME_MAX);
  m_dLifeSpeed = Random(0.1, 1.0);

  //  случайный цвет (генерим до тех пор, пока не получится достаточно яркий цвет)
  double dShinessMax;
  do 
  {
    dShinessMax = 0.0;
    for (int i = 0; i < 3; i++)
    {
      m_dRGB[i] = Random(0.0, 1.0);
      if (dShinessMax < m_dRGB[i])
        dShinessMax = m_dRGB[i];
    }
  } while (dShinessMax < 0.5);    //  яркость хотя бы 1-й компоненты должна быть больше 0.5

  //  след
  m_nTraceLength = Random(TRACE_MIN, TRACE_MAX + 1);
  m_Trace.clear();
}


void CParticle::TimerTick()
{
  //  жизнь частицы подходит к концу
  m_dLifeTime -= m_dLifeSpeed;
  if ( m_dLifeTime <= 0.0 )
  {
    //  частица умерла, перерождаем и выходим
    Generate();
    return;
  }
     
  //  двигаем частицу

  TXYZ xyz = {m_dX, m_dY, m_dZ};
  m_Trace.push_back(xyz); //  только сначала запоминаем след
  if (m_Trace.size() > m_nTraceLength)
    m_Trace.pop_front();

  m_dX += m_dXDelta;
  m_dY += m_dYDelta;
  m_dZ += m_dZDelta;

  //  вычисляем влияние гравитации аттрактора на скорость

  double dAttractorDistance = 
    sqrt( (m_dX - m_pAttractionCylinder->m_dX) * (m_dX - m_pAttractionCylinder->m_dX) + 
          (m_dY - m_pAttractionCylinder->m_dY) * (m_dY - m_pAttractionCylinder->m_dY) +
          (m_dZ - m_pAttractionCylinder->m_dZ) * (m_dZ - m_pAttractionCylinder->m_dZ) );

  if(dAttractorDistance < m_pAttractionCylinder->m_dRadiusGravity) 
  {
    double dXr = m_pAttractionCylinder->m_dX - 
      m_pAttractionCylinder->m_dRadius * (m_pAttractionCylinder->m_dX - m_dX) / dAttractorDistance - 
      m_dX;
    double dYr = m_pAttractionCylinder->m_dY - 
      m_pAttractionCylinder->m_dRadius * (m_pAttractionCylinder->m_dY - m_dY) / dAttractorDistance -  
      m_dY;
    double dZr = m_pAttractionCylinder->m_dZ - 
      m_pAttractionCylinder->m_dRadius * (m_pAttractionCylinder->m_dZ - m_dZ) / dAttractorDistance -  
      m_dZ;

    m_dXDelta += m_pAttractionCylinder->m_dGravity * 
      (1.0 - dXr / m_pAttractionCylinder->m_dRadiusGravity) * dXr;
    m_dYDelta += m_pAttractionCylinder->m_dGravity * 
      (1.0 - dYr / m_pAttractionCylinder->m_dRadiusGravity) * dYr;
    m_dZDelta += m_pAttractionCylinder->m_dGravity * 
      (1.0 - dZr / m_pAttractionCylinder->m_dRadiusGravity) * dZr;
  }

  //  применяем замедление

  m_dXDelta -= (m_dXDelta * SPEED_SLOWDOWN_COEFF);
  m_dYDelta -= (m_dYDelta * SPEED_SLOWDOWN_COEFF);
  m_dZDelta -= (m_dZDelta * SPEED_SLOWDOWN_COEFF);
}


void CParticle::Draw()
{
  //частица
  glColor4d(m_dRGB[0], m_dRGB[1], m_dRGB[2], m_dLifeTime / m_dLifeTimeMax );
  glPushMatrix();
  {
    glTranslated(m_dX, m_dY, m_dZ);
    glutSolidSphere(PARTICLE_SIZE, 4, 4);
  }
  glPopMatrix();

  //  след частицы
  std::deque<TXYZ>::iterator i, iend = m_Trace.end();
  for (i = m_Trace.begin(); i != iend; i++)
  {
    glPushMatrix();
    {
      glTranslated(i->xyz[0], i->xyz[1], i->xyz[2]);
      glutSolidSphere(PARTICLE_SIZE, 3, 3);
    }
    glPopMatrix();
  }
}
