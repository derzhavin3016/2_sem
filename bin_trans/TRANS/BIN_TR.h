#ifndef __BIN_TR__
#define __BIN_TR__

#include "../STRS/strings.h"
#include "../../../../!FST_SEM(C++)/Processor/proc.h"
#include "../opcodes.h"
#include <windows.h>
#include <time.h>
#include <new>

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

const size_t IMAGE_BASE = 0x00400000;

const size_t ENTRY_POINT_ADDR = 0x1000;

const size_t VRT_SIZE = 0x5000;

const size_t SIZE_RAW = 0x1000;

const size_t DATA_START = VRT_SIZE * 2 + ENTRY_POINT_ADDR;

const size_t IMPORT_START = VRT_SIZE + ENTRY_POINT_ADDR;
//////////////////////////////////////////////////////////////
///IMPORT BY NAME
struct import_name
{
  WORD Hint;
  const CHAR *Name;
	size_t size;

  import_name( const char name[], size_t size, WORD hnt = 0 ) : Hint(hnt), Name(name), size(size)
  {
  }

	void Fill( const char name[], size_t size, WORD hnt = 0 )
	{
	  Name = name;
		this->size = size;
		Hint = hnt;
	}

  size_t PutInFile( FILE *file )
  {
    if (file == nullptr)
      return 0;
		fwrite(&Hint, sizeof(WORD), 1, file);
    fwrite(Name, sizeof(char), size, file);
		char zero = 0;
		fwrite(&zero, sizeof(zero), 1, file);
    return size;
  }
};
//////////////////////////////////////////////////////////////
///Import table
class imprt_tbl
{
private:
  size_t size;
  IMAGE_IMPORT_DESCRIPTOR *table;
  import_name *name_imp;
	size_t names_size;
  IMAGE_THUNK_DATA64 *thunk;
	size_t align_size;

public:
  imprt_tbl( size_t sz = 5 ) : size(sz + 1), name_imp(nullptr), names_size(0), align_size(0)
  {
    table    = (IMAGE_IMPORT_DESCRIPTOR *)calloc(sz + 1, sizeof(IMAGE_IMPORT_DESCRIPTOR));
		name_imp = (import_name *)            calloc(sz, sizeof(import_name));
    thunk    = (IMAGE_THUNK_DATA64 *)       calloc(sz, sizeof(thunk[0]));
  }

  bool FillDefault( void )
  {
    FillDefNames();
		size_t tbl_size = sizeof(table[1]) * size;
		size_t thunks_size = sizeof(thunk[1]) * (size - 1);
		size_t names_start = IMPORT_START + tbl_size + thunks_size;
		size_t fst_thunk_start = names_start + names_size;
		size_t name_rva = fst_thunk_start + thunks_size;
		size_t cur_name_size = 0;

		for (size_t i = 0; i < size - 1; ++i)
		{
		  table[i].OriginalFirstThunk = IMPORT_START + tbl_size + i * sizeof(thunk[1]);
      table[i].FirstThunk = fst_thunk_start + i * sizeof(thunk[1]);
		  table[i].Name = name_rva;
			thunk[i].u1.AddressOfData = (names_start + cur_name_size);
			cur_name_size += name_imp[i].size + 1 + 2;
		}
    return true;
  }

	void FillDefNames( void )
	{
		name_imp[0].Fill("ExitProcess", 11);
		name_imp[1].Fill("WriteConsoleA", 13);
		name_imp[2].Fill("ReadConsoleA", 12);
		name_imp[3].Fill("ToDec", 5);
		name_imp[4].Fill("ad6scanf", 8);

		for (size_t i = 0; i < size - 1; ++i)
			names_size += name_imp[i].size + 1 + 2;
	}

	bool PutInFile( FILE *file ) //DEfault
	{
		if (file == nullptr)
			return false;

		const size_t dll_name_size = 11;
		const size_t stub_size = 1 + SIZE_RAW - (sizeof(table[0]) * size + 2 * sizeof(thunk[1]) * (size - 1) + names_size + dll_name_size);
		char *stub = new char[stub_size]{0};

		fwrite(table, sizeof(table[0]), size, file);

		fwrite(thunk, sizeof(thunk[0]), size - 1, file);

		for (size_t i = 0; i < size - 1; ++i)
			name_imp[i].PutInFile(file);

		fwrite(thunk, sizeof(thunk[0]), size - 1, file);

		fprintf(file, "ad6lib.dll\0");

		fwrite(stub, sizeof(stub[0]), stub_size, file);

		delete[] stub;
		return true;
	}

	unsigned GetProcAddr( size_t num )
	{
		return sizeof(table[0]) * size + sizeof(thunk[0]) * (size - 1) + names_size + num * sizeof(thunk[0]);
	}

  ~imprt_tbl( void )
  {
    if (table != nullptr)
    {
      free(table);
      table = nullptr;
    }
    if (name_imp != nullptr)
    {
      free(name_imp);
      name_imp = nullptr;
    }
    if (thunk != nullptr)
    {
      free(thunk);
      thunk = nullptr;
    }
    size = 0;
  }
};
//////////////////////////////////////////////////////////////
extern imprt_tbl IMPORT_TABLE;


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