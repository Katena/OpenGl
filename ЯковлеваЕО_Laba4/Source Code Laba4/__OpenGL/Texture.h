#pragma once

#include <gl/GL.h>
#include <gl/GLaux.h>


namespace OpenGL
{

  AUX_RGBImageRec *LoadBMP(TCHAR *szFilename)				
  {
    if (!szFilename)										
      return NULL;									

    FILE *File = _tfopen( szFilename, _T("r") );							
    if (File)											
    {
      fclose(File);									
      return auxDIBImageLoad(szFilename);				
    }

    return NULL;										
  }


  bool LoadGLTexture(GLuint &nTexture, TCHAR *szFilename)									
  {
    bool Status = false;									

    AUX_RGBImageRec *TextureImage = NULL;					

    
    if ( TextureImage = LoadBMP(szFilename) )
    {
      Status = true;									

      glGenTextures(1, &nTexture);					

      
      glBindTexture(GL_TEXTURE_2D, nTexture);
      glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage->sizeX, TextureImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage->data);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    if (TextureImage)									
    {
      if (TextureImage->data)							
        free(TextureImage->data);					
      free(TextureImage);								
    }

    return Status;										
  }
}