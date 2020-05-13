#include "TRANS/BIN_TR.h"

const size_t NAME_SIZE = 100;

int main( void )
{
  buffer in;
  char filename[NAME_SIZE] = {0};

  printf("Input file name to load code from:\n");
  scanf_s("%s", filename, NAME_SIZE);

  in.Fill(filename);
  buffer out(in.size);

  Translate(in, out);

  return 0;
}