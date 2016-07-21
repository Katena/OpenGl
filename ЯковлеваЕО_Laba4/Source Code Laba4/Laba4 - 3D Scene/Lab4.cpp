#include "StdAfx.h"
#include "Lab4.h"
#include "..\__OpenGL\Texture.h"


//////////////////////////////////////////////////////////////////////////

#define TEXCOUNT    6
#define TEX_FLOOR   0
#define TEX_WALL    1
#define TEX_SPHERE  2
#define TEX_CUBE    3
#define TEX_CAP     4
#define TEX_WALL1   5 

GLuint g_TextureID[TEXCOUNT];

const float box_s = 0.4;
const float sphere_R = 0.1;
const float cube_Size = 0.3;
float Plane[4] = {1, 0, 0, 0};

float LightAmbient[] = {0.5f, 0.5f, 0.5f, 1.0f};
float LightDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
float LightSpecular[] = {1.0, 1.0, 1.0, 1.0};
float LightPosition[] = {-2*box_s, 2*box_s, box_s/2, 1.0f};



GLfloat hi_shininess[] = { 100.0 };
GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };

float fShadowMatrix[16];

//////////////////////////////////////////////////////////////////////////


CLab4::CLab4(void)
{
  Room_size = 3.0;


  initializeGL();
}


CLab4::~CLab4(void)
{}


void CLab4::TimerTick()
{
  //  empty
}


void CLab4::initializeGL()
{
	glEnable(GL_COLOR_MATERIAL); 

	// цвет фона
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f );

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST); 
	
	genTexture();

  }

void CLab4::genTexture()
{
  OpenGL::LoadGLTexture(g_TextureID[TEX_FLOOR], _T("floor.bmp"));
  OpenGL::LoadGLTexture(g_TextureID[TEX_WALL], _T("wall.bmp"));
  OpenGL::LoadGLTexture(g_TextureID[TEX_WALL1], _T("wall1.bmp"));
  OpenGL::LoadGLTexture(g_TextureID[TEX_SPHERE], _T("sphere.bmp"));
  OpenGL::LoadGLTexture(g_TextureID[TEX_CUBE], _T("cube.bmp"));
  OpenGL::LoadGLTexture(g_TextureID[TEX_CAP], _T("cap.bmp"));  
}


void CLab4::Draw()
{
  glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);
  glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  drawRoom();

  glFlush ();
}


void CLab4::drawFloor(float Size, int k, int textureId)
{
  glPushMatrix();
  glBindTexture(GL_TEXTURE_2D, textureId);
  glBegin(GL_QUADS);
  glNormal3f(0, k, 0);
  glTexCoord2f(1, 1); glVertex3f(      0, 0,       0);
  glTexCoord2f(0, 1); glVertex3f( k*Size, 0,       0);
  glTexCoord2f(0, 0); glVertex3f( k*Size, 0,  k*Size);
  glTexCoord2f(1, 0); glVertex3f(      0, 0,  k*Size);
  glEnd();
  glPopMatrix();
}


void CLab4::drawWall_left(float Size, int k, int textureId)
{
  glPushMatrix();
  glBindTexture(GL_TEXTURE_2D, textureId);
  glBegin(GL_QUADS);
  glNormal3f(k, 0, 0);
  glTexCoord2f(0, 0); glVertex3f(0,       0,       0);
  glTexCoord2f(1, 0); glVertex3f(0,       0,  k*Size);
  glTexCoord2f(1, 1); glVertex3f(0,  k*Size,  k*Size);
  glTexCoord2f(0, 1); glVertex3f(0,  k*Size,       0);
  glEnd();
  glPopMatrix();
}


void CLab4::drawWall_straight(float Size, int textureId)
{
  glPushMatrix();
  glBindTexture(GL_TEXTURE_2D, textureId);
  glBegin(GL_QUADS);
  glNormal3f(0, 0, 1);
  glTexCoord2f(1, 0); glVertex3f(    0,     0,     0);
  glTexCoord2f(1, 1); glVertex3f(    0,  Size,     0);
  glTexCoord2f(0, 1); glVertex3f( Size,  Size,     0);
  glTexCoord2f(0, 0); glVertex3f( Size,     0,     0);
  glEnd();
  glPopMatrix();
}


void CLab4::drawSphere(GLdouble sphere_size, GLint slices, GLint stacks, int textureId)
{
  glMaterialfv(GL_FRONT, GL_SPECULAR, LightSpecular);  
  glMaterialfv(GL_FRONT, GL_SHININESS, hi_shininess);    
  glPushMatrix();
  {
    glBindTexture(GL_TEXTURE_2D, textureId);
    /*glTranslatef(0.8, -0.8, 0);*/
    glTranslatef(0, 0, 0);

    GLUquadricObj *quadObj = gluNewQuadric();
    gluQuadricDrawStyle(quadObj, GLU_SMOOTH);
    gluQuadricTexture(quadObj, GLU_TRUE);
    gluSphere(quadObj, sphere_size, slices, stacks);
    gluDeleteQuadric(quadObj);
  }  
  glPopMatrix();
  glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
  glMaterialfv(GL_FRONT, GL_SHININESS, no_mat);

//   glDisable(GL_CULL_FACE);
}


void CLab4::drawCube(float box_size, int textureId)
{
  glPushMatrix();
  {
    glRotatef(20, 0, 1, 1);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glBegin(GL_QUADS);
    {
      glNormal3f(0.0, 1.0f, 0.0f); //Top face
      glTexCoord2f(0.0f, 1.0f); glVertex3f(-box_size/2, box_size/2, -box_size/2); 
      glTexCoord2f(0.0f, 0.0f); glVertex3f(-box_size/2, box_size/2,  box_size/2); 
      glTexCoord2f(1.0f, 0.0f); glVertex3f( box_size/2, box_size/2,  box_size/2); 
      glTexCoord2f(1.0f, 1.0f); glVertex3f( box_size/2, box_size/2, -box_size/2);

      glNormal3f(0.0, -1.0f, 0.0f); //Bottom face
      glTexCoord2f(1.0f, 1.0f); glVertex3f(-box_size/2, -box_size/2, -box_size/2); 
      glTexCoord2f(0.0f, 1.0f); glVertex3f( box_size/2, -box_size/2, -box_size/2); 
      glTexCoord2f(0.0f, 0.0f); glVertex3f( box_size/2, -box_size/2,  box_size/2); 
      glTexCoord2f(1.0f, 0.0f); glVertex3f(-box_size/2, -box_size/2,  box_size/2);

      glNormal3f(-1.0, 0.0f, 0.0f); //Left face
      glTexCoord2f(1.0f, 1.0f); glVertex3f(-box_size/2, box_size/2, box_size/2); 
      glTexCoord2f(0.0f, 1.0f); glVertex3f(-box_size/2, box_size/2,  -box_size/2); 
      glTexCoord2f(0.0f, 0.0f); glVertex3f(-box_size/2,  -box_size/2,  -box_size/2); 
      glTexCoord2f(1.0f, 0.0f); glVertex3f(-box_size/2,  -box_size/2, box_size/2);

      glNormal3f(1.0, 0.0f, 0.0f); //Right face
      glTexCoord2f(1.0f, 0.0f); glVertex3f( box_size/2, -box_size/2, -box_size/2); 
      glTexCoord2f(1.0f, 1.0f); glVertex3f( box_size/2,  box_size/2, -box_size/2); 
      glTexCoord2f(0.0f, 1.0f); glVertex3f( box_size/2,  box_size/2,  box_size/2); 
      glTexCoord2f(0.0f, 0.0f); glVertex3f( box_size/2, -box_size/2,  box_size/2);

      glNormal3f(0.0, 0.0f, 1.0f); //Front face
      glTexCoord2f(0.0f, 0.0f); glVertex3f(-box_size/2, -box_size/2,  box_size/2); 
      glTexCoord2f(1.0f, 0.0f); glVertex3f( box_size/2, -box_size/2,  box_size/2); 
      glTexCoord2f(1.0f, 1.0f); glVertex3f( box_size/2,  box_size/2,  box_size/2); 
      glTexCoord2f(0.0f, 1.0f); glVertex3f(-box_size/2,  box_size/2,  box_size/2);

      glNormal3f(0.0, 0.0f, -1.0f); //Back face
      glTexCoord2f(1.0f, 0.0f); glVertex3f(-box_size/2, -box_size/2, -box_size/2); 
      glTexCoord2f(1.0f, 1.0f); glVertex3f(-box_size/2,  box_size/2, -box_size/2); 
      glTexCoord2f(0.0f, 1.0f); glVertex3f( box_size/2,  box_size/2, -box_size/2); 
      glTexCoord2f(0.0f, 0.0f); glVertex3f( box_size/2, -box_size/2, -box_size/2);
    } 
    glEnd();
  } 
  glPopMatrix(); 
}


void CLab4::drawObjects(int k)
{
  glPushMatrix();
  {
    int plane_k = 0;
    switch(k) {
    case 0: { Plane[0] =   0; Plane[1] =   1; Plane[2] =   0; Plane[3] = 0; plane_k = Plane[0] + Plane[1] + Plane[2] + Plane[3];
      break; }
    case 1: { Plane[0] =   1; Plane[1] =   0; Plane[2] =   0; Plane[3] = 0; plane_k = Plane[0] + Plane[1] + Plane[2] + Plane[3];
      break; }
    case 2: { Plane[0] =   0; Plane[1] =   0; Plane[2] =   1; Plane[3] = 0; plane_k = Plane[0] + Plane[1] + Plane[2] + Plane[3];
      break; }
    case 3: { Plane[0] =   0; Plane[1] = - 1; Plane[2] =   0; Plane[3] = 0; plane_k = Plane[0] + Plane[1] + Plane[2] + Plane[3];
      break; }
    case 4: { Plane[0] = - 1; Plane[1] =   0; Plane[2] =   0; Plane[3] = 0; plane_k = Plane[0] + Plane[1] + Plane[2] + Plane[3];
      break; }
    default:  break;
    }

    glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    flipLight();
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, 1, 0xFFFFFFFF);
    glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);

    glColor4f(0.9, 0.9, 0.9, 1.0);

    switch(k) {
    case 0:  drawFloor(Room_size, plane_k, g_TextureID[TEX_FLOOR]);
      break;
    case 1:  drawWall_left(Room_size, plane_k, g_TextureID[TEX_WALL]);
      break;
    case 2:  drawWall_straight(Room_size, g_TextureID[TEX_WALL]);
      break;
    case 3:  drawFloor(Room_size, plane_k, g_TextureID[TEX_CAP]);
      break;
    case 4:  drawWall_left(Room_size, plane_k, g_TextureID[TEX_WALL]);
      break;
    default:  break;
    }

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);
    glStencilFunc(GL_EQUAL, 1, 0xFFFFFFFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

    switch(k) {
    case 0:  drawFloor(Room_size, plane_k, g_TextureID[TEX_FLOOR]);
      break;
    case 1:  drawWall_left(Room_size, plane_k, g_TextureID[TEX_WALL1]);
      break;
    case 2:  drawWall_straight(Room_size, g_TextureID[TEX_WALL]);
      break;
    case 3:  drawFloor(Room_size, plane_k, g_TextureID[TEX_CAP]);
      break;
    case 4:  drawWall_left(Room_size, plane_k, g_TextureID[TEX_WALL1]);
      break;
    default:  break;
    }

    if (k == 0) //  0 = пол
    {
      //  рисование теней от объектов на "полу"

      glPushMatrix();
      {
        glColor4f(0.0, 0.0, 0.0, 0.5f);
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);

        glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);

        glTranslatef(plane_k*0.0001, plane_k*0.0001, plane_k*0.0001);
        glMultMatrixf(fShadowMatrix);
        glTranslatef(-plane_k*0.0001, -plane_k*0.0001, -plane_k*0.0001);

        glPushMatrix();
        {
          glTranslatef(plane_k * Room_size/2, plane_k * Room_size/2, plane_k*Room_size/2);
          drawSphere(sphere_R, 30, 30, g_TextureID[TEX_SPHERE]);
          drawCube(cube_Size, g_TextureID[TEX_CUBE]);
        }
        glPopMatrix();

        glEnable(GL_TEXTURE_2D);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
        glEnable(GL_LIGHTING);
      }
      glPopMatrix();
    }
    
    glDisable(GL_STENCIL_TEST);
    
    if(k == 3)
    {
      //  рисование самих объектов

      //  источник света
      glDisable(GL_LIGHTING);
      glPushMatrix();
      {
        glTranslatef(LightPosition[0], LightPosition[1]-1, LightPosition[2]);
        //glColor4fv(LightDiffuse);
        glColor4f(0.0, 0.0, 0.0, 1.0);
        glutWireSphere(Room_size/50, 10, 10);
      }
      glPopMatrix();
      glEnable(GL_LIGHTING);
      
      //  куб и сфера

      glPushMatrix();
      {
        glTranslatef(plane_k*Room_size/2, plane_k*Room_size/2, plane_k*Room_size/2);
        glColor4f(0.5, 0.5, 0.5, 1.0);
        drawSphere(sphere_R, 30, 30, g_TextureID[TEX_SPHERE]);

        glEnable (GL_BLEND);      
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);

        glColor4f(0.5, 0.5, 0.5, 0.5);
        drawCube(cube_Size, g_TextureID[TEX_CUBE]);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable (GL_BLEND);
      }
      glPopMatrix();
    }

  }
  glPopMatrix();
}


void CLab4::drawRoom()
{
  glEnable(GL_BACK);

  glPushMatrix();
  {
    LightPosition[0] = LightPosition[0] + Room_size; 
    LightPosition[1] = LightPosition[1] + Room_size; 
    LightPosition[2] = LightPosition[2] + Room_size;
    glTranslatef(- Room_size/2, - Room_size/2, - Room_size/2);
    for(int k = 0; k < 3; k++) 
      drawObjects(k);  
 
  }
  glPopMatrix();
  
  glPushMatrix();
  {
    LightPosition[0] = LightPosition[0] - Room_size; 
    LightPosition[1] = LightPosition[1] - Room_size; 
    LightPosition[2] = LightPosition[2] - Room_size;
    glTranslatef(Room_size/2, Room_size/2, Room_size/2);
    for(int k = 3; k < 5; k++) 
      drawObjects(k);


  }
  glPopMatrix();
}


void CLab4::keyPressEvent(UINT vKeyCode, bool *Keys)
{
  //  изменение позиции лампы

  if( Keys[VK_CONTROL] ) 
  {
    switch(vKeyCode) 
    {
    case VK_LEFT:     
      if(LightPosition[0] < - Room_size*1.5 + 0.1) LightPosition[0] = - Room_size*1.5; else LightPosition[0] = LightPosition[0] - 0.1;
      break;
    case VK_RIGHT:    
      if(LightPosition[0] >   Room_size*1.5 - 0.1) LightPosition[0] =   Room_size*1.5; else LightPosition[0] = LightPosition[0] + 0.1;
      break;
    case VK_UP:       
      if(LightPosition[2] < - Room_size*1.5 + 0.1) LightPosition[2] = - Room_size*1.5; else LightPosition[2] = LightPosition[2] - 0.1;
      break;
    case VK_DOWN:     
      if(LightPosition[2] >   Room_size*1.5 - 0.1) LightPosition[2] =   Room_size*1.5; else LightPosition[2] = LightPosition[2] + 0.1;
      break;
    case VK_NEXT: 
      if(LightPosition[1] < - Room_size*1.5 + 0.1) LightPosition[1] = - Room_size*1.5; else LightPosition[1] = LightPosition[1] - 0.1;
      break;
    case VK_PRIOR:   
      if(LightPosition[1] >   Room_size*1.5 - 0.1) LightPosition[1] =   Room_size*1.5; else LightPosition[1] = LightPosition[1] + 0.1;
      break;
    default:  break;
    }
  }

  //  изменение цвета лампы

  if( Keys[VK_SHIFT] ) 
  {
    switch(vKeyCode) 
    {
    case VK_LEFT:     
      if(LightDiffuse[0] < - 0.9) LightDiffuse[0] = - 1.0; else LightDiffuse[0] = LightDiffuse[0] - 0.1;
      break;
    case VK_RIGHT:    
      if(LightDiffuse[0] >   0.9) LightDiffuse[0] =   1.0; else LightDiffuse[0] = LightDiffuse[0] + 0.1;
      break;
    case VK_DOWN:       
      if(LightDiffuse[2] < - 0.9) LightDiffuse[2] = - 1.0; else LightDiffuse[2] = LightDiffuse[2] - 0.1;
      break;
    case VK_UP:     
      if(LightDiffuse[2] >   0.9) LightDiffuse[2] =   1.0; else LightDiffuse[2] = LightDiffuse[2] + 0.1;
      break;
    case VK_NEXT: 
      if(LightDiffuse[1] < - 0.9) LightDiffuse[1] = - 1.0; else LightDiffuse[1] = LightDiffuse[1] - 0.1;
      break;
    case VK_PRIOR:   
      if(LightDiffuse[1] >   0.9) LightDiffuse[1] =   1.0; else LightDiffuse[1] = LightDiffuse[1] + 0.1;
      break;
    default:  break;
    }
  }
}


void CLab4::SetShadowMatrix(float fDestMat[16],float fLightPos[4],float fPlane[4])
{
  float dot;
  dot = fPlane[0]*fLightPos[0] + fPlane[1]*fLightPos[1] + fPlane[2]*fLightPos[2] + fPlane[3]*fLightPos[3];

  // first column
  fDestMat[0]  = dot  - fLightPos[0] * fPlane[0];
  fDestMat[4]  = 0.0f - fLightPos[0] * fPlane[1];
  fDestMat[8]  = 0.0f - fLightPos[0] * fPlane[2];
  fDestMat[12] = 0.0f - fLightPos[0] * fPlane[3];

  // second column
  fDestMat[1]  = 0.0f - fLightPos[1] * fPlane[0];
  fDestMat[5]  = dot  - fLightPos[1] * fPlane[1];
  fDestMat[9]  = 0.0f - fLightPos[1] * fPlane[2];
  fDestMat[13] = 0.0f - fLightPos[1] * fPlane[3];

  // third column
  fDestMat[2]  = 0.0f - fLightPos[2] * fPlane[0];
  fDestMat[6]  = 0.0f - fLightPos[2] * fPlane[1];
  fDestMat[10] = dot  - fLightPos[2] * fPlane[2];
  fDestMat[14] = 0.0f - fLightPos[2] * fPlane[3];

  // fourth column
  fDestMat[3]  = 0.0f - fLightPos[3] * fPlane[0];
  fDestMat[7]  = 0.0f - fLightPos[3] * fPlane[1];
  fDestMat[11] = 0.0f - fLightPos[3] * fPlane[2];
  fDestMat[15] = dot  - fLightPos[3] * fPlane[3];
}


void CLab4::flipLight()
{
  glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
  SetShadowMatrix(fShadowMatrix, LightPosition, Plane);
}
