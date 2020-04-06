#include <stdio.h>

extern "C" void Ad6Printf( const char*, ... );

int main( void )
{
  Ad6Printf("Hello %c %s %x %d%%%c %b %o", 'I', "love", 0xeda, 100, '!', 127, 63);
  return 0;
}