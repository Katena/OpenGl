#include "stdafx.h"
#include "engine.h"
#define _USE_MATH_DEFINES 
#include <math.h>




GLvoid Engine::SetProjectionMatrix(GLvoid)
{
	glMatrixMode(GL_PROJECTION);						// �������� ����� ������������� � �������� ��������
	glLoadIdentity();									      // ������� ������� (��������) ������������ �� ���������
	glFrustum(-1, 1, -1, 1, 1, 100);					// ��������������� ������������� ��������, �������� � ������� ����� ���������
}


GLvoid Engine::SetModelviewMatrix(GLvoid)
{
	glMatrixMode(GL_MODELVIEW);							// �������� ����� ������������� � �������� ������
	glLoadIdentity();									      // ������� ������� (������) ������������ �� ���������
  glTranslatef(0.0, 0.0, -8.0);						// ������� ��������� ����������� ������ �����													
  glRotatef(30.0, 1.0, 0.0, 0.0);					// � �������������� �� 30 �������� ������ ��� x,
  glRotatef(50.0, 0.0, 1.0, 0.0);	              // � ����� �� 50 �������� ������ ��� y
  glRotatef(30.0, 0.0, 0.0, 1.0);                  // � ����� �� 30 �������� ������ ��� z
}



GLvoid Engine::Resize(GLsizei width, GLsizei height)
{
	if (height == 0)									
	{
		height = 1;										
	}

  GLsizei nSize = min(width, height);
	glViewport(0, 0, nSize, nSize);					// ��������������� ������� ���������
  //glViewport(0, 0, width, height);					// ��������������� ������� ���������

	Height = height;
	Width = width;

	SetProjectionMatrix();
	SetModelviewMatrix();
}


GLvoid Engine::Init(GLvoid)
{
	glClearColor(0.2f, 0.5f, 0.75f, 1.0f);				// ��������������� ����� ���
	glClearDepth(1.0f);									// ��������������� �������� ���
														          // ���������� ������ ������� �� ���������
	glEnable(GL_DEPTH_TEST);						// ���������� ���� �������
	glDepthFunc(GL_LEQUAL);							// ��������������� ��������, ������������
														          // � ���������� ��� �������������
														          // ������ �������

	glShadeModel(GL_SMOOTH);						// ���������� ������� ���������
	glEnable(GL_LINE_SMOOTH);						// ���������� ����������� �����
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);		// ���������� ����� ������������
														                // ����� ����������� ��� �����
	glEnable(GL_BLEND);									// ���������� �������� ������, �����������
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	// ��� ������ ����������� � ��������
														                          // ������ ��������
}


GLvoid Engine::Draw(GLvoid)									
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// ��������� ����� ����� � ����� �������
	
  glPushMatrix();										// ������������ ������� ������
  {
    
   
	  
	glutWireCube(5.0f);									// �������� ����������� ��� �� �������� 5
    //  ��� ���������

    glBegin(GL_LINES);
    {
      //  ������� - �
      glColor3f(0.0f, 1.0f, 0.0f);
      glVertex3d(-150.0, 0.0, 0.0);
      glVertex3d(+150.0, 0.0, 0.0);

      //  ������ - �
      glColor3f(0.0f, 0.0f, 0.0f);
      glVertex3d(0.0, -150.0, 0.0);
      glVertex3d(0.0, +150.0, 0.0);

      //  ������� - z
      glColor3f(1.0f, 0.1f, 0.1f);
      glVertex3d(0.0, 0.0, -150.0);
      glVertex3d(0.0, 0.0, +150.0);
    }
    glEnd();

	//���
  	glutWireTorus(0.7,1.5,20.0,20.0);    // ��������� ���
  //glutSolidTorus(0.7,1.5,20.0,20.0);   // ����������� ���
	
	

  }
	glPopMatrix();										// ����������������� ������� ������
}
