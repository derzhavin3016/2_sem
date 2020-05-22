#include "TRANS/BIN_TR.h"

const unsigned NAME_SIZE = 100;

#define X(a) a + 4

int main( void )
{
  buffer in;
  char filename[NAME_SIZE] = "j.adasm";

  //printf("Input file name to load code from:\n");
  //scanf_s("%s", filename, NAME_SIZE);

  in.Fill(filename);
  buffer out(in.size * 100);

  Translate(in, out);

  out.Put("j_0.txt");
  return 0;
}