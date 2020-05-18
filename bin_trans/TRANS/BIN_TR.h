#ifndef __BIN_TR__
#define __BIN_TR__

#include "../STRS/strings.h"
#include "../../../../!FST_SEM(C++)/Processor/proc.h"
#include "../opcodes.h"

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
  char *adasm_ptr;    // pointer to jmp command in adasm code
  char *x86_ptr;      // pointer to jmp command in x86 code
  char *adasm_dest;   // pointer to jump to in adasm code
  char *x86_dest;     // pointer to jump to in x86 code (relative, count from next command)
};

struct jmp_table
{
  jmp_inf table[JMP_SIZE];
  size_t size;

  jmp_table( void );

  int JmpProcess( char *adasm_ptr, char *x86_ptr );

  bool CmdProcess( char *adasm_addr, char *x86_addr, bool IsJmp = false );

  bool PushOldInf( char *adasm_ptr, char *adasm_dest );
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