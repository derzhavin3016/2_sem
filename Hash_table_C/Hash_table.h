#ifndef __HASH_TABLE_H_
#define __HASH_TABLE_H_

#include "Simple_list/Lst.h"
#include "Strings/strings.h"

typedef unsigned long hash_t;

extern const String BUFFER[];

const size_t TABLE_SIZE = 50000;

struct hash_table
{
  list table[TABLE_SIZE];
  // pointer to hash function
  hash_t (*Hash_func)( const String* str );
  const String *strs;
  size_t buf_size, strs_size;
  bool IsHeaderPrint;
};

void   Clear     ( hash_table *this_ );

void   TableInit ( hash_table *this_ );

bool   LoadTxt   ( hash_table *this_, const char filename[] );

bool   Insert    ( hash_table *this_, const String *str, bool IsDic = true );

hash_t Find      ( hash_table *this_, const String *str, hash_t hsh = 0 );

void   Hashing   ( hash_table *this_, hash_t (*Hash)( const String *str ), bool IsDic  = true );

void   TableClose( hash_table *this_ );

#if 1
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
#endif

#endif /* __HASH_TABLE_H_ */

