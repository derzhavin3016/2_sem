#include "BIN_TR.h"


////// JUMP TABLE ////////////////////////
jmp_table TABLE;
//////////////////////////////////////////
////// IMPORT TABLE //////////////////////
imprt_tbl IMPORT_TABLE;
//////////////////////////////////////////

// default constructor
buffer::buffer( void ) : buf(nullptr),
                         size(0)
{
}

buffer::buffer( size_t size, char init /* = 0 */ ) : buf(new char[size]{init}),
                                                      size(size)
{
}



buffer::buffer( buffer&& bf ) : buf(bf.buf),
                                size(bf.size)
{
  bf.buf = nullptr;
}

/**
 * \brief Fill buffer from file function
 * \param [in] filename Name of a file to load from
 */
void buffer::Fill( const char filename[] )
{
  this->buf = FillBuf(filename, &this->size);
} /* End of 'Fill' function */

bool buffer::Put( const char filename[] )
{
  return PutBufToFile(filename, buf, size);
}

char & buffer::operator [] ( size_t index )
{
  return buf[index];
}


/**
 * \brief Translate code from one buffer and put to another
 * \param [in]  in      reference to input buffer(source)
 * \param [out] out     reference to output buffer(destination)
 */
bool Translate( const buffer &in, buffer &out )
{
  const char *code_ptr = (char *)((int *)in.buf + 2);
  char *buf_out = out.buf;


  if (*((int *)in.buf) != ad6::SIGNATURE)
    return printf("Incorrect signature: %d\n", *((int *)in.buf)), false;

  FillJmpTable(in);

  bool IsJmp = false;
  for (size_t PC = 0; PC < in.size - 8;)
  {
#define DEF_CMD(name, cde, action, table_act)                                   \
    case cde:                                                                   \
      IsJmp = (cde == 40 || (cde >= 20 && cde <= 26));                          \
      TABLE.CmdProcess(const_cast<char *>(&code_ptr[PC]), buf_out, IsJmp);      \
      ++PC;                                                                     \
      {action;}                                                                 \
      break;

    switch (code_ptr[PC])
    {

      #include "../commands.h"


    default:
      printf("Invalid opcode: %d\nPosition %zd\n", (int) code_ptr[PC], PC);
      return false;
    }
  }

#undef DEF_CMD
  return true;
} /* End of 'Translate' function */

/**
 * \brief Fill jump table from input buffer function
 * \param [in]  in      reference to input buffer(source)
 */
void FillJmpTable( const buffer &in )
{
  char *code_ptr = in.buf;

  for (size_t PC = 8; PC < in.size;)
  {
#define DEF_CMD(nm, cde, act, tbl_act)            \
    case cde:                                     \
      ++PC;                                       \
      {tbl_act;}                                  \
      break;
    
    switch (code_ptr[PC])
    {

      #include "../commands.h"

    default:
      printf("Incorrect opcode : %d\nPosition %zd\n", code_ptr[PC], PC);
      return;

    }

#undef DEF_CMD
  }


} /* End of 'FillJmpTable' function */


buffer::~buffer( void )
{
  if (buf != nullptr)
    delete[] buf;
}


jmp_table::jmp_table( void ) : table{0},
                               size(0)
{
}

bool jmp_table::PushOldInf( char *adasm_ptr, char *adasm_dest )
{
  assert(size < JMP_SIZE);

  table[size].adasm_ptr = adasm_ptr;
  table[size++].adasm_dest = adasm_dest;

  return true;
}

/**
 * \brief Processing command function
 * \param [in]      adasm_addr  - pointer to command in adasm code
 * \param [in, out] x86_addr    - pointer to command in x86 code
 * \param [in]      IsJmp       - flag value, true when command is jmp-type, false otherwise
 */
bool jmp_table::CmdProcess( char *adasm_addr, char *x86_addr, bool IsJmp /* = false */ )
{
  // check all table, because one place in code can be
  // destination for several jmps

  for (size_t i = 0; i < size; ++i)
    // check if the jmp destianations in adasm are equal
    if (table[i].adasm_dest == adasm_addr)
    {
      // check if the address of jmp in x86 code is valid
      if (table[i].x86_ptr != nullptr)
      {
        int rel_x86_dest = static_cast<int>(x86_addr - table[i].x86_ptr - sizeof(int) - sizeof(char));
        // insert in the address of jump it's destination address
        *(reinterpret_cast<int *>(table[i].x86_ptr + 1)) = rel_x86_dest;
      }
      table[i].x86_dest = x86_addr;
    }
    else if (IsJmp && table[i].adasm_ptr == adasm_addr)
    {
      char jmp_code = *(table[i].adasm_ptr);
      char *x86_cond_addr = x86_addr;

      if (jmp_code != 40 && jmp_code != 20)
        // size of introduction commands for cond. jumps
        x86_cond_addr += 5; // pop, pop, cmp, half of cond jmp code

      table[i].x86_ptr = x86_cond_addr;
      *reinterpret_cast<int *>(x86_cond_addr + 1) = static_cast<int>(table[i].x86_dest - table[i].x86_ptr - sizeof(int) - sizeof(char));
      // because one jump can be in jmp table only one time as jmp
      IsJmp = false;
    }


  return true;
} /* End of 'CmdProcess' function */

/**
 * \brief Processing jump function
 * \param [in] adasm_ptr  - pointer to jmp command in adasm code
 * \param [in, out] x86_ptr - pointer to jmp command in x86 code
 */
int jmp_table::JmpProcess( char *adasm_ptr, char *x86_ptr )
{
  for (size_t i = 0; i < size; ++i)
    if (table[i].adasm_dest == adasm_ptr)
    {
    }
    else if (table[i].adasm_ptr == adasm_ptr)
    {
      table[i].x86_ptr = x86_ptr;
      *reinterpret_cast<int *>(x86_ptr + 1) = static_cast<int>(table[i].x86_dest - table[i].x86_ptr - sizeof(int) - sizeof(char));
      return (int)i;
    }
  return -1;
} /* End of 'JmpProcess' function */

/**
 * \brief Create an .exe file function.
 * \param [in] filename   Name of an .exe file
 * \param [in] out        Reference to buffer structure with x86 code.
 * \return true if all is ok, false otherwise
 **/
bool CreateExe( const char filename[], const buffer &out )
{
  const size_t stub_size = SIZE_RAW;
  char stub[stub_size] = {0};
  FILE *exe = fopen(filename, "wb");
  size_t vrt_addr = ENTRY_POINT_ADDR,
         vrt_size = VRT_SIZE,
         size_raw = SIZE_RAW,
         ptr_raw  = 0x400;


  if (exe == nullptr)
    return printf("Problem with openning %s file", filename), true;

  fwrite(&DOS_HEADER,        sizeof(DOS_HEADER), 1, exe);
  fwrite(&DOS_STUB,          sizeof(DOS_STUB[0]), sizeof(DOS_STUB)/sizeof(DOS_STUB[0]), exe);
  fwrite(&CreateNTHeader(3), sizeof(IMAGE_NT_HEADERS), 1, exe);

  fwrite(&CreateSecHeader(".text", vrt_size, vrt_addr, size_raw, ptr_raw, CODE_CHAR), 
         sizeof(IMAGE_SECTION_HEADER), 1, exe);
  fwrite(&CreateSecHeader(".idata", vrt_size, IMPORT_START, size_raw, ptr_raw + size_raw, IDATA_CHAR), 
         sizeof(IMAGE_SECTION_HEADER), 1, exe);
  fwrite(&CreateSecHeader(".data", vrt_size, DATA_START, size_raw, ptr_raw + 2 * size_raw, DATA_CHAR), 
         sizeof(IMAGE_SECTION_HEADER), 1, exe);

  fwrite(stub, sizeof(stub[0]), 30 * 16, exe);

  fwrite(out.buf, sizeof(out.buf[0]), out.size, exe);

  IMPORT_TABLE.PutInFile(exe);
  
  fwrite(stub, sizeof(stub[0]), stub_size, exe);

  fclose(exe);
  return true; 
} /* End of 'CreateExe' function */

IMAGE_FILE_HEADER CreateFileHeader( int NUM_OF_SEC )
{
  IMAGE_FILE_HEADER FILE_HDR =
  {
    .Machine = IMAGE_FILE_MACHINE_I386,
    .NumberOfSections     = static_cast< WORD>(NUM_OF_SEC),
    .TimeDateStamp        = static_cast<DWORD>(time(nullptr)),
    .SizeOfOptionalHeader = sizeof(IMAGE_OPTIONAL_HEADER32),
    .Characteristics = IMAGE_FILE_EXECUTABLE_IMAGE | IMAGE_FILE_32BIT_MACHINE
  };
  return FILE_HDR;
}

IMAGE_OPTIONAL_HEADER CreateOptHeader( void )
{
  IMAGE_OPTIONAL_HEADER OPT_HDR =
  {
    .Magic = 0x10b,
    .AddressOfEntryPoint = ENTRY_POINT_ADDR,
    .ImageBase = IMAGE_BASE,
    .SectionAlignment = ENTRY_POINT_ADDR,
    .FileAlignment = 0x200,
    .MajorSubsystemVersion = 4,
    .SizeOfImage = 2 * 0x8000,
    .SizeOfHeaders = 0x400,
    .Subsystem = IMAGE_SUBSYSTEM_WINDOWS_CUI,
    .NumberOfRvaAndSizes = 0x10,
  };
  OPT_HDR.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress = IMPORT_START;

  return OPT_HDR;
}

IMAGE_SECTION_HEADER CreateSecHeader( const char sec_name[], size_t vrt_size, 
                                      size_t vrt_addr,       size_t size_raw_data, 
                                      size_t ptr_raw_data,   size_t charac )
{
  IMAGE_SECTION_HEADER SEC_HDR =
  {
    .VirtualAddress   = vrt_addr,
    .SizeOfRawData    = size_raw_data,
    .PointerToRawData = ptr_raw_data,
    .Characteristics  = charac
  };

  sprintf_s((char * const)SEC_HDR.Name, (size_t)IMAGE_SIZEOF_SHORT_NAME, "%s", sec_name); 
  SEC_HDR.Misc.VirtualSize = vrt_size;

  return SEC_HDR;
}

IMAGE_NT_HEADERS CreateNTHeader( int NUM_OF_SEC )
{
  IMAGE_NT_HEADERS PE_HEADER =
  {
    .Signature      = '\0EP\0',
    .FileHeader     = CreateFileHeader(NUM_OF_SEC),
    .OptionalHeader = CreateOptHeader()
  };
  return PE_HEADER;
}

// END OF 'BIN_TR.CPP' FILE