#include "stdafx.h"
#include "engine.h"
#define _USE_MATH_DEFINES 
#include <math.h>


#define N_POINTS    50
#define k    3.0
#define kt 20
// procedure Line(x1, y1, x2, y2 : double);
// BEGIN
//  ...
// end;
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
      x += dX; //  x = x + dX;
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


void CircleAroundSquare(double x1, double y1, double x2, double y2)
{
  double x = x1 + (x2 - x1) / 2;
  double y = y1 + (y2 - y1) / 2;
  double r = sqrt( pow((x2 - x1) / 2, 2) + pow((y2 - y1) / 2, 2) );
  Circle(x, y, r);
}

/*procedure torus;
var i,j:integer;
begin
for j := 0 to 20 do
begin
glBegin(GL_QUAD_STRIP);
for i := 0 to 20 do
begin
glVertex3f((0.5+0.125*cos(2*Pi*j/20))*cos(2*Pi*i/20),
(0.5+0.125*cos(2*Pi*j/20))*sin(2*Pi*i/20),
0.125*sin(2*Pi*j/20));
glVertex3f((0.5+0.125*cos(2*Pi*(j+1)/20))*cos(2*Pi*i/20),
(0.5+0.125*cos(2*Pi*(j+1)/20))*sin(2*Pi*i/20),
0.125*sin(2*Pi*(j+1)/20));
end;
glEnd;
end;
end;*/

/* void tor()
{
	
	
		glBegin(GL_QUAD_STRIP);
		   	 glVertex3f(0.5,1.0,2.0); //(0.5+0.125*cos(2*M_PI*j/20.00))*cos(2*M_PI*i/20),(0.5+0.125*cos(2*M_PI*j/20))*sin(2*M_PI*i/20),0.125*sin(2*M_PI*j/20));
             glVertex3f(3.0,1.0,0.5);   //(0.5+0.125*cos(2*M_PI*(j+1)/20))*cos(2*M_PI*i/20),(0.5+0.125*cos(2*M_PI*(j+1)/20))*sin(2*M_PI*i/20),0.125*sin(2*M_PI*(j+1)/20));
		
		glEnd();
	


}*/



GLvoid Engine::SetProjectionMatrix(GLvoid)
{
	glMatrixMode(GL_PROJECTION);						// ƒействи€ будут производитьс€ с матрицей проекции
	glLoadIdentity();									      // “екуща€ матрица (проекции) сбрасываетс€ на единичную
	glFrustum(-1, 1, -1, 1, 1, 100);					// ”станавливаетс€ перспективна€ проекци€, передн€€ и дальн€€ грани отсечени€
}


GLvoid Engine::SetModelviewMatrix(GLvoid)
{
	glMatrixMode(GL_MODELVIEW);							// ƒействи€ будут производитьс€ с матрицей модели
	glLoadIdentity();									      // “екуща€ матрица (модели) сбрасываетс€ на единичную
  glTranslatef(0.0, 0.0, -100.0);						// —истема координат переноситс€ вглубь сцены													
  // glRotatef(30.0, 1.0, 0.0, 0.0);					// и поворачиваетс€ на 30 градусов вокруг оси x,
	//glRotatef(70.0, 0.0, 1.0, 0.0);	              // а затем на 70 градусов вокруг оси y
	//glRotatef(30.0, 0.0, 0.0, 1.0);
}



GLvoid Engine::Resize(GLsizei width, GLsizei height)
{
	if (height == 0)									
	{
		height = 1;										
	}

  GLsizei nSize = min(width, height);
	glViewport(0, 0, nSize, nSize);					// ”станавливаетс€ область просмотра
  //glViewport(0, 0, width, height);					// ”станавливаетс€ область просмотра

	Height = height;
	Width = width;

	SetProjectionMatrix();
	SetModelviewMatrix();
}


GLvoid Engine::Init(GLvoid)
{
	glClearColor(0.2f, 0.5f, 0.75f, 1.0f);				// ”станавливаетс€ синий фон
	glClearDepth(1.0f);									// ”станавливаетс€ значение дл€
														          // заполнени€ буфера глубины по умолчанию
	glEnable(GL_DEPTH_TEST);						// ¬ключаетс€ тест глубины
	glDepthFunc(GL_LEQUAL);							// ”станавливаетс€ значение, используемое
														          // в сравнени€х при использовании
														          // буфера глубины

	glShadeModel(GL_SMOOTH);						// ¬ключаетс€ плавное затенение
	glEnable(GL_LINE_SMOOTH);						// ¬ключаетс€ сглаживание линий
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);		// ¬ыбираетс€ самый качественный
														                // режим сглаживани€ дл€ линий
	glEnable(GL_BLEND);									// ¬ключаетс€ смешение цветов, необходимое
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	// дл€ работы сглаживани€ и задаетс€
														                          // способ смешени€
}


GLvoid Engine::Draw(GLvoid)									
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// ќчищаетс€ буфер кадра и буфер глубины
	
  glPushMatrix();										// «апоминаетс€ матрица модели
  {
    //glColor3f(1.0f, 1.0f, 1.0f);						// «адаетс€ текущий цвет примитивов
    glutWireCube(5.0f);									// –исуетс€ проволочный куб со стороной 20

    //  оси координат

    glBegin(GL_LINES);
    {
      //  зелена€ - х
      glColor3f(0.0f, 1.0f, 0.0f);
      glVertex3d(-150.0, 0.0, 0.0);
      glVertex3d(+150.0, 0.0, 0.0);

      //  черна€ - у
      glColor3f(0.0f, 0.0f, 0.0f);
      glVertex3d(0.0, -150.0, 0.0);
      glVertex3d(0.0, +150.0, 0.0);

      //  красна€ - z
      glColor3f(1.0f, 0.1f, 0.1f);
      glVertex3d(0.0, 0.0, -150.0);
      glVertex3d(0.0, 0.0, +150.0);
    }
    glEnd();

	//тор
  // 	glutWireTorus(0.7,1.5,20.0,20.0);    // каркасный тор
  //glutSolidTorus(0.7,1.5,20.0,20.0);   // закрашенный тор
	
	
	//  рисуем линию по точкам

    glColor3f(1.0f, 1.0f, 0.0f);
    Line(0, 0, 28, 40);

    //  рисуем квадратик и окружность вокруг него

    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);
    {
      glVertex3d(20.0, 40.0, 0.0);
      glVertex3d(20.0, 80.0, 0.0);
      //glColor3f(1.0f, 0.0f, 1.0f);
      glVertex3d(60.0, 80.0, 0.0);
      glVertex3d(60.0, 40.0, 0.0);
	  //xТ = x * cos q Ц y * sin q
	  //yТ = x * sin q + y * cos q
	
	 //  сколько угодно точек
      //  каждые 4 объедин€ютс€ в квадрат
      //  ...
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
	
	
	 //  сколько угодно точек
      //  каждые 4 объедин€ютс€ в квадрат
      //  ...
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
	  //xТ = x * cos q Ц y * sin q
	  //yТ = x * sin q + y * cos q
	
	 //  сколько угодно точек
      //  каждые 4 объедин€ютс€ в квадрат
      //  ...
    }
    glEnd();
	glColor3f(1.0f, 0.0f, 1.0f);
	povorot(20.0,40.0,20.0,80.0,60.0,80.0,60.0,40.0);
	//tor();
	    
	glColor3f(0.0f, 1.0f, 0.0f);
    //Circle(40.0, 60.0, 20.0);
    CircleAroundSquare(50.0, 60.0, 60.0, 80.0);

  }
	glPopMatrix();										// ¬осстанавливаетс€ матрица модели
}
