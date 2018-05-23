/******************************************************************
 *       File Name: my_struct.c
 *     Description: 
 *          Author: He Xiwen
 *           Email: hexiwen2000@163.com 
 *    Created Time: 2016-01-04 21:25:20
 *   Modified Time: 2016-01-04 21:25:11
 *         License: 
 *****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#include "my_struct.h"

int conMyStruct( MyStruct *ms )
{
  ms->col_cnt = 0;
  ms->col_ind = NULL;
  ms->val = NULL;
  return 0;
}


int desMyStruct( MyStruct *ms )
{
  if( ms->col_ind != NULL ) free( ms->col_ind );
  ms->col_ind = NULL;
  if( ms->val != NULL ) free( ms->val );
  ms->val = NULL;
  ms->col_cnt = 0;
  return 0;
}


int cpyMyStruct( MyStruct *ms1, MyStruct *ms2 ) // *ms1 = *ms2;
{
  //int i;

  if( ms1->col_cnt < ms2->col_cnt )
  { // reallocate the space.
    desMyStruct( ms1 );
    ms1->col_cnt = ms2->col_cnt;
    ms1->col_ind = (int *) malloc( ms1->col_cnt * sizeof(int) );
    ms1->val = (double *) malloc( ms1->col_cnt * sizeof(double) );
    if( ms1->col_ind==NULL || ms1->val==NULL ) return -1; 
  }
  else
  {
    // nothing
  }
  memcpy( ms1->col_ind, ms2->col_ind, ms1->col_cnt*sizeof(int) );
  memcpy( ms1->val, ms2->val, ms1->col_cnt*sizeof(double) );
  return 0;
}


int clearMyStruct( MyStruct *ms )
{
  //ms->col_cnt = 0;
  return 0;
}


int cmpMyStruct( const MyStruct *ms1, const MyStruct *ms2 ) // compare;
{
  return ms1->col_cnt < ms2->col_cnt ? -1 : ( ms1->col_cnt > ms2->col_cnt ? 1 : 0 );
}


#ifdef DEBUG_TEST
int displayMyStruct( MyStruct *ms )
{
  if( ms == NULL ) return -1;
  //printf( "MyStruct: capacity:%d data:%s\n", ms->capacity, ms->data );
  displayToFileMyStruct( stdout, ms );
  return 0;
}
#endif


int displayToFileMyStruct( FILE *ofp, MyStruct *ms )
{
  int i;
  if( ms == NULL ) return -1;
  //fprintf( ofp, "MyStruct: capacity:%d data:%s\n", ms->capacity, ms->data );
  fprintf( ofp, "%d\n", ms->col_cnt );
  for( i=0; i<ms->col_cnt; ++i ) fprintf( ofp, "%d ", ms->col_ind[i] );
  fprintf( ofp, "\n" );
  for( i=0; i<ms->col_cnt; ++i ) fprintf( ofp, "%lf ", ms->val[i] );
  fprintf( ofp, "\n" );
  return 0;
}


