/*****************************************************************
 *       File Name: my_Struct.c
 *     Description: to implement functions in my_Struct.h
 *          Author: He Xiwen
 *           Email: hexiwen2000@163.com 
 *    Created Time: 2016-01-04 21:25:11
 *   Modified Time: 2016-01-04 21:25:11
 *         License: 
 ****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#include "my_struct_arr.h"


static int expandMyStructArr( MyStructArr *s1 );
static int cmpMyStructForQsort( const void *p1, const void *p2 );


int conMyStructArr( MyStructArr *msa )
{
  uint32_t i;
  msa->size = 0; // msa: empty; need to conMyStruct();
  msa->capacity = 4;
  msa->arr = (MyStruct *)malloc( msa->capacity*sizeof(MyStruct) );
  if( msa->arr == NULL ) return -1;
  for( i=0; i<msa->capacity; ++i )
  {
    conMyStruct( msa->arr+i );
  }
  return 0;
}


int desMyStructArr( MyStructArr *msa )
{
  uint32_t i;
  //MyStruct *ms;
  if( msa->arr != NULL )
  {
    for( i=0; i<msa->capacity; ++i )
    {
      desMyStruct( msa->arr+i );
    }
    free( msa->arr );
  }
  msa->arr = NULL;
  msa->size = msa->capacity = 0;
  return 0;
}


int cpyMyStructArr( MyStructArr *s1, MyStructArr *s2 )
{
  int res;
  uint32_t i;
  if( s1->capacity <= s2->size+1 )
  { // reallocate the space.
    res = recapacityMyStructArr( s1, s2->size+2 );
    if( res < 0 ) return res;
    //desMyStructArr( s1 );
    s1->size = s2->size;
    //s1->capacity = s2->size+2;
    //s1->arr = (MyStruct *)malloc( s1->capacity*sizeof(MyStruct) );
    //if( s1->arr == NULL ) return -1; 
  }
  else
  {
    s1->size = s2->size;
  }
  for( i=0; i<s1->size; ++i )
  {
    //s1->arr[i] = s2->arr[i];
    cpyMyStruct( s1->arr+i, s2->arr+i );
  }
  return 0;
}


int clearMyStructArr( MyStructArr *msa )
{
  msa->size = 0;
  // not reallocate the space;
  //msa->capacity = 4;
  //if( msa->arr != NULL ) free( msa->arr );
  //msa->arr = (MyStruct*)malloc( sizeof(MyStruct)*4 );
  //if( msa->arr == NULL ) return -1;
  return 0;
}


MyStruct* getMyStructArr( MyStructArr *s1, uint32_t loc )
{
  if( loc >= s1->size ) return NULL;
  return &(s1->arr[loc]);
}


int pushMyStructArr( MyStructArr *s1, MyStruct *ms )
{
  expandMyStructArr( s1 );
  //return ( s1->arr[s1->size ++] = ms );
  return ( cpyMyStruct( s1->arr+( s1->size ++ ), ms ) );
}


int insertFirstMyStructArr( MyStructArr *s1, MyStruct *ms )
{
  uint32_t i;
  expandMyStructArr( s1 );
  if( s1->size > 0 )
  {
    for( i=s1->size; i>0; --i )
    {
      //s1->arr[i+1] = s1->arr[i];
      cpyMyStruct( s1->arr+i, s1->arr+(i-1) ); // actually it can be implement more efficient, just move pointer of MyStruct::data.
      // efficient way: (s1->arr+(i+1))->data=(s1->arr+i)->data; (s1->arr+(i+1))->capacity=(s1->arr+i)->capacity;
    }
    ++ s1->size;
  }
  //return ( s1->arr[0]=ms );
  return ( cpyMyStruct( s1->arr+0, ms ) );
}


int popMyStructArr( MyStructArr *s1 )
{
  if( s1->size <= 0 )return 0;
  -- s1->size;
  return 0;
  //return ( s1->arr[-- s1->size] );
}


int deleteFirstMyStructArr( MyStructArr *s1 )
{
  //MyStruct *ms;
  uint32_t i;
  if( s1->size <= 0 ) return 0;
  //ms = s1->arr+0;
  for( i=1; i<s1->size; ++i )
  {
    //s1->arr[i-1] = s1->arr[i];
    cpyMyStruct( s1->arr+(i-1), s1->arr+i ); // efficient way: just like what in insertFirstMyStructArr().
  }
  -- s1->size;
  //return ms;
  return 0;
}


static int expandMyStructArr( MyStructArr *s1 )
{
  uint32_t i;
  MyStruct *tmp;
  uint32_t size;
  uint32_t cap;

  if( s1->size <= s1->capacity-2 ) return 0;
  // need to expand. capacity=capacity*2;
  tmp = (MyStruct *)malloc( s1->capacity*2*sizeof(MyStruct) );
  if( tmp==NULL ) return -1;
  for( i=0; i<(s1->capacity*2); ++i )
  {
    conMyStruct( tmp+i );
  }
  for( i=0; i<s1->size; ++i )
  {
      //tmp[i] = s1->arr[i];
      cpyMyStruct( tmp+i, s1->arr+i );
  }
  size = s1->size;
  cap = s1->capacity;
  desMyStructArr( s1 );
  //free( s1->arr );
  s1->arr = tmp;
  s1->capacity = cap * 2;
  s1->size = size;
  return 0;
}


int recapacityMyStructArr( MyStructArr *s1, uint32_t cap )
{
  uint32_t i;
  MyStruct *tmp;
  uint32_t size;

  if( cap<s1->size || cap<4 ) return -1;
  // recapacity; // maybe realloc() is better;
  tmp = (MyStruct *)malloc( cap*sizeof(MyStruct) ); 
  if( tmp==NULL ) return -1;
  for( i=0; i<cap; ++i )
  {
    conMyStruct( tmp+i );
  }
  for( i=0; i<s1->size; ++i )
  {
      //tmp[i] = s1->arr[i];
      cpyMyStruct( tmp+i, s1->arr+i );
  }
  size = s1->size;
  //free( s1->arr );
  desMyStructArr( s1 );
  s1->arr = tmp;
  s1->capacity = cap;
  s1->size = size;
  return 0;
}


uint32_t findMyStructArr( MyStructArr *s1, MyStruct *ms ) // first one found;
{
  uint32_t i;
  for( i=0; i<s1->size; ++i )
  {
    //if( s1->arr[i] == ms ) return i;
    if( cmpMyStruct( s1->arr+i, ms ) == 0 ) return i;
  }
  return s1->size;
}


static int cmpMyStructForQsort( const void *p1, const void *p2 )
{
  return cmpMyStruct( (MyStruct *)p1, (MyStruct *)p2 );
}


int sortMyStructArr( MyStructArr *msa ) // sort; ascending order;
{
  // sort; stdlib.h::qsort();
  qsort( msa->arr, msa->size, sizeof( MyStruct ), cmpMyStructForQsort );
  return 0;
}


#ifdef DEBUG_TEST
int displayMyStructArr( MyStructArr *s1 )
{
  uint32_t i;
  if( s1==NULL ) return -1;
  printf( "MyStructArr->display(): \n" );
  printf( "size: %d, capacity: %d. \n", s1->size, s1->capacity );
  if( s1->size>0 ) // s1!=0
  {
      // must int i, not unsigned i; // for( i=s1->size-1; i>=0; --i ) //printf( "%8x ", s1->arr[i] );
      for( i=0; i<s1->size; ++i )
      {
        printf( "  " );
        displayMyStruct( s1->arr+i );
      }
  }
  //else printf( "" );
  printf( "Display end. \n" );
  return 0;
}
#endif


int displayToFileMyStructArr( FILE *ofp, MyStructArr *s1 )
{
  uint32_t i;
  if( s1==NULL ) return -1;
  fprintf( ofp, "MyStructArr->display(): \n" );
  fprintf( ofp, "size: %d, capacity: %d. \n", s1->size, s1->capacity );
  if( s1->size>0 ) // s1!=0
  {
      // must int i, not unsigned i; // for( i=s1->size-1; i>=0; --i ) //printf( "%8x ", s1->arr[i] );
      for( i=0; i<s1->size; ++i )
      {
        fprintf( ofp, "  " );
        displayToFileMyStruct( ofp, s1->arr+i );
      }
  }
  //else printf( "" );
  fprintf( ofp, "Display end. \n" );
  return 0;
}


