#pragma warning (disable: 4124)

#include "patch.h"
#include "media.h"

const char wav[] = "keygen.wav";

const int Name_len = 100;

int main( int argc, char *argv[] )
{
  char filename[Name_len] = {0};
  
  printf("Input filename to patch: ");
  scanf("%s", filename);

  if (CheckCond(!PatchCode(filename), "!!!ERROR: Error in patching.\n"))
    return 1;
   

  if (!PlayMusic(wav))
  {
    printf("Cannot open file '%s' \n", wav);
    return 1;
  }
 
  try
  {
    LoadWnd(&argc, argv);
  }
  catch ( int zero )
  {
    PlaySound(NULL, NULL, 1);
    printf("Patching succed\n");
  }
  return 0;
}
  


