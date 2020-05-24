#ifndef __BIN_TR__
#define __BIN_TR__

#include "../STRS/strings.h"
#include "../../../../!FST_SEM(C++)/Processor/proc.h"
#include "../opcodes.h"
#include <windows.h>
#include <time.h>

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

bool CreateExe( const char filename[], const buffer &out );

IMAGE_NT_HEADERS CreateNTHeader( int NUM_OF_SEC );

IMAGE_FILE_HEADER CreateFileHeader( int NUM_OF_SEC );

IMAGE_OPTIONAL_HEADER CreateOptHeader( void );

IMAGE_SECTION_HEADER CreateSecHeader( const char sec_name[], size_t vrt_size, 
                                      size_t vrt_addr,       size_t size_raw_data, 
                                      size_t ptr_raw_data,   size_t charac );

const size_t CODE_CHAR = IMAGE_SCN_CNT_CODE | IMAGE_SCN_MEM_EXECUTE | IMAGE_SCN_MEM_READ;

const size_t DATA_CHAR = IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_WRITE | IMAGE_SCN_CNT_INITIALIZED_DATA;

const size_t IDATA_CHAR = IMAGE_SCN_CNT_INITIALIZED_DATA | IMAGE_SCN_MEM_READ;

const size_t ENTRY_POINT_ADDR = 0x1000;

const size_t VRT_SIZE = 0x5000;

const size_t SIZE_RAW = 0x1000;
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


///////////////////// DOS HEADER AND DOS STUB ////////////////////////////
const IMAGE_DOS_HEADER DOS_HEADER = 
{
  .e_magic    = 'ZM',
	.e_cblp     = 0x0090,
	.e_cp       = 0x0003,
	.e_cparhdr  = 0x0004,
	.e_maxalloc = 0xFFFF,
	.e_sp       = 0x00B8,
  .e_lfarlc   = 0x0040,
	.e_lfanew   = 0x00B0
};
///////////////////////////////////////////////////////////////////////////
const unsigned char DOS_STUB[] = 
{
	0x0E,
	0x1F,
	0xBA,
	0x0E,
	0x00,
	0xB4,
	0x09,
	0xCD,
	0x21,
	0xB8,
	0x01,
	0x4C,
	0xCD,
	0x21,
	0x54,
	0x68,
	0x69,
	0x73,
	0x20,
	0x70,
	0x72,
	0x6F,
	0x67,
	0x72,
	0x61,
	0x6D,
	0x20,
	0x63,
	0x61,
	0x6E,
	0x6E,
	0x6F,
	0x74,
	0x20,
	0x62,
	0x65,
	0x20,
	0x72,
	0x75,
	0x6E,
	0x20,
	0x69,
	0x6E,
	0x20,
	0x44,
	0x4F,
	0x53,
	0x20,
	0x6D,
	0x6F,
	0x64,
	0x65,
	0x2E,
	0x0D,
	0x0D,
	0x0A,
	0x24,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x5D,
	0x5C,
	0x6D,
	0xC1,
	0x19,
	0x3D,
	0x03,
	0x92,
	0x19,
	0x3D,
	0x03,
	0x92,
	0x19,
	0x3D,
	0x03,
	0x92,
	0x97,
	0x22,
	0x10,
	0x92,
	0x1E,
	0x3D,
	0x03,
	0x92,
	0xE5,
	0x1D,
	0x11,
	0x92,
	0x18,
	0x3D,
	0x03,
	0x92,
	0x52,
	0x69,
	0x63,
	0x68,
	0x19,
	0x3D,
	0x03,
	0x92,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00
};
//////////////////////////////////////////////////////////////////////////


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