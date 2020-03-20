#ifndef __HASH_H_
#define __HASH_H_

#include "..\!FST_SEM(C++)\Processor\Strings\strings.h"
#include <stdarg.h>


// typedef of hash data type
typedef unsigned long long hash_t;

const char conf_name[] = "hash.config";

const size_t Bias = 0xCC;

/**
 * \brief FAQ6 hash. (template function)
 * \param data  - pointer to data to calculate hash.
 * \param size  - amount of elements (1 default).
 * \return Calculated hash.
 */
template <typename Data>
hash_t FAQ6Hash( const Data *data, size_t size = 1 )
{
  hash_t hash = 0;
  
  for (size_t i = 0; i < sizeof(Data) * size; i++)
  {
      hash += (unsigned char)(data[i]);
      hash += (hash << 10);
      hash ^= (hash >> 6);
  }

  hash += (hash << 3);
  hash ^= (hash >> 11);
  hash += (hash << 15);
  
  return hash;
} /* End of 'FAQ6Hash' function */


bool PatchCode( const char filename[] );

bool HashOk( const unsigned char *buf, size_t buf_length );

bool CheckCond( int condition, const char err_str[], ... );

#endif /* __HASH_H_ */


/* END OF HASH.H FILE */