#ifndef __BIN_TR__
#define __BIN_TR__

#include "../STRS/strings.h"
#include "../../../../!FST_SEM(C++)/Processor/proc.h"

const size_t JMP_SIZE = 1000;



struct buffer
{
  char *buf;
  size_t size;


  buffer( void );

  buffer( size_t size, char init = 0 );

  buffer( const buffer &bf ) = delete;  // because we don't need to copy any buffer

  void Fill( const char filename[] );

  bool Put( const char filename[] );

  char & operator [] ( size_t index );

  buffer( buffer &&bf );

  ~buffer( void );
};

bool Translate( const buffer &in, buffer &out );

void FillJmpTable( const buffer &in );

//////////////////////////////////////////////////////////////
// load bytes to buffer -> FillBuff (already exists)
// in cycle check every byte and translate to x86 bytecodes
// create .exe
//////////////////////////////////////////////////////////////

struct jmp_inf         // structure for store information about jumps
{
  char *old_ptr;      // pointer to jmp command in adasm code
  char *new_ptr;      // pointer to jmp command in x86 code
  int old_jmp;         // address to jump to in adasm code
  int new_jmp;         // address to jump to in x86 code (relative)
};

struct jmp_table
{
  jmp_inf table[JMP_SIZE];
  size_t size;

  jmp_table( void );

  int JmpProcess( char *jmp_ptr, char *new_ptr );

  int FindByDest( int old_jmp_addr, int new_jmp_addr );

  bool PushOldInf( char *ptr, int dest_addr );
};


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