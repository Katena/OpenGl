#ifndef __ENGINE
#define __ENGINE

#include "api.h"

class Engine {
	GLsizei Height, Width;
    
	GLvoid SetProjectionMatrix(GLvoid);						// ������� ������������ ��������������
	GLvoid SetModelviewMatrix(GLvoid);						// ������� �������������� ������-����
public:
	GLvoid Resize(GLsizei width, GLsizei height);			// �������, ���������� ��� ��������� �������� ����
	GLvoid Init(GLvoid);									// �������, ��� ������� ��������� ����������
	GLvoid Draw(GLvoid);									// ��������� (render) �����
};

#endif