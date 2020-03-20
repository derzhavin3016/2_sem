#ifndef __MEDIA_H_
#define __MEDIA_H_

#include "TXLib.h"
#include "glut.h"
#include <Windows.h>

#include <mmsystem.h>

#pragma comment(lib, "winmm")

#pragma warning (disable: 4996)

const int FRAME_W = 500;

const int FRAME_H = 70;

const int POS_X = 300;

const int POS_Y = 300;

const int BAR_H = 50;

const int BAR_W = 400;

const int BAR_X = 50;

const int BAR_Y = 10;

const double Zoom = 1;

typedef unsigned char byte;

struct col
{
  byte r, g, b;

  col( byte red = 0, byte green = 0, byte blue = 0 ) : r(red), 
                                                       g(green),
                                                       b(blue)
  {
  }
};

bool PlayMusic( const char filename[] );

bool LoadWnd( int *argc, char *argv[], int width = FRAME_W, int height = FRAME_H, int posx = POS_X, int posy = POS_Y );

void Display( void );

void PutPixel( int x, int y, const col &c );

void Keyboard( byte key, int, int );

void DrawBar( int proc, int height, int width, int x0, int y0 );

void DrawTxt( const char *buf, size_t len, int x0, int y0, const col &c );

#endif /* __MEDIA_H_ */

/* END OF 'MEDIA.H' FILE */
