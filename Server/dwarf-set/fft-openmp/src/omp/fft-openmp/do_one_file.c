/******************************************************************
 *       File Name: do_one_file.c
 *     Description: 
 *          Author: He Xiwen
 *           Email: hexiwen2000@163.com 
 *    Created Time: 2015-12-23 19:39:20
 *   Modified Time: 
 *         License: 
 *****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#ifdef _OPENMP
#include <omp.h>
#else
#include "my_single_omp.h"
#endif

#include "my_seq_ops.h"
#include "fft.h"
#include "do_one_file.h"


clock_t clock_single_read;
clock_t clock_single_write;
clock_t clock_parallel;
double walltime_parallel;


void do_one_file( const char *ifpath, const char *ofpath )
{
  int i, j;
  fftwf_plan plan;
  int row, col, nz;
  fftwf_complex *data; // matrix;
  double nml;
  FILE *ifp, *ofp;
  //size_t readed_size;
  //int lineno;
  //size_t writed_size;
  clock_t c_s, c_e;
  struct timespec ts_b, ts_e;

  //readed_size = 0;
  //lineno = 0;
  //writed_size = 0;

  // single read;
  c_s = clock();
  ifp = fopen(ifpath, "r");
  if (ifp == NULL) {
    perror("Cannot open input file");
    exit(EXIT_FAILURE);
  }
  data = readToMem( ifp, &row, &col, &nz );
  fclose( ifp );
#ifdef DEBUG_TEST
  //displayCrs( ma );
#endif

  // fftw plan
#ifdef DEBUG_TEST
  printf( "fftw threads: %d\n", omp_get_max_threads() );
#endif
#ifdef _OPENMP
  fftwf_plan_with_nthreads( omp_get_max_threads() );
#endif
  plan = fftwf_plan_dft_2d( row, col, data, data, FFTW_FORWARD, FFTW_ESTIMATE );
  plan = fftwf_plan_dft_2d( row, col, data, data, FFTW_FORWARD, FFTW_ESTIMATE );
  plan = fftwf_plan_dft_2d( row, col, data, data, FFTW_FORWARD, FFTW_ESTIMATE );
  
  c_e = clock();
  clock_single_read += c_e - c_s;

  c_s = clock();
  clock_gettime( CLOCK_MONOTONIC, &ts_b );
  // parallel part;
  fftwf_execute( plan );
  c_e = clock();
  clock_gettime( CLOCK_MONOTONIC, &ts_e );
  clock_parallel += c_e - c_s;
  walltime_parallel += (ts_e.tv_sec - ts_b.tv_sec) + (ts_e.tv_nsec - ts_b.tv_nsec)/(1000000000.0);

  c_s = clock();
  // single write;
  ofp = fopen(ofpath, "w");
  if (ofp == NULL) {
    perror("Cannot open output file");
    exit(EXIT_FAILURE);
  }
  // normalize data;
  nml = sqrt( (double) row * col );
  for( i=0; i<row; ++i ) {
    for( j=0; j<col; ++j ) {
      data[i*col+j][0] /= nml;
      data[i*col+j][1] /= nml;
    }
  }
//  single_write( ofp, data, row, col );
  fclose( ofp );
  c_e = clock();
  clock_single_write += c_e - c_s;

  fftwf_destroy_plan( plan );
  fftwf_free( data );
}


