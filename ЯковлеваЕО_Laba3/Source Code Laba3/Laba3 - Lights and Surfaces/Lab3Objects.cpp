#include "StdAfx.h"
#include "Lab3Objects.h"
#include <math.h>
#include "..\__OpenGL\Texture.h"


//////////////////////////////////////////////////////////////////////////

#define AXIS_X      0
#define AXIS_Y      1

#define TEXCOUNT    2
#define TEX_CYL     0
#define TEX_TEAPOT  1

GLUquadricObj *quadObj;
GLuint g_TextureID[TEXCOUNT];


//////////////////////////////////////////////////////////////////////////




CLab3Objects::CLab3Objects()
{
  scl = 100;
  rate = 1.0;
  ang_light_X = 0; ang_light_Y = 0; 
  axis_select = AXIS_X;

  initializeGL();
}

CLab3Objects::~CLab3Objects() { }


void CLab3Objects::initializeGL()
{
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  //glFrontFace(GL_CCW);  

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);

  glEnable(GL_TEXTURE_2D);
  genTexture();
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
}


void CLab3Objects::genTexture()
{
  OpenGL::LoadGLTexture(g_TextureID[TEX_TEAPOT], _T("teapot.bmp"));
  OpenGL::LoadGLTexture(g_TextureID[TEX_CYL], _T("cylinder.bmp"));
}


void CLab3Objects::glutSolidCylinder(GLint radius, GLint height, GLint slices, GLint stacks) 
{
  GLUquadricObj *quadObj = gluNewQuadric();
  gluQuadricDrawStyle(quadObj, GLU_SMOOTH);
  gluQuadricTexture(quadObj, GLU_TRUE);
  
  glPushMatrix();
  {
    gluCylinder(quadObj, radius, radius, height, slices, stacks);
    glRotatef(180, 1, 0, 0);
    gluDisk(quadObj, 0, radius, slices, stacks);
    glTranslatef(0, 0, -height);
    glRotatef(180, 1, 0, 0);
    gluDisk(quadObj, 0, radius, slices, stacks);
  }
  glPopMatrix();

  gluDeleteQuadric(quadObj);
}


void CLab3Objects::Draw()
{
    GLfloat light_0_posuition[] = {  40,   0,  15, 1.0 };
    GLfloat light_1_posuition[] = { -40,   0,  15, 1.0 };

    GLfloat light_0_color[] = { 0, 0, rate, 1 };
    GLfloat light_1_color[] = { 0, rate, 0, 1 };

//    GLfloat no_shininess[] = { 0.0 };
    GLfloat low_shininess[] = { 4.0 };
    GLfloat hi_shininess[] = { 100.0 };

    GLfloat cone_f = 1.0;
    GLfloat _specular[] = { cone_f, cone_f, cone_f, 1.0 };
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };

//    glRotatef(-180, 1, 0, 0);

    glDisable(GL_TEXTURE_2D);
    //  расставл€ем источники света и их цвета/€ркость
    glPushMatrix ();
        glRotatef(ang_light_X, 1, 0, 0);
        glRotatef(ang_light_Y, 0, 1, 0);

        glLightfv(GL_LIGHT0, GL_POSITION, light_0_posuition);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light_0_color);
        glLightfv(GL_LIGHT1, GL_POSITION, light_1_posuition);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, light_1_color);

        //  рисуем сами источники света в виде проволочных сфер
        glDisable(GL_LIGHTING);
        glPushMatrix();
            glTranslatef(75, 0, 30);
            glColor3d(0.0, 0.0, 1.0);
            glutWireSphere(2, 10, 10);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(-75, 0, 30);
            glColor3d(0.0, 1.0, 0.0);
            glutWireSphere(2, 10, 10);
        glPopMatrix ();
        glEnable(GL_LIGHTING);

    glPopMatrix();    
    glEnable(GL_TEXTURE_2D);


    glPushMatrix();
    {
      glDisable(GL_TEXTURE_2D);
      glTranslatef(0, 40, 0);
      glColor4f(0.5, 1.0, 0.5, 1.0);
      auxSolidTorus(5, 15);
      glEnable(GL_TEXTURE_2D);
    }
    glPopMatrix();

    glTranslatef(-30, 0, 0);


    //glDepthMask (GL_FALSE);
    //  включаем отражаемые поверхности
    glMaterialfv(GL_FRONT, GL_SPECULAR, _specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, hi_shininess);    
      glColor4f(1.0, 1.0, 1.0, 1.0);
      glBindTexture(GL_TEXTURE_2D, g_TextureID[TEX_CYL]);
        glutSolidCylinder(20, 50, 50, 50);
    glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
    //glDepthMask (GL_TRUE);
    //glDisable (GL_BLEND);

    glEnable (GL_BLEND);  
    glBlendFunc (GL_SRC_ALPHA, GL_ONE);
    glTranslatef(60, 0, 0);
    glBindTexture(GL_TEXTURE_2D, g_TextureID[TEX_TEAPOT]);
      glColor4f(1.0, 1.0, 1.0, 0.3);
      glutSolidTeapot(20);
    glDisable (GL_BLEND);

    glFlush();
}


void CLab3Objects::keyPressEvent(UINT vKeyCode, bool *Keys)
{
  //  кнопочки 1, 2 - выбор оси вращени€
  //  стрелка вверх - вращение
  switch(vKeyCode) 
  {
  case '1': axis_select = AXIS_X; break;
  case '2': axis_select = AXIS_Y; break;
  case VK_UP:
    {
      switch(axis_select) 
      {
      case 0:
        ang_light_X = (ang_light_X + 5) % 360;
        break;
      case 1: 
        ang_light_Y = (ang_light_Y + 5) % 360;
        break;
      }
    }
    break;
  }

  //  shift + влево/вправо - управление €ркостью лампочек
  if( Keys[VK_SHIFT] ) 
  {
    switch(vKeyCode) 
    {
    case VK_LEFT: 
      if(rate < 0.1) rate = 0.0;   else  rate = rate - 0.1;
      break;
    case VK_RIGHT: 
      if(rate > 0.9) rate = 1.0; else rate = rate + 0.1;
      break;
    }
  }

  //  ctrl + 1,2 - вкл./выкл. лампочек
  if( Keys[VK_CONTROL] ) 
  {
    if(Keys['1'])
    {
      if(!glIsEnabled(GL_LIGHT0)) glEnable(GL_LIGHT0); else glDisable(GL_LIGHT0);
    }
    else if(Keys['2'])
    {
      if(!glIsEnabled(GL_LIGHT1)) glEnable(GL_LIGHT1); else glDisable(GL_LIGHT1);
    }
  }

}



void CLab3Objects::TimerTick()
{
  //  empty
}