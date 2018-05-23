/******************************************************************
 *       File Name: fft.c
 *     Description: 
 *          Author: He Xiwen
 *           Email: hexiwen2000@163.com 
 *    Created Time: 2016-01-06 21:47:31
 *   Modified Time: 
 *         License: 
 *****************************************************************/

#include <fftw3.h>

#include "fft.h"


void fft_init()
{
#ifdef _OPENMP
  fftwf_init_threads();
#endif
}


fftwf_complex *readToMem( FILE *ifp, int *row, int *col, int *nz )
{
  int i, j;
  fftwf_complex *data;
  int r, c, n;
  int x, y;
  double val;

  rewind( ifp );
  fscanf( ifp, "%d,%d,%d", &r, &c, &n );
  data = (fftwf_complex *) fftwf_malloc( sizeof(fftwf_complex) * r * c );
  // init;
  for( i=0; i<r; ++i ) {
    for( j=0; j<c; ++j ) {
      data[i*c+j][0] = 0;
      data[i*c+j][1] = 0;
    }
  }
  for( i=0; i<n; ++i ) {
    fscanf( ifp, "%d,%d,%lf", &x, &y, &val );
    data[x*c+y][0] = val;
    data[x*c+y][1] = 0;
  }

  *row = r;
  *col = c;
  *nz = n;
  return data;
}


void single_write( FILE *ofp, fftwf_complex *data, int row, int col )
{
/*
  int i, j;
  // too slow...
  fprintf( ofp, "%d,%d\n", row, col );
  for( i=0; i<row; ++i ) {
    for( j=0; j<col; ++j ) {
      fprintf( ofp, "%lf %lf ", data[i*col+j][0], data[i*col+j][1] );
    }
    fprintf( ofp, "\n" );
  }
*/

  // not readable...
  fwrite( data, sizeof(fftwf_complex)*row*col, 1, ofp );

}


