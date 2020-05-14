#include "BIN_TR.h"

jmp_inf jmp_table[JMP_SIZE] = {0};

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


// END OF 'BIN_TR.CPP' FILE