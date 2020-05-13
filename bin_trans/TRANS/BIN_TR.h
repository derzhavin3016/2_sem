#ifndef __BIN_TR__
#define __BIN_TR__

#include "../STRS/strings.h"
#include "../../../!FST_SEM(C++)/Processor/proc.h"

using uchar = unsigned char;



struct buffer
{
  uchar *buf;
  size_t size;

  buffer( void );

  buffer( size_t size );

  buffer( const buffer &bf ) = delete;

  void Fill( const char filename[] );

  bool Put( const char filename[] );

  uchar & operator [] ( size_t index );

  buffer( buffer &&bf );

  ~buffer( void );
};

bool Translate( const buffer &in, buffer &out );

// load bytes to buffer -> FillBuff (already exists)
// in cycle check every byte and translate to x86 bytecodes
// create .exe



/* Debug memory allocation support */ 
#ifndef NDEBUG 
# define _CRTDBG_MAP_ALLOC
# include <crtdbg.h> 
# define SetDbgMemHooks() \
  _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF | \
  _CRTDBG_ALLOC_MEM_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))

static class __Dummy 
{ 
public: 
  /* Class constructor */
  __Dummy( void ) 
  { 
    SetDbgMemHooks(); 
  } /* End of '__Dummy' constructor */
} __ooppss;

#endif /* _DEBUG */ 

#ifdef _DEBUG 
# ifdef _CRTDBG_MAP_ALLOC 
#   define new new(_NORMAL_BLOCK, __FILE__, __LINE__) 
# endif /* _CRTDBG_MAP_ALLOC */ 
#endif /* _DEBUG */


#endif /* __BIN_TR__ */


/* END OF 'BIN_TR.CPP' FILE */