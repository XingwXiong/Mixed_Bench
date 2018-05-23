/******************************************************************
 *       File Name: do_one_file.c
 *     Description: 
 *          Author: He Xiwen
 *           Email: hexiwen2000@163.com 
 *    Created Time: 2015-11-23 17:27:24
 *   Modified Time: 
 *          License: 
 *****************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#ifdef _OPENMP
#include <omp.h>
#else
#include "my_single_omp.h"
#endif

#include "my_seq_ops.h"
#include "my_string.h"
#include "my_data.h"
#include "union.h"
#include "do_one_file.h"


//static size_t single_read_block( FILE *ifp, size_t read_block_size, int linecnt_max, char *line, MyStringArr *msa, MyString *ms, int *lineno );
static size_t single_read_lines( FILE *ifp, int linecnt, char *line, MyStringArr *msa, MyString *ms, int *lineno );
static size_t single_write_block( FILE *ofp, MyStringArr *msa );
static void union_two_line( MyString *ms_1, MyString *ms_2, MyStringArr *u );
static void union_one_line( MyString *ms, MyStringArr *u );
static void for_parallel_two( MyString *ms_1, MyString *ms_2 );
static void for_parallel_one( MyString *ms );
static size_t read_one_line_to_msa( FILE *ifp, char *line, MyStringArr *msa, MyString *ms, int *line_no );
static size_t read_one_line( FILE *ifp, char *line, MyString *ms, int *line_no );

void do_one_file( const char *ifpath_1, const char *ifpath_2, const char *ofpath, char *line, MyStringArr *msa_1, MyStringArr *msa_2, MyString *ms )
{
  int i;
  FILE *ifp_1;
  FILE *ifp_2;
  FILE *ofp;
  size_t one_readed_size;
  size_t readed_size_1;
  size_t readed_size_2;
  size_t writed_size;
  int lineno_1;
  int lineno_2;
#ifdef DEBUG_TEST
  int lineno_old_1;
#endif
  //int lineno_old_2;
  int lcnt;
  MyStringArr *msa;

#ifdef DEBUG_TEST
  printf( "now to file: %s\n", ifpath_1 );
#endif

  ifp_1 = fopen(ifpath_1, "r");
  if (ifp_1 == NULL) {
    perror("Cannot open input file");
    jump++;
    exit(EXIT_FAILURE);
  }
  ifp_2 = fopen(ifpath_2, "r");
  if (ifp_2 == NULL) {
    perror("Cannot open input file");
    jump++;
    exit(EXIT_FAILURE);
  }
  ofp = fopen(ofpath, "w");
  if (ofp == NULL) {
    perror("Cannot open output file");
    jump++;
    exit(EXIT_FAILURE);
  }
  readed_size_1 = 0;
  lineno_1 = 0;
  readed_size_2 = 0;
  lineno_2 = 0;
  writed_size = 0;

  while( (!feof( ifp_1 )) || (!feof( ifp_2 )) ){
    logic++;
#ifdef DEBUG_TEST
    lineno_old_1 = lineno_1;
#endif
    one_readed_size = single_read_lines( ifp_1, LINE_CNT_MAX, line, msa_1, ms, &lineno_1 );
    readed_size_1 += one_readed_size;
#ifdef DEBUG_TEST
    printf( "one block:  size:%ld  line:%d\n", one_readed_size, lineno_1-lineno_old_1 );
#endif

    //lineno_old_2 = lineno_2;
    one_readed_size = single_read_lines( ifp_2, LINE_CNT_MAX, line, msa_2, ms, &lineno_2 );
    readed_size_2 += one_readed_size;

    lcnt = msa_1->size > msa_2->size ? msa_2->size : msa_1->size;
#ifdef _OPENMP
#pragma omp parallel for
#endif
    for( i=0; i<lcnt; ++i ){
#ifdef DEBUG_TEST
      //if( 0==i ) printf( "thread_cnt: %d\n", omp_get_num_threads() );
#endif
      // msa_1[i] = msa_1[i] union msa_2[i]
      for_parallel_two( &(msa_1->arr[i]), &(msa_2->arr[i]) );
    }

    lcnt = msa_1->size > msa_2->size ? msa_1->size-msa_2->size : msa_2->size-msa_1->size;
    msa = msa_1->size > msa_2->size ? msa_1 : msa_2;
    if( lcnt > 0 ){
#ifdef _OPENMP
#pragma omp parallel for
#endif
      for( i=0; i<lcnt; ++i ){
#ifdef DEBUG_TEST
        //if( 0==i ) printf( "thread_cnt: %d\n", omp_get_num_threads() );
#endif
        // msa_1[i] = msa_1[i] union msa_2[i]
        for_parallel_one( &(msa->arr[msa->size-lcnt+i]) );
      }
    }

    if( msa_1->size < msa_2->size ) { // need to move msa_2[size-lcnt...size-1] to msa_1[size...size+lcnt-1];
      if( msa_1->capacity < msa_2->capacity ) {
        recapacityMyStringArr( msa_1, msa_2->capacity );
      }
      for( i=0; i<lcnt; ++i ) {
        cpyMyString( msa_1->arr+(msa_1->size+i), msa_2->arr+(msa_2->size-lcnt+i) );
      }
      msa_1->size = msa_2->size;
    }

    writed_size += single_write_block( ofp, msa_1 );
  }

  fclose(ifp_2);
  fclose(ifp_1);
  fclose(ofp);
}

/*
static size_t single_read_block( FILE *ifp, size_t read_block_size, int linecnt_max, char *line, MyStringArr *msa, MyString *ms, int *lineno ){
  size_t readed_size;
  int linecnt;
  int ret = 0;

  readed_size = 0;
  linecnt = 0;
  clearMyStringArr( msa );
  //clearMyString( ms );

  while (1) {
    if( feof( ifp ) ) break;
    if( readed_size > read_block_size ) break;
    if( linecnt > linecnt_max ) break;
    ret = read_one_line_to_msa( ifp, line, msa, ms, lineno );
    if( ret < 0 ) break; // eof;
    readed_size += ret;
    ++linecnt;
  }
  return readed_size;
}
*/

// read excat linecnt lines if possible; finally read msa->size lines;
static size_t single_read_lines( FILE *ifp, int linecnt, char *line, MyStringArr *msa, MyString *ms, int *lineno ){
  size_t readed_size;
  int lcnt;
  int ret = 0;

  readed_size = 0;
  lcnt = 0;
  clearMyStringArr( msa );
  //clearMyString( ms );

  while (1) {
    if( feof( ifp ) ) break;
    //if( readed_size > read_block_size ) break;
    if( lcnt >= linecnt ) break;
    ret = read_one_line_to_msa( ifp, line, msa, ms, lineno );
    if( ret < 0 ) break; // eof;
    readed_size += ret;
    ++lcnt;
  }
  jump++;
  return readed_size;
}


static size_t single_write_block( FILE *ofp, MyStringArr *msa ){
  int i;
  char *buf;
  size_t writed_size = 0;
  for( i=0; i<msa->size; ++i ){
    buf = (msa->arr[i]).data;
    fprintf( ofp, "%s\n", buf );
    writed_size += strlen( buf ) + 1;
  }
  jump++;
  return writed_size;
}


static void union_two_line( MyString *ms_1, MyString *ms_2, MyStringArr *u ) {
  int len;
  int i;
  char *str, *one;
  MyStringArr msa_words;

  conMyStringArr( &msa_words );
  clearMyStringArr( u );

  // to words and union;
  break_to_words( ms_1, &msa_words );
  union_to_msa( u, &msa_words );

  clearMyStringArr( &msa_words );
  break_to_words( ms_2, &msa_words );
  union_to_msa( u, &msa_words );

  // words to string;
  clearMyString( ms_1 );
  len = 0;
  for( i=0; i<u->size; ++i ) {
    len += strlen( (u->arr[i]).data );
  }
  len += u->size +2;
  if( ms_1->capacity < len ) recapacityMyString( ms_1, len );
  str = ms_1->data;
  for( i=0; i<u->size; ++i ) {
    one = (u->arr[i]).data;
    len = strlen( one );
    strcpy( str, one );
    str += len;
    *str = ' ';
    *(++str) = 0;
  }
  *(--str) = 0;

  desMyStringArr( &msa_words );
}


static void union_one_line( MyString *ms, MyStringArr *u ){
  int len;
  int i;
  char *str, *one;
  MyStringArr msa_words;

  conMyStringArr( &msa_words );
  clearMyStringArr( u );

  // to words and union;
  break_to_words( ms, &msa_words );
  union_to_msa( u, &msa_words );

  // words to string;
  clearMyString( ms );
  len = 0;
  for( i=0; i<u->size; ++i ) {
    len += strlen( (u->arr[i]).data );
  }
  len += u->size +2;
  if( ms->capacity < len ) recapacityMyString( ms, len );
  str = ms->data;
  for( i=0; i<u->size; ++i ) {
    one = (u->arr[i]).data;
    len = strlen( one );
    strcpy( str, one );
    str += len;
    *str = ' ';
    *(++str) = 0;
  }
  *(--str) = 0;

  desMyStringArr( &msa_words );
}


static void for_parallel_two( MyString *ms_1, MyString *ms_2 ) {
  MyStringArr u;

  conMyStringArr( &u );

  union_two_line( ms_1, ms_2, &u );

  desMyStringArr( &u );
}


static void for_parallel_one( MyString *ms ){
  MyStringArr u;

  conMyStringArr( &u );

  union_one_line( ms, &u );

  desMyStringArr( &u );
}


static size_t read_one_line_to_msa( FILE *ifp, char *line, MyStringArr *msa, MyString *ms, int *line_no )
{
  size_t ret = 0;
  int line_no_old = *line_no;

  if( feof( ifp ) ) {jump++;return -1;} // eof;
  ret = read_one_line( ifp, line, ms, line_no );
  if( line_no_old == *line_no ) {jump++;return -1;} // eof;
  pushMyStringArr( msa, ms );
  //clearMyString( ms );
  jump++;
  return ret;
}


static size_t read_one_line( FILE *ifp, char *line, MyString *ms, int *line_no ) // ret: line len; ret!=0 <=> ms->data[0]!=0;
{
  char *s_res;
  const int line_size = MY_LINE_MAX + 1;
  int lline;
  size_t ret;

  clearMyString( ms );
  ret = 0;
  while( 1 )
  {
    s_res = fgets( line, line_size, ifp );
    if( s_res == NULL )
    {
      if( ms->data[0] != 0 )
      {
        // line in ms; // no eol for last line in file;
        printf( "no EOL for the last line in file!" );
        ++ (*line_no);
      }
      jump++;
      break; // eof;
    }
    lline = strlen( line );
    ret += lline;
    if( line[lline-1] == '\n' )
    {
      // eol;
      ++ (*line_no);
      if( (*line_no) % 10000 == 0 ) printf( "reading: line_no:%d\n", *line_no );
      appendnMyString( ms, line, lline-1 );
      jump++;
      break;
    }
    else
    {
      appendnMyString( ms, line, lline );
    }
  }
  jump++;
  return ret;
}

