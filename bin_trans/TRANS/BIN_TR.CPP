#include "BIN_TR.h"

// default constructor
buffer::buffer( void ) : buf(nullptr),
                         size(0)
{
}

buffer::buffer( size_t size ) : buf(new uchar[size]{0}),
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

uchar & buffer::operator [] ( size_t index )
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
  const uchar *code_ptr = (uchar *)((int *)in.buf + 2);

  if (*((int *)in.buf) != ad6::SIGNATURE)
  {
    printf("Incorrect signature: %d\n", *((int *)in.buf));
    return false;
  }
  for (size_t PC = 0; PC < in.size - 8; ++PC)
  {
#define DEF_CMD(name, code, action)      \
    {                                    \
    case code:                           \
      action;                            \
      break;                             \
    }

    switch (code_ptr[PC])
    {

    default:
      printf("Invalid opcode: %u\nPosition %zd", (int) code_ptr[PC], PC);
      return false;
    }
  }

#undef DEF_CMD
  return true;
} /* End of 'Translate' function */


buffer::~buffer( void )
{
  if (buf != nullptr)
    delete[] buf;
}


// END OF 'BIN_TR.CPP' FILE