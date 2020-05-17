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

  for (size_t PC = 0; PC < in.size - 8;)
  {
#define DEF_CMD(name, cde, action, table_act)       \
    case cde:                                       \
       ++PC;                                        \
      {action;}                                     \
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
  char *ptr = in.buf + 8;

  for (size_t PC = 0; PC < in.size - 8;)
  {
#define DEF_CMD(nm, cde, act, tbl_act)            \
    case cde:                                     \
      ++PC;                                       \
      {tbl_act;}                                  \
      break;
    
    switch (ptr[PC])
    {

      #include "../commands.h"

    default:
      printf("Incorrect opcode : %d\nPosition %zd\n", ptr[PC], PC);
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

bool jmp_table::PushOldInf( char *ptr, int dest_addr )
{
  assert(size < JMP_SIZE);

  table[size].old_ptr = ptr;
  table[size++].old_jmp = dest_addr;

  return true;
}

int jmp_table::FindByDest( int old_jmp_addr, int new_jmp_addr )
{
  // check all table, because one place in code can be
  // destination for several jmps

  for (size_t i = 0; i < size; ++i)
    // check if the jmp destianations in adasm are equal
    if (table[i].old_jmp == old_jmp_addr)
    {
      // check if the address of jmp in x86 code is valid
      if (table[i].new_ptr != nullptr)
        // insert in the address of jump it's destination address
        *((int *)(table[i].new_ptr + 1)) = new_jmp_addr;
      table[i].new_jmp = new_jmp_addr;
    }

  return -1;
}

/**
 * \brief Processing jump function
 * \param [in] jmp_ptr  - pointer to jmp command in adasm code
 * \param [in, out] new_ptr - pointer to jmp command in x86 code
 */
int jmp_table::JmpProcess( char *jmp_ptr, char *new_ptr )
{
  for (size_t i = 0; i < size; ++i)
    if (table[i].old_ptr == jmp_ptr)
    {
      table[i].new_ptr = new_ptr;
      *((int *)new_ptr + 1) = table[i].new_jmp;
      return (int)i;
    }
  return -1;
} /* End of 'JmpProcess' function */

// END OF 'BIN_TR.CPP' FILE