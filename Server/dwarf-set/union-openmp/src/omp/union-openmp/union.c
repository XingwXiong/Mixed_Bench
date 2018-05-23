/******************************************************************
 *       File Name: union.c
 *     Description: 
 *          Author: He Xiwen
 *           Email: hexiwen2000@163.com 
 *    Created Time: 2015-11-24 14:42:31
 *   Modified Time: 
 *          License: 
 *****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "union.h"


static void one_word( const char *str, int i, int limit, int *start, int *end );

void break_to_words( MyString *ms, MyStringArr *words ) {
  MyString one;
  int start, end;
  int i = 0;
  int limit = strlen( ms->data );

  conMyString( &one );

  while( 1 )
  {
    one_word( ms->data, i, limit, &start, &end );
    if( start == end )
    {
      // empty;
      jump++;
      break;
    }
    i = end;
    cpynStrMyString( &one, ms->data+start, end-start );
#ifdef DEBUG_TEST
    //printf( "word: \n  " );
    //displayMyString( &one );
#endif
    pushMyStringArr( words, &one );
  }

  desMyString( &one );
}

void union_to_msa( MyStringArr *u, MyStringArr *words ) {
  int i;
  for( i=0; i<words->size; ++i ) {
    if( findMyStringArr( u, &(words->arr[i]) ) == u->size )
    {
      // new word
      pushMyStringArr( u, &(words->arr[i]) );
    }
  }
}


static void one_word( const char *str, int i, int limit, int *start, int *end )
{
  for( ; str[i]!=0 && i<limit; ++i )
  {
    if( ! is_blank( str[i] ) ) {jump++;break;}
  }
  *start = i;
  for( ; str[i]!=0 && i<limit; ++i )
  {
    if( is_letter( str[i] ) ) {jump++;continue;}
    else if( is_blank( str[i] ) ) {jump++;break;}
    else
    {
      if( i == *start ) ++i;
      jump++;
      break;
    }
  }
  *end = i;
}


