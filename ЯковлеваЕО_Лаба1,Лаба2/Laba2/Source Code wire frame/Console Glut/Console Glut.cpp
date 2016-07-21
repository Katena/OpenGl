

#include "stdafx.h"

#include "engine.h"


Engine *engine;


GLvoid display(GLvoid)
{
  engine->Draw();
  glutSwapBuffers();
}


GLvoid reshape(GLsizei width, GLsizei height)
{
  engine->Resize(width, height);
}


int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowSize(800, 600);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("(c)Kate 2012. Laba 2 :D");
  
  engine = new Engine();  
  engine->Init();

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  
  glutMainLoop();
  
  return 0;
}
