#pragma once

#include <gl/GL.h>
#include <vector>
#include <math.h>


#define OPENGL_PIXELSIZE    1.0


namespace OpenGL
{

inline void SetPixel(GLint nX, GLint nY)
{
  glBegin(GL_QUADS);
  {
    glVertex2d((double)nX - OPENGL_PIXELSIZE / 2 + 0.1, (double)nY - OPENGL_PIXELSIZE / 2 + 0.1);
    glVertex2d((double)nX - OPENGL_PIXELSIZE / 2 + 0.1, (double)nY + OPENGL_PIXELSIZE / 2 - 0.1);
    glVertex2d((double)nX + OPENGL_PIXELSIZE / 2 - 0.1, (double)nY + OPENGL_PIXELSIZE / 2- 0.1);
    glVertex2d((double)nX + OPENGL_PIXELSIZE / 2 - 0.1, (double)nY - OPENGL_PIXELSIZE / 2 + 0.1);
  }
  glEnd();
}



class CRaster
{
public:
  std::vector<COLORREF> m_arrPixels;
  int m_nWidth,
      m_nHeight;

public:
  CRaster()
  {
    m_nWidth = 0;
    m_nHeight = 0;
  }

  CRaster(int nWidth, int nHeight)
  {
    Resize(nWidth, nHeight);
  }

  CRaster(const CRaster &RasterToCopy)
    : m_arrPixels(RasterToCopy.m_arrPixels)
  {
    m_nWidth = RasterToCopy.m_nWidth;
    m_nHeight = RasterToCopy.m_nHeight;
  }


  COLORREF Get(int nX, int nY)
  {
    return m_arrPixels[nY * m_nWidth + nX];    
  }


  void Set(int nX, int nY, COLORREF cl)
  {
    m_arrPixels[nY * m_nWidth + nX] = cl;
  }


  void Resize(int nWidth, int nHeight)
  {
    m_arrPixels.resize(nWidth * nHeight);
    m_nWidth = nWidth;
    m_nHeight = nHeight;
  }


  void Draw()
  {
    int nPixel = 0;
    for (int nY = 0; nY < m_nHeight; nY++)
      for(int nX = 0; nX < m_nWidth; nX++)
      {
        glColor3ubv((GLubyte*)&m_arrPixels[nPixel++]);
        SetPixel(nX, nY);
      }
  }


  void Antialiasing(int nWnd = 1)
  {
    //  антиалиасинг с шириной окна в nWnd пикселей

    CRaster Copy(*this);
    
    //  TODO: доделать (крайние пиксели на растре пока игнорируются)

    for (int nY = nWnd; nY < m_nHeight - nWnd; nY++)
      for(int nX = nWnd; nX < m_nWidth - nWnd; nX++)
      {
        //  находим сумму цветов всех пикселей
        COLORREF nColorSum = 0;

        for(int nYW = nY - nWnd; nYW <= nY + nWnd; nYW++)
          for(int nXW = nX - nWnd; nXW <= nX + nWnd; nXW++)
            nColorSum += Copy.Get(nXW, nYW);
        
        //  делим на кол-во пикселей
        nColorSum /= pow((double)nWnd * 2 + 1, 2);

        Set(nX, nY, nColorSum);
      }
  }
};


extern CRaster Raster;
}