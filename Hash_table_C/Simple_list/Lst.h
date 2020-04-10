#ifndef __LST_H_
#define __LST_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../Strings/strings.h"

typedef String lst_data;

extern lst_data POI_VALUE;

typedef struct TAGbase_lst base_list;

typedef struct TAGbase_lst
{
  const lst_data *Data;
  base_list *Next;
} base_list;

int BaseDel( base_list **this_ );

int BaseFind( base_list *this_, const lst_data *Data, int (*cmp)( const void *a, const void *b ) );

void BasePrint( base_list *this_ );

int BasePushBack( base_list *this_, const lst_data *Data );

typedef struct TAGlist
{
  base_list *Head, *Tail;
  size_t size;
} list;

int LstInit( list *this_ );

bool LstPushBack( list *this_, const lst_data *Data );

int LstFind( list *this_, const lst_data *Data, int (*cmp)( const void *a, const void *b ) );

int LstDel( list *this_ );


#endif /* __LST_H_ */
