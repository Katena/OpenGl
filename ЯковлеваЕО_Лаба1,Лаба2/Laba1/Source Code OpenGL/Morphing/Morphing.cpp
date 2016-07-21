
#include <windows.h>									
#include <math.h>										
#include <stdio.h>										
#include <gl\gl.h>										
#include <gl\glu.h>										
HDC			hDC=NULL;									
HGLRC		hRC=NULL;									
HWND		hWnd=NULL;									
HINSTANCE	hInstance;									

bool		keys[256];									 
bool		active=TRUE;								
bool		fullscreen=TRUE;							

GLfloat		xrot,yrot,zrot,								// X, Y & Z поворот
			xspeed,yspeed,zspeed,						// X, Y & Z скорость спина
			cx,cy,cz=-15;								// X, Y & Z позиции

int			key=1;										
int			step=0,steps=200;							// Max и нач. число шаков
bool		morph=FALSE;								

typedef struct											
{
	float	x, y, z;									
} VERTEX;												

typedef struct											
{
 int		verts;										
 VERTEX		*points;									
} OBJECT;												

int			maxver;									
OBJECT	morph1, morph2,				  
			  helper, *sour, *dest;	  


LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	


void objallocate(OBJECT *k,int n)						
{														
	k->points=(VERTEX*)malloc(sizeof(VERTEX)*n);		
}														


void objfree(OBJECT *k)									
{
	free(k->points);									
}


void readstr(FILE *f,char *string)						
{
	do													
	{
		fgets(string, 255, f);							
	} while ((string[0] == '/') || (string[0] == '\n'));
	return;												
}


void objload(char *name,OBJECT *k)						
{
	int		ver;										
	float	rx,ry,rz;									
	FILE	*filein;									
	char	oneline[255];								

	filein = fopen(name, "rt");							
														
	readstr(filein,oneline);							
	sscanf(oneline, "Vertices: %d\n", &ver);			
	k->verts=ver;										
	objallocate(k,ver);									

	for (int i=0;i<ver;i++)							
	{
		readstr(filein,oneline);						
		sscanf(oneline, "%f %f %f", &rx, &ry, &rz);		
		k->points[i].x = rx;							
		k->points[i].y = ry;							
		k->points[i].z = rz;							
	}
	fclose(filein);										

	if(ver>maxver) maxver=ver;							
}														
														

VERTEX calculate(int i)									
{
	VERTEX a;											
	a.x=(sour->points[i].x-dest->points[i].x)/steps;	
	a.y=(sour->points[i].y-dest->points[i].y)/steps;	
	a.z=(sour->points[i].z-dest->points[i].z)/steps;	
	return a;											
}														

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		
{
	if (height==0)										
	{
		height=1;										
	}

	glViewport(0,0,width,height);						

	glMatrixMode(GL_PROJECTION);						
	glLoadIdentity();									

	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);							
	glLoadIdentity();									
}


int InitGL(GLvoid)										
{
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);					
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);				
	glClearDepth(1.0);									
	glDepthFunc(GL_LESS);								
	glEnable(GL_DEPTH_TEST);							
	glShadeModel(GL_SMOOTH);							
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	

	maxver=0;											
	objload("data/sphere.txt",&morph1);					
	objload("data/torus.txt",&morph2);					

  objload("data/sphere.txt",&helper);					
	sour=dest=&morph1;									

	return TRUE;										
}


void DrawGLScene(GLvoid)								
{
  //  перед началом рисования сцену очищаем
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glLoadIdentity();									

  //  перемещаем курсор рисования в заданную позицию (cx, cy, cz)
  //  (эта позиция управляется с клавиатуры)
  glTranslatef(cx, cy, cz);								

  //  устанавливаем нужный угол курсора рисования
  //  (угол меняется от кадра к кадру, 
  //   скорость поворота вокруг осей управляется с клавиатуры)
  glRotatef(xrot, 1, 0, 0);								
	glRotatef(yrot, 0, 1, 0);							
	glRotatef(zrot, 0, 0, 1);							

  //  рисуем все точки фигуры

	VERTEX q; //  вычисляемые значения (дельта-x, дельта-y и дельта-z) для каждой точки

	glBegin(GL_POINTS);									
    //  цикл по всем точкам фигуры
		for(int i = 0; i < morph1.verts; i++)  
		{						
      //  если морфинг в процессе, то изменяем координаты точки (x, y, z) на (дельта-x, -y, -z)
			if(morph) 
      {
        q = calculate(i);   //  найдем (дельта-x, дельта-y и дельта-z) для текущей точки

        //  двигаем точку на дельта-...
        helper.points[i].x -= q.x;
        helper.points[i].y -= q.y;					
        helper.points[i].z -= q.z;					
      }
			
      //  рисуем точку
			glColor3f(0,1,1);							
			glVertex3f(helper.points[i].x,
                 helper.points[i].y,
                 helper.points[i].z);					
		}												
	glEnd();											

  //  вычисления для следующего кадра

  //  изменяем угол курсора рисования (скорость изменения управляется с клавиатуры)
  xrot += xspeed; 
  yrot += yspeed; 
  zrot += zspeed;			

  //  если морфинг в процессе (не достингли шага №200), то увеличиваем текущий шаг морфинга на 1
	if(morph && step<=steps)
    step++; 
  else 
  { 
    //  а иначе морфинг окончен
    morph=FALSE; 
    sour=dest; 
    step=0;
  }
}


GLvoid KillGLWindow(GLvoid)								
{
	objfree(&morph1);									
	objfree(&morph2);									
	objfree(&helper);									

	if (fullscreen)										
	{
		ChangeDisplaySettings(NULL,0);					
		ShowCursor(TRUE);								
	}

	if (hRC)											
	{
		if (!wglMakeCurrent(NULL,NULL))					
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										
	}

	if (hWnd && !DestroyWindow(hWnd))					
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										
	}

	if (!UnregisterClass("OpenGL",hInstance))			
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									
	}
}



BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;			
	WNDCLASS	wc;						
	DWORD		dwExStyle;				
	DWORD		dwStyle;				
	RECT		WindowRect;				
	WindowRect.left=(long)0;			
	WindowRect.right=(long)width;		
	WindowRect.top=(long)0;				
	WindowRect.bottom=(long)height;		

	fullscreen=fullscreenflag;			

	hInstance			= GetModuleHandle(NULL);				
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	
	wc.lpfnWndProc		= (WNDPROC) WndProc;					
	wc.cbClsExtra		= 0;									
	wc.cbWndExtra		= 0;									
	wc.hInstance		= hInstance;							
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			
	wc.hbrBackground	= NULL;									
	wc.lpszMenuName		= NULL;									
	wc.lpszClassName	= "OpenGL";								

	if (!RegisterClass(&wc))									
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											
	}

	if (fullscreen)												
	{
		DEVMODE dmScreenSettings;								
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		
		dmScreenSettings.dmPelsWidth	= width;				
		dmScreenSettings.dmPelsHeight	= height;				
		dmScreenSettings.dmBitsPerPel	= bits;					
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{

			if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","NeHe GL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				fullscreen=FALSE;								
			}
			else
			{
				
				MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
				return FALSE;									
			}
		}
	}

	if (fullscreen)												
	{
		dwExStyle=WS_EX_APPWINDOW;								
		dwStyle=WS_POPUP;										
		ShowCursor(FALSE);										
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			
		dwStyle=WS_OVERLAPPEDWINDOW;							
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);	

	
	if (!(hWnd=CreateWindowEx(	dwExStyle,						
								"OpenGL",						
								title,							
								dwStyle |						
								WS_CLIPSIBLINGS |				
								WS_CLIPCHILDREN,				
								0, 0,							
								WindowRect.right-WindowRect.left,	
								WindowRect.bottom-WindowRect.top,	
								NULL,							
								NULL,							
								hInstance,						
								NULL)))							
	{
		KillGLWindow();											
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											
	}

	static	PIXELFORMATDESCRIPTOR pfd=							
	{
		sizeof(PIXELFORMATDESCRIPTOR),							
		1,														
		PFD_DRAW_TO_WINDOW |									
		PFD_SUPPORT_OPENGL |									
		PFD_DOUBLEBUFFER,										
		PFD_TYPE_RGBA,											
		bits,													
		0, 0, 0, 0, 0, 0,										
		0,														
		0,														
		0,														
		0, 0, 0, 0,												
		16,														
		0,														
		0,														
		PFD_MAIN_PLANE,											
		0,														
		0, 0, 0													
	};

	if (!(hDC=GetDC(hWnd)))										
	{
		KillGLWindow();											
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))				
	{
		KillGLWindow();											
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))					
	{
		KillGLWindow();											
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											
	}

	if (!(hRC=wglCreateContext(hDC)))							
	{
		KillGLWindow();											
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											
	}

	if(!wglMakeCurrent(hDC,hRC))								
	{
		KillGLWindow();											
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											
	}

	ShowWindow(hWnd,SW_SHOW);									
	SetForegroundWindow(hWnd);									
	SetFocus(hWnd);												
	ReSizeGLScene(width, height);								

	if (!InitGL())												
	{
		KillGLWindow();											
		MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											
	}

	return TRUE;												
}


LRESULT CALLBACK WndProc(	HWND	hWnd,						
							UINT	uMsg,						
							WPARAM	wParam,						
							LPARAM	lParam)						
{
	switch (uMsg)												
	{
		case WM_ACTIVATE:										
		{
			if (!HIWORD(wParam))								
			{
				active=TRUE;									
			}
			else												
			{
				active=FALSE;									
			}

			return 0;											
		}

		case WM_SYSCOMMAND:										
		{
			switch (wParam)										
			{
				case SC_SCREENSAVE:								
				case SC_MONITORPOWER:							
				return 0;										
			}
			break;												
		}

		case WM_CLOSE:											
		{
			PostQuitMessage(0);									
			return 0;											
		}

		case WM_KEYDOWN:										
		{
			keys[wParam] = TRUE;								
			return 0;											
		}

		case WM_KEYUP:											
		{
			keys[wParam] = FALSE;								
			return 0;											
		}

		case WM_SIZE:											
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));		
			return 0;											
		}
	}

	
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}


int WINAPI WinMain(	HINSTANCE	hInstance,						
					HINSTANCE	hPrevInstance,					
					LPSTR		lpCmdLine,						
					int			nCmdShow)						
{
	MSG		msg;												
	BOOL	done=FALSE;											

	
	if (MessageBox(NULL,"Would You Like To Run In Fullscreen Mode?", "Start FullScreen?",MB_YESNO|MB_ICONQUESTION)==IDNO)
		fullscreen=FALSE;										

	
	if (!CreateGLWindow("(c)Kate 2012 laba2 Morphing",640,480,16,fullscreen))
		return 0;												

	while(!done)												
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))				
		{
			if (msg.message==WM_QUIT)							
			{
				done=TRUE;										
			}
			else												
			{
				TranslateMessage(&msg);							
				DispatchMessage(&msg);							
			}
		}
		else													
		{
			
			if (active && keys[VK_ESCAPE])						
			{
				done=TRUE;										
			}
			else												
			{
				DrawGLScene();									
				SwapBuffers(hDC);								

				if(keys[VK_PRIOR])								
					zspeed+=0.01f;								

				if(keys[VK_NEXT])								
					zspeed-=0.01f;								

				if(keys[VK_DOWN])								
					xspeed+=0.01f;								

				if(keys[VK_UP])									
					xspeed-=0.01f;								

				if(keys[VK_RIGHT])								
					yspeed+=0.01f;								

				if(keys[VK_LEFT])								
					yspeed-=0.01f;								

				if (keys['Q'])									
				 cz-=0.01f;										

				if (keys['Z'])									
				 cz+=0.01f;										

				if (keys['W'])									
				 cy+=0.01f;										

				if (keys['S'])									
				 cy-=0.01f;										

				if (keys['D'])									
				 cx+=0.01f;										

				if (keys['A'])									
				 cx-=0.01f;										

				if (keys['1'] && (key!=1) && !morph)			
				{
					key=1;										
					morph=TRUE;									
					dest=&morph1;								
				}
				if (keys['2'] && (key!=2) && !morph)			
				{
					key=2;										
					morph=TRUE;									
					dest=&morph2;								
				}

				if (keys[VK_F1])								
				{
					keys[VK_F1]=FALSE;							
					KillGLWindow();								
					fullscreen=!fullscreen;						
					
					if (!CreateGLWindow("(c)Kate 2012 laba2 Morphing",640,480,16,fullscreen))
					{
						return 0;								
					}
				}

        //Sleep(100);
			}
		}
	}

	
	KillGLWindow();												
	return (msg.wParam);										
}
