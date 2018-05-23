/******************************************************************
 *       File Name: main.c
 *     Description: 
 *          Author: He Xiwen
 *           Email: hexiwen2000@163.com 
 *    Created Time: 2015-11-17 15:19:05
 *   Modified Time: 
 *          License: 
 *****************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

#ifdef _OPENMP
#include <omp.h>
#else
#include "my_single_omp.h"
#endif

#include "my_seq_ops.h"
#include "my_string.h"
#include "my_data.h"
#include "do_one_file.h"
#include "union.h"

int main(int argc, const char *argv[])
{
  int i;
  DIR *dir;
  struct dirent *fi;
  struct stat st;
  const char *input_f_d_1;
  const char *input_f_d_2;
  const char *output_f_d;
  int ilen_1, ilen_2, olen;
  char *ifpath_1;
  char *ifpath_2;
  char *ofpath;
  char *line;
  int thread_cnt;
  MyString ms;
  MyStringArr msa_1;
  MyStringArr msa_2;
  time_t time_start, time_end;

  time_start = time( NULL );
  my_seq_ops_init();

#ifdef _OPENMP
  // usage: exe input_f_d_1 input_f_d_2 output_f_d thread_cnt
  if (argc != 5)
#else
  // usage: exe input_f_d_1 input_f_d_2 output_f_d
  if (argc != 4)
#endif
  {
    if(argc != 1) fprintf(stderr, "Wrong arguments!\n");
#ifdef _OPENMP
    printf("Usage: %s input_f_d_1 input_f_d_2 output_f_d thread_cnt\n", argv[0]);
#else
    printf("Usage: %s input_f_d_1 input_f_d_2 output_f_d\n", argv[0]);
#endif
    if( argc==1 ) {jump++;exit(EXIT_SUCCESS);}
    else {jump++;exit( EXIT_FAILURE );}
  }
#ifdef DEBUG_TEST
  printf("argc: %d\n", argc);
  printf("argv: ");
  for (i = 0; i < argc; ++i) printf("%s ", argv[i]);
  printf("\n");
#endif

#ifdef _OPENMP
  thread_cnt = atoi( argv[4] );
  omp_set_num_threads( thread_cnt );
#pragma omp parallel for
  for( i=0; i<10; ++i ){
    if( 0==i ) printf( "thread_cnt: %d\n", omp_get_num_threads() );
  }
#else
  thread_cnt = omp_get_num_threads();
  printf( "thread_cnt: %d\n", thread_cnt );
#endif

  conMyString( &ms );
  conMyStringArr( &msa_1 );
  conMyStringArr( &msa_2 );

  input_f_d_1 = argv[1];
  input_f_d_2 = argv[2];
  output_f_d = argv[3];

  thread_cnt = omp_get_num_threads();

  line = (char *) my_malloc(sizeof(char) * (MY_LINE_MAX + 1));
  line[MY_LINE_MAX] = 0;

  ilen_1 = strlen(input_f_d_1);
  ilen_1 += MY_FILENAME_MAX + 1;
  ifpath_1 = (char *) my_malloc(sizeof(char) * ilen_1);
  ifpath_1[0] = 0;

  ilen_2 = strlen(input_f_d_2);
  ilen_2 += MY_FILENAME_MAX + 1;
  ifpath_2 = (char *) my_malloc(sizeof(char) * ilen_2);
  ifpath_2[0] = 0;

  olen = strlen(output_f_d);
  olen += MY_FILENAME_MAX + 1;
  ofpath = (char *) my_malloc(sizeof(char) * olen);
  ofpath[0] = 0;

  // stat test: only test one side( input_1, not input_2 );
  if (stat(input_f_d_1, &st) < 0) {
    // not exist;
    perror("Cannot access input_file_or_dir");
    jump++;
    exit(EXIT_FAILURE);
  }
  if (S_ISDIR(st.st_mode)) {
    // input_f_d_1: input_d
    printf("process dir.\n");

    if ((dir = opendir(input_f_d_1)) == NULL) {
      perror("Cannot open input_f_d_1");
      jump++;
      exit(EXIT_FAILURE);
    }

    while ((fi = readdir(dir)) != NULL) {
      snprintf(ifpath_1, ilen_1, "%s/%s", input_f_d_1, fi->d_name);
      snprintf(ifpath_2, ilen_2, "%s/%s", input_f_d_2, fi->d_name);
      if (fi->d_name[0] == '.') {
        // ignore: hidden file, or pwd, or parent dir of pwd;
        jump++;
        continue;
      }
      if (stat(ifpath_1, &st) < 0) {
        // not exist
        jump++;
        continue;
      }
      if (S_ISDIR(st.st_mode)) {
        // ignore: child dir
        jump++;
        continue;
      }
#ifdef DEBUG_TEST
      //printf( "ifpath_1: %s\n", ifpath_1 );
#endif
      if (S_ISREG(st.st_mode)) {
        snprintf( ofpath, olen, "%s/%s", output_f_d, fi->d_name );
        do_one_file( ifpath_1, ifpath_2, ofpath, line, &msa_1, &msa_2, &ms );
      }

    }
    closedir(dir);

  } else if (S_ISREG(st.st_mode)) {
    // input_f_d_1: input_f
    printf("process file.\n");

    snprintf( ifpath_1, ilen_1, "%s", input_f_d_1 );
    snprintf( ifpath_2, ilen_2, "%s", input_f_d_2 );
    snprintf( ofpath, olen, "%s", output_f_d );
    do_one_file( ifpath_1, ifpath_2, ofpath, line, &msa_1, &msa_2, &ms );
  }

  my_free(ifpath_1);
  my_free(ifpath_2);
  my_free(ofpath);
  my_free(line);
  desMyStringArr( &msa_2 );
  desMyStringArr( &msa_1 );
  desMyString( &ms );
  my_seq_ops_end();
  
  jump++;  

  time_end = time(NULL);
  printf( "time: %ld\n", time_end-time_start );
  printf("logic number: %d\n",logic);
  printf("jump number: %d\n",jump); 

  return 0;
}

