#include "Hash_table.h"
#include "hash_func.h"
#include <iostream>

const int MAX_NAME = 100;

const size_t STR_SIZE = 349900;

hash_t LyHash_cpp( const String *str )
{
  hash_t hash = 0;

  for (size_t i = 0; i < str->len; i++)
    hash = (hash * 1664525) + (unsigned char)(str->str[i]) + 1013904223;
  
  return hash;
} /* End of 'LyHash' function */


//extern "C" hash_t LyHash( const String *str );

int main( void )
{

 hash_table tbl;

 TableInit(&tbl);

 tbl.strs = BUFFER;
 tbl.strs_size = STR_SIZE;

 Hashing(&tbl, LyHash_cpp);

  const String st = {(const unsigned char *) "aa", 8};
  const String st1 = {(const unsigned char *)"aa", 3};

  //int cnt = 0;

 //std::cout << StringCmp(&st, &st1);

  for (size_t i = 0; i < 100; ++i)
    for (size_t j = 0; j < STR_SIZE; ++j)
      Find(&tbl, BUFFER + j);
 

 TableClose(&tbl);
  return 0;
}