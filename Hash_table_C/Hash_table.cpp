#include "Hash_table.h"

#define TBL_LOCATION __FILE__, __LINE__, __FUNCSIG__

/**
 * \brief Insert to hash table function
 * \param [in] str    pointer to string to insert.
 * \param [in] IsDic  boolean variable shows is searching needed (true default).
 */
bool Insert( hash_table *this_, const String *str, bool IsDic /*= true*/ )
{
  hash_t h = this_->Hash_func(str);

  if (IsDic || Find(this_, str, h) == 0)
    return LstPushBack(this_->table + (h % TABLE_SIZE), str);
 
  return true;
} /* End of 'Insert' function */


/**
 * \brief Find in hash table function
 * \param [in] this_     pointer to hash_table.
 * \param [in] str      pointer to string to find.
 * \param [in] hsh       Hash of a string(if it has already calculated) (0 default).
 * \return hash of a string if string was find
 * \return 0 if string was not find.
 */
hash_t Find( hash_table *this_, const String *str, hash_t hsh /*= 0*/ )
{
 
  if (hsh == 0)
    hsh = this_->Hash_func(str);

  if (LstFind(this_->table + (hsh % TABLE_SIZE), str, StringCmp) == 0)
    return 0;
 
 
  return hsh;
} /* End of 'Find' function */

void Clear( hash_table *this_ )
{
  for (size_t i = 0; i < TABLE_SIZE; ++i)
    LstDel(this_->table + i);
}

/**
 * \brief Hash all words from buf function.
 * \param [in] this_ pointer to hash table.
 * \param [in] Hash pointer to hash function.
 * \param [in] IsDic  boolean variable shows is searching needed (true default).
 */
void Hashing( hash_table *this_, hash_t (*Hash)( const String *str ), bool IsDic /* = true*/ )
{
 
  this_->Hash_func = Hash;
  for (size_t cnt = 0; cnt < this_->strs_size; cnt++)
    Insert(this_, this_->strs + cnt, IsDic);
 
} /* End of 'Hashing' function */

void TableInit( hash_table *this_ )
{
  for (size_t i = 0; i < TABLE_SIZE; ++i)
    LstInit(this_->table + i);
  this_->buf_size = 0;
  this_->strs_size = 0;
  this_->strs = nullptr;
  this_->IsHeaderPrint = false;
}

void TableClose( hash_table *this_ )
{
  for (size_t i = 0; i < TABLE_SIZE; ++i)
    LstDel(this_->table + i);

  this_->buf_size = 0;
  this_->strs_size = 0;

  this_->IsHeaderPrint = false;
}

#undef TBL_ASSERT

#undef TBL_LOCATION