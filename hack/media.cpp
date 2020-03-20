#include "media.h"

byte Frame[FRAME_H][FRAME_W][3];

int WinNum = 0;

const char *Phrases[] = 
{
  "Hacking penthagone",
  "Finding the key byte in program",
  "Patching..."
};
const size_t Phrases_size = sizeof(Phrases) / sizeof(Phrases[0]);

/**
 * \brief Put pixel to screen function.
 * \param [in] x - x coordinate 
 * \param [in] y - y coordinate
 * \param [in] c - reference to color to put.
 */
void PutPixel( int x, int y, const col &c )
{
  Frame[y][x][0] = c.b;
  Frame[y][x][1] = c.g;
  Frame[y][x][2] = c.r;
} /* End of 'PutPixel' fucntion */


/**
 * \brief Play music from .wav file function.
 * \param [in]  filename - name of a file with music.
 * \return true if all is OK, false otherwise.
 */
bool PlayMusic( const char filename[] )
{
  return PlaySound(filename, NULL, SND_ASYNC);
} /* End of 'PlayMusic' function */

/**
 * \brief Create loading window function
 * \return true if all is OK, false otherwise.
 */
bool LoadWnd( int *argc, char *argv[], int width, int height, 
                int posx /* = 0 */, int posy /* = 0 */ )
{
  glutInit(argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

  glutInitWindowPosition(posx, posy);
  glutInitWindowSize(width, height);
  WinNum = glutCreateWindow("Keygen ver 22.8");

  glutDisplayFunc(Display);
  glutKeyboardFunc(Keyboard);

  glutMainLoop();

  return true;
} /* End of 'Window' function */

// Display fucntion
void Display( void )
{
  glClearColor(0.3, 0.5, 0.7, 1);
  glClear(0);

  glRasterPos2d(-1, 1);
  glPixelZoom(Zoom, -Zoom);

  static int procents = 0;
  system("cls");

  if (procents == 100)
  {
    glutDestroyWindow(WinNum);
    throw 0;
  }

  DrawBar(procents++, BAR_H, BAR_W, BAR_X, BAR_Y);

  glDrawPixels(FRAME_W, FRAME_H, GL_BGR_EXT, GL_UNSIGNED_BYTE, Frame);

  srand(clock());
  printf("%d%% - %s...", procents - 1, Phrases[rand() % Phrases_size]);

  srand(clock());
  Sleep(10 * (rand() % 10) * 10);


  glFinish();
  glutSwapBuffers();
  glutPostRedisplay();
} /* End of 'Display' function */

// Keyboard fucntion
void Keyboard( byte key, int, int )
{
  switch (key)
  {
  case VK_ESCAPE:
    exit(0);
    break;
  }
} /* End of 'Keyboard' function */


void DrawBar( int proc, int height, int width, int x0, int y0 )
{
  int proc_w = (width * proc) / 100.0;

  for (int y = 0; y < height; y++)
    for (int x = 0; x < width; x++)
    {
      if (y == 0 || y == height - 1 || x == 0 || x == width - 1)
      {
        PutPixel(x + x0, y + y0, col(255, 255, 255));
        continue;
      }
      if (x < proc_w)
        PutPixel(x + x0, y + y0, col(0, 255, 0));
    }

} /* End of 'DrawBar' function */

// Draw Text on screen
void DrawTxt( const char *buf, size_t len, int x0, int y0, const col &c )
{
  glRasterPos2d((double)-x0 / FRAME_W, (double)y0 / FRAME_H);
  glColor3b(c.r, c.g, c.b);
  for (size_t i = 0; i < len; i++)
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, buf[i]);
}

/* END OF 'MEDIA.CPP' FILE */