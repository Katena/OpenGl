#include "stdafx.h"
#include "engine.h"
#define _USE_MATH_DEFINES 
#include <math.h>


#define N_POINTS    200
#define k    3.0
#define kt 20

char c;

void Line(double x1, double y1, double x2, double y2)
{
  double dX = (x2 - x1) / N_POINTS,
         dY = (y2 - y1) / N_POINTS;

  glBegin(GL_POINTS);
  {
    double x = x1, y = y1;
    for (int i = 0; i <= N_POINTS; i++)
    {
      glVertex3d(x, y, 0.0);
      x += dX; 
      y += dY;
    }
  }
  glEnd();
}


void povorot(double x1, double y1,double x2, double y2,double x3, double y3,double x4, double y4)
{
  double x11=x1*cos(M_PI/4) -y1 * sin(M_PI/4)+54.0,
         y11=x1 * sin(M_PI/4) + y1 * cos(M_PI/4)-11.0,
         x22=x2*cos(M_PI/4) -y2 * sin(M_PI/4) +54.0,
         y22=x2 * sin(M_PI/4) + y2 * cos(M_PI/4)-11.0,
		 x33=x3*cos(M_PI/4) -y3 * sin(M_PI/4)+54.0 ,
         y33=x3 * sin(M_PI/4) + y3 * cos(M_PI/4)-11.0,
		 x44=x4*cos(M_PI/4) -y4 * sin(M_PI/4)+54.0 ,
         y44=x4 * sin(M_PI/4) + y4 * cos(M_PI/4)-11.0;
    glBegin(GL_QUADS);
    {
      glVertex3d(x11, y11, 0.0);
      glVertex3d(x22, y22, 0.0);
      //glColor3f(1.0f, 0.0f, 1.0f);
      glVertex3d(x33, y33, 0.0);
      glVertex3d(x44, y44, 0.0);
	  
    }
    glEnd();
}

void Circle(double x, double y, double r)
{
  glBegin(GL_LINE_LOOP);
  for (int i = 0; i <= N_POINTS; i++) 
  {
    double angle = 2. * M_PI * i / N_POINTS;
    glVertex2f(x + r * cos(angle), y + r * sin(angle));
  }
  glEnd();
}

void Af()
{
		int i;
		glColor3f(0.0f, 1.0f, 0.0f);		
		for (int i = 0; i != 15; i++)
		   Circle(5.5,-15+i, 0.5);
		glColor3f(0.0f, 1.0f, 0.0f);		
		for (int i = 0; i != 7; i++)
			Circle(5.5,-15-i, 0.5);
		glColor3f(0.0f, 1.0f, 0.0f);		
		for (int i = 0; i !=8; i++)
			Circle(5.5+(i/2),-22-i, 0.5);
}



void CircleAroundSquare(double x1, double y1, double x2, double y2)
{
  double x = x1 + (x2 - x1) / 2;
  double y = y1 + (y2 - y1) / 2;
  double r = sqrt( pow((x2 - x1) / 2, 2) + pow((y2 - y1) / 2, 2) );
  Circle(x, y, r);
}



GLvoid Engine::SetProjectionMatrix(GLvoid)
{
	glMatrixMode(GL_PROJECTION);						// Действия будут производиться с матрицей проекции
	glLoadIdentity();									      // Текущая матрица (проекции) сбрасывается на единичную
	glFrustum(-1, 1, -1, 1, 1, 100);					// Устанавливается перспективная проекция, передняя и дальняя грани отсечения
}


GLvoid Engine::SetModelviewMatrix(GLvoid)
{
	glMatrixMode(GL_MODELVIEW);							// Действия будут производиться с матрицей модели
	glLoadIdentity();									      // Текущая матрица (модели) сбрасывается на единичную
    glTranslatef(0.0, 0.0, -100.0);						// Система координат переносится вглубь сцены													
    
}



GLvoid Engine::Resize(GLsizei width, GLsizei height)
{
	if (height == 0)									
	{
		height = 1;										
	}

  GLsizei nSize = min(width, height);
	glViewport(0,0,nSize,nSize);					// Устанавливается область просмотра
   // glViewport(0, 0, width, height);					// Устанавливается область просмотра

	Height = height;
	Width = width;

	SetProjectionMatrix();
	SetModelviewMatrix();
}


GLvoid Engine::Init(GLvoid)
{
	glClearColor(0.2f, 0.5f, 0.75f, 1.0f);				// Устанавливается синий фон
	glClearDepth(1.0f);									// Устанавливается значение для
														          // заполнения буфера глубины по умолчанию
	glEnable(GL_DEPTH_TEST);						// Включается тест глубины
	glDepthFunc(GL_LEQUAL);							// Устанавливается значение, используемое
														          // в сравнениях при использовании
														          // буфера глубины

	glShadeModel(GL_SMOOTH);						// Включается плавное затенение
	glEnable(GL_LINE_SMOOTH);						// Включается сглаживание линий
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);		// Выбирается самый качественный
														                // режим сглаживания для линий
	glEnable(GL_BLEND);									// Включается смешение цветов, необходимое
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	// для работы сглаживания и задается
														                          // способ смешения
}


GLvoid Engine::Draw(GLvoid)									
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Очищается буфер кадра и буфер глубины
	
  glPushMatrix();										// Запоминается матрица модели
  {
 
	  
	 glColor3f(1.0f, 1.0f, 1.0f);						// Задается текущий цвет примитивов
    
	
	  
/*	  //  оси координат

    glBegin(GL_LINES);
    {
      //  зеленая - х
      glColor3f(0.0f, 1.0f, 0.0f);
      glVertex3d(-150.0, 0.0, 0.0);
      glVertex3d(+150.0, 0.0, 0.0);

      //  черная - у
      glColor3f(0.0f, 0.0f, 0.0f);
      glVertex3d(0.0, -150.0, 0.0);
      glVertex3d(0.0, +150.0, 0.0);

    }
    glEnd();

	*/
	
	//  рисуем букву "K" по точкам

    glColor3f(1.0f, 1.0f, 0.0f);
    Line(-45, 5, -25, 30);
	Line(-45,-30,-45,30);
	Line(-45, 5, -25, -30);
	 
	// рисуем букву а(окружность + "швостик" из точек(маленьких окружностей))
	glColor3f(0.0f, 1.0f, 0.0f);

	
	Circle(-10.0, -15.0, 15.0);
	Circle(-10.0, -15.0, 14.7);

    /*for(bbb=0.0; bbb=10.0; bbb++)*/
	 
		  /*Circle(55.5,-15.0, 0.5); 
	      Circle(55.5,-14.0, 0.5); 
		  Circle(55.5,-13.0, 0.5); 
		  Circle(55.5,-12.0, 0.5); 
		  Circle(55.5,-11.0, 0.5); */

   Af();

   // буква "Т" по точкам
   Line(25,0,25,-30);
   Line(10, 0, 40, 0);

   // буква "Я" по точкам и окружности

   glColor3f(0.0f, 1.0f, 0.0f);
   Circle(50.0, -8.5, 8.5);
   Line(58.5, 0.0,58.5,-30.0);
   Line(42.5,-30.0,58.5,-12.5);

  

    //  рисуем квадратик и окружность вокруг него

   /* glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);
    {
      glVertex3d(20.0, 40.0, 0.0);
      glVertex3d(20.0, 80.0, 0.0);
      //glColor3f(1.0f, 0.0f, 1.0f);
      glVertex3d(60.0, 80.0, 0.0);
      glVertex3d(60.0, 40.0, 0.0);
	  
	}
    glEnd();
	
	glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    {
      glVertex3d(20.0, 40.0, 0.0);
      glVertex3d(20.0, 80.0*k, 0.0);
      //glColor3f(1.0f, 0.0f, 1.0f);
      glVertex3d(60.0*k, 80.0*k, 0.0);
      glVertex3d(60.0*k, 40.0, 0.0);
	
	
    }
    glEnd();

	glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);
    {
      glVertex3d(20.0, 40.0, 0.0);
      glVertex3d(20.0, 80.0, 0.0);
      //glColor3f(1.0f, 0.0f, 1.0f);
      glVertex3d(60.0, 80.0, 0.0);
      glVertex3d(60.0, 40.0, 0.0);
	 
    }
    glEnd();
	glColor3f(1.0f, 0.0f, 1.0f);
	povorot(20.0,40.0,20.0,80.0,60.0,80.0,60.0,40.0);
	*/
	    
	

  }
	glPopMatrix();										// Восстанавливается матрица модели
}
