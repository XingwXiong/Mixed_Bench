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
//#include "my_data.h"
#include "do_one_file.h"


int main(int argc, const char *argv[])
{
  int i;
  struct stat st;
  const char *input_f_1;
  const char *input_f_2;
  const char *output_f;
  int thread_cnt;
  time_t time_start, time_end;
  clock_t clock_start, clock_end;

  time_start = time(NULL);
  clock_start = clock();
  clock_single_read = 0;
  clock_single_write = 0;
  clock_parallel = 0;
  walltime_parallel = 0;
  my_seq_ops_init();
  //my_data_init();

#ifdef _OPENMP
  // usage: exe input_f_1 input_f_2 output_f thread_cnt
  if (argc != 5)
#else
  // usage: exe input_f_1 input_f_2 output_f
  if (argc != 4)
#endif
  {
    if(argc != 1) fprintf(stderr, "Wrong arguments!\n");
#ifdef _OPENMP
    printf("Usage: %s input_f_1 input_f_2 output_f thread_cnt\n", argv[0]);
#else
    printf("Usage: %s input_f_1 input_f_2 output_f\n", argv[0]);
#endif
    if( argc==1 ) exit(EXIT_SUCCESS);
    else exit( EXIT_FAILURE );
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
    if( 0==i ) printf( "multi-thread: thread_cnt: %d\n", omp_get_num_threads() );
  }
#else
  thread_cnt = omp_get_num_threads();
  printf( "single-thread: thread_cnt: %d\n", thread_cnt );
#endif

  input_f_1 = argv[1];
  input_f_2 = argv[2];
  output_f = argv[3];

  if (stat(input_f_1, &st) < 0) {
    // not exist;
    perror("Cannot access input_file");
    exit(EXIT_FAILURE);
  }
  if (S_ISDIR(st.st_mode)) {
    fprintf( stderr, "Error: Cannot process dir now...\n" );
    exit(EXIT_FAILURE);
    // input_f: input_d
    printf("process dir.\n");
  } else if (S_ISREG(st.st_mode)) {
    // input_f: input_f
    printf("process file.\n");

    do_one_file( input_f_1, input_f_2, output_f );
  }

  //my_data_end();
  my_seq_ops_end();

  time_end = time(NULL);
  clock_end = clock();
  printf( "time: %ld\n", time_end-time_start );
  printf( "clock: %lf\n", (clock_end-clock_start)/(1.0*CLOCKS_PER_SEC) );
  printf( "clock_single_read: %lf\n", (clock_single_read)/(1.0*CLOCKS_PER_SEC) );
  printf( "clock_single_write: %lf\n", (clock_single_write)/(1.0*CLOCKS_PER_SEC) );
  printf( "clock: parallel part:  total of threads:%lf  wall time:%lf\n", (clock_parallel)/(1.0*CLOCKS_PER_SEC), walltime_parallel );
printf("matrixMult finish\n");
  return 0;
}

