

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN            

#include <windows.h>


#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      

#include <atlbase.h>
#include <atlstr.h>



#include <atlwin.h>
#include <atlapp.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glaux.h>

#pragma comment( lib, "opengl32.lib" )							
#pragma comment( lib, "glu32.lib" )								
#pragma comment( lib, "glaux.lib" )								
#pragma comment( lib, "glut32.lib" )							



