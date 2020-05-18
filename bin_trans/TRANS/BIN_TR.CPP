#include "BIN_TR.h"


////// JUMP TABLE ////////////////////////
jmp_table TABLE;
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
  char *code_ptr = in.buf + 8;

  for (size_t PC = 0; PC < in.size - 8;)
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
      break;

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
      table[i].x86_ptr = x86_addr;
      *reinterpret_cast<int *>(x86_addr + 1) = static_cast<int>(table[i].x86_dest - table[i].x86_ptr - sizeof(int) - sizeof(char));
      return (int)i;
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

// END OF 'BIN_TR.CPP' FILE