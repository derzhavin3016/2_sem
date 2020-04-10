// ReSharper disable All
#include "Lst.h"

lst_data POI_VALUE = {0};

base_list lsts[349900];

/**
 * \brief List init function.
 * \param this_   pointer to list.
 * \return 1 if all is ok, 0 otherwise.
 */
int LstInit( list *this_ )
{
  assert(this_ != nullptr);

  this_->Head = nullptr;
  this_->Tail = nullptr;
  this_->size = 0;
  return 1;
} /* End of 'LstInit' function */

/**
 * \brief Add element to the end of the list function.
 * \param this_   pointer to list.
 * \param Data    data to add.
 * \return 1 if all is ok, 0 otherwise.
 */
bool LstPushBack( list *this_, const lst_data *Data )
{
  assert(this_ != nullptr);
  
  static int call_cnt = 0;

  if (this_->Head == nullptr)
  {
    this_->Head = lsts + call_cnt++;
    if (this_->Head == nullptr)
      return false;
    this_->Head->Next = nullptr;
     this_->Head->Data = Data;
    this_->size++;
    this_->Tail = this_->Head;
    return true;      
  }
  
  //BasePushBack(this_->Tail, Data);
  
  this_->Tail->Next = lsts + call_cnt++;

  this_->Tail = this_->Tail->Next;
  this_->Tail->Data = Data;
  this_->Tail->Next = nullptr;

  this_->size++;
  return true;
} /* End of 'LstInit' function */

#if 1
/**
 * \brief Find element in list function.
 * \param [in, out] this_   pointer to list structure.
 * \param [in]      Data    Data to find.
 * \param [in]      cmp     pointer to compare function
 * \return 1 if element was find, 0 otherwise.
 */
int LstFind( list *this_, const lst_data *Data, int (*cmp)( const void *a, const void *b ) )
{
  assert(this_ != nullptr);

  if (this_->Head == nullptr)
    return 0;

  base_list *lst = this_->Head;

  for (size_t i = 0; i < this_->size; ++i)
  {
    if (cmp(lst->Data, Data) == 0)
      return 1;
    lst = lst->Next;
  }

  return 0;
} /* End of 'LstFind' function */
#endif
/**
 * \brief Find element in base list function.
 * \param [in, out] this_   pointer to base_list structure.
 * \param [in]      Data    Data to find.
 * \param [in]      cmp     pointer to compare function
 * \return 1 if element was find, 0 otherwise.
 */
int BaseFind( base_list *this_, const lst_data *Data, int (*cmp)( const void *a, const void *b ))
{
  assert(this_ != nullptr);

  while (this_ != nullptr)
  {
    if (cmp(this_->Data, Data) == 0)
      return 1;
    this_ = this_->Next;
  }

  return 0;
} /* End of 'BaseFind' function */

/**
 * \brief Add element to the end of the list function.
 * \param this_   pointer to base.
 * \param Data    data to add.
 * \return 1 if all is ok, 0 otherwise.
 */
int BasePushBack( base_list *this_, const lst_data *Data )
{
  assert(this_ != nullptr);
  
  base_list **Start = &this_;

  while ((*Start)->Next != nullptr)
    Start = &(*Start)->Next;
  
  (*Start)->Next = (base_list *)calloc(1, sizeof(base_list));
  if ((*Start)->Next == nullptr)
    return 0;
  
  (*Start)->Next->Next = nullptr;
  (*Start)->Next->Data = Data;  
  
  return 1;
} /* End of 'BasePushBack' function */

/**
 * \brief List delete function.
 * \param this_   pointer to list.
 * \return 1 if all is ok, 0 otherwise.
 */
int LstDel( list *this_ )
{
  assert(this_ != nullptr);
  
  this_->size = 0;
  this_->Tail = nullptr;
  this_->Head = nullptr;

  //if (this_->Head != nullptr)
    //BaseDel(&this_->Head);

  return 1;
} /* End of 'LstDel' function */ 

/**
 * \brief Delete base_list function.
 * \param this_   pointer to list.
 * \return 1 if all is ok, 0 otherwise.
 */
int BaseDel( base_list **this_ )
{
  assert(this_ != nullptr);

  base_list *prev = nullptr;

  while ((*this_)->Next != nullptr)
  {
    prev = *this_;
    *this_ = (*this_)->Next;
    free(prev);
  }

  free(*this_);
  *this_ = nullptr;
  return 1;
} /* End of 'BaseInit' function */

