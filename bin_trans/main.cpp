#include "TRANS/BIN_TR.h"

const unsigned NAME_SIZE = 100;

int main( void )
{
  buffer in;
  char filename[NAME_SIZE] = {0};

  printf("Input file name to load code from:\n");
  scanf_s("%s", filename, NAME_SIZE);

  in.Fill(filename);
  buffer out(in.size * 10);

  Translate(in, out);

  return 0;
}