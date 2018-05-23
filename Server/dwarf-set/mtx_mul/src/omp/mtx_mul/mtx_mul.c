/******************************************************************
 *       File Name: mtx_mul.c
 *     Description: 
 *          Author: He Xiwen
 *           Email: hexiwen2000@163.com 
 *    Created Time: 2015-12-23 19:39:12
 *   Modified Time: 
 *         License: 
 *****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _OPENMP
#include <omp.h>
#else
#include "my_single_omp.h"
#endif

#include "my_seq_ops.h"
#include "my_string.h"
#include "my_string.h"
#include "my_struct.h"
#include "my_struct_arr.h"
#include "mtx_mul.h"


#define MY_LINE_MAX 1024

clock_t clock_single_read;
clock_t clock_single_write;
clock_t clock_parallel;
double walltime_parallel;


// sort ind( row or col ), change val's position too.
static void ind_sort( int *ind, double *val, int cnt );
static int one_row_mm( int row_nr, MtxCrs *crs, MtxCcs *ccs, int *col_cnt, int *col_ind, double *val );
static void one_cpy( MyStruct *ms, int col_cnt, int *col_ind, double *val );


MtxCrs *readToCrs( FILE *ifp ) // malloc a MtxCrs, return it;
{
  int i;
  MtxCrs *ret;
  int row, col, nz;
  int x, y;
  double val;
  int *row_cnt;

  fscanf( ifp, "%d,%d,%d", &row, &col, &nz );
  ret = (MtxCrs *) my_malloc( sizeof(MtxCrs) * 1 );
  ret->row = row;
  ret->col = col;
  ret->nz = nz;
  ret->val = (double *) my_malloc( sizeof(double) * nz );
  ret->col_ind = (int *) my_malloc( sizeof(int) * nz );
  ret->row_ptr = (int *) my_malloc( sizeof(int) * (row+1) );
  ret->row_ptr[0] = 0;
  row_cnt = (int *) my_malloc( sizeof(int) * (row+1) );

  // con row_ptr; just counting;
  memset( row_cnt, 0, sizeof(int)*(row+1) );
  for( i=0; i<nz; ++i ) {
    fscanf( ifp, "%d,%d,%lf", &x, &y, &val );
    ++ row_cnt[x+1];
  }
  for( i=0; i<row; ++i ) {
    ret->row_ptr[i+1] = ret->row_ptr[i] + row_cnt[i+1];
    row_cnt[i+1] = ret->row_ptr[i+1];
  }

  // read data to val and col_ind;
  // count in row_cnt;
  rewind( ifp );
  fscanf( ifp, "%d,%d,%d", &row, &col, &nz ); // head;
  for( i=0; i<nz; ++i ) {
    fscanf( ifp, "%d,%d,%lf", &x, &y, &val );
    ret->val[ row_cnt[x] ] = val;
    ret->col_ind[ row_cnt[x] ] = y;
    ++ row_cnt[x];
  }

  // sort each row by col_ind;
  for( i=0; i<row; ++i ) {
    // row i;
    ind_sort( ret->col_ind+ ret->row_ptr[i], ret->val+ ret->row_ptr[i], ret->row_ptr[i+1]-ret->row_ptr[i] );
  }

  // done;
  free( row_cnt );
  return ret;
}


void destroyCrs( MtxCrs *crs ) // free;
{
  my_free( crs->val );
  my_free( crs->col_ind );
  my_free( crs->row_ptr );
  my_free( crs );
}


void displayCrs( MtxCrs *crs )
{
  int i, j;
  int from, end;
  printf( "%d,%d,%d\n", crs->row, crs->col, crs->nz );
  for( i=0; i<crs->row; ++i ) {
    from = crs->row_ptr[i];
    end = crs->row_ptr[i+1];
    for( j=from; j<end; ++j ) {
      printf( "%d,%d,%lf\n", i, crs->col_ind[j], crs->val[j] );
    }
  }
}


MtxCcs *readToCcs( FILE *ifp ) // malloc a MtxCcs, return it;
{
  int i;
  MtxCcs *ret;
  int row, col, nz;
  int x, y;
  double val;
  int *col_cnt;

  fscanf( ifp, "%d,%d,%d", &row, &col, &nz );
  ret = (MtxCcs *) my_malloc( sizeof(MtxCcs) * 1 );
  ret->row = row;
  ret->col = col;
  ret->nz = nz;
  ret->val = (double *) my_malloc( sizeof(double) * nz );
  ret->row_ind = (int *) my_malloc( sizeof(int) * nz );
  ret->col_ptr = (int *) my_malloc( sizeof(int) * (col+1) );
  ret->col_ptr[0] = 0;
  col_cnt = (int *) my_malloc( sizeof(int) * (col+1) );

  // con col_ptr; just counting;
  memset( col_cnt, 0, sizeof(int)*(col+1) );
  for( i=0; i<nz; ++i ) {
    fscanf( ifp, "%d,%d,%lf", &x, &y, &val );
    ++ col_cnt[y+1];
  }
  for( i=0; i<col; ++i ) {
    ret->col_ptr[i+1] = ret->col_ptr[i] + col_cnt[i+1];
    col_cnt[i+1] = ret->col_ptr[i+1];
  }

  // read data to val and row_ind;
  // count in col_cnt;
  rewind( ifp );
  fscanf( ifp, "%d,%d,%d", &row, &col, &nz ); // head;
  for( i=0; i<nz; ++i ) {
    fscanf( ifp, "%d,%d,%lf", &x, &y, &val );
    ret->val[ col_cnt[y] ] = val;
    ret->row_ind[ col_cnt[y] ] = x;
    ++ col_cnt[y];
  }

  // sort each col by row_ind;
  for( i=0; i<col; ++i ) {
    // col i;
    ind_sort( ret->row_ind+ ret->col_ptr[i], ret->val+ ret->col_ptr[i], ret->col_ptr[i+1]-ret->col_ptr[i] );
  }

  // done;
  free( col_cnt );
  return ret;
}


void destroyCcs( MtxCcs *ccs ) // free;
{
  my_free( ccs->val );
  my_free( ccs->row_ind );
  my_free( ccs->col_ptr );
  my_free( ccs );
}


void displayCcs( MtxCcs *ccs )
{
  int i, j;
  int from, end;
  printf( "%d,%d,%d\n", ccs->row, ccs->col, ccs->nz );
  for( i=0; i<ccs->col; ++i ) {
    from = ccs->col_ptr[i];
    end = ccs->col_ptr[i+1];
    for( j=from; j<end; ++j ) {
      printf( "%d,%d,%lf\n", ccs->row_ind[j], i, ccs->val[j] );
    }
  }
}


void spa_mtx_mul_mtx_to_file( FILE *ofp, MtxCrs *crs, MtxCcs *ccs )
{
  int i, j;
  int o_row, o_col, o_nz;
  clock_t c_s, c_e;
  struct timespec ts_b, ts_e;
  int thread_cnt;
  int thread_n;
  int *col_cnt; // size: thread_cnt;
  int *col_ind; // size: thread_cnt*col;
  double *val; // size: thread_cnt*col;
  MyStruct ms;
  MyStructArr msa;

  conMyStruct( &ms );
  conMyStructArr( &msa );
  o_row = crs->row;
  o_col = ccs->col;
  o_nz = 0;

#ifdef _OPENMP
#pragma omp parallel for
#endif
  for( i=0; i<2; ++i ) {
    if(i==0) thread_cnt = omp_get_num_threads();
  }
  col_cnt = (int *) malloc( thread_cnt * sizeof(int) );
  for( i=0; i<thread_cnt; ++i ) {
    col_cnt[i] = -1;
  }
  col_ind = (int *) malloc( thread_cnt * o_col * sizeof(int) );
  val = (double *) malloc( thread_cnt * o_col * sizeof(double) );
  recapacityMyStructArr( &msa, o_row );
  msa.size = o_row;

  c_s = clock();
  clock_gettime( CLOCK_MONOTONIC, &ts_b );
#ifdef _OPENMP
#pragma omp parallel for private(thread_n)
#endif
  for( i=0; i<o_row; ++i ) {
#ifdef DEBUG_TEST
    //if( i == 0 ) printf( "thread_cnt: %d\n", omp_get_num_threads() );
#endif
    thread_n = omp_get_thread_num();
    //one_row_mm( i, crs, ccs, &(col_cnt[thread_n]), &(col_ind[thread_n][0]), &(val[thread_n][0]) );
    one_row_mm( i, crs, ccs, &(col_cnt[thread_n]), &(col_ind[thread_n*o_col+0]), &(val[thread_n*o_col+0]) );
    one_cpy( getMyStructArr( &msa, i ), col_cnt[thread_n], col_ind+thread_n*o_col, val+thread_n*o_col );
  }
  c_e = clock();
  clock_gettime( CLOCK_MONOTONIC, &ts_e );
  clock_parallel += c_e - c_s;
  walltime_parallel += (ts_e.tv_sec - ts_b.tv_sec) + (ts_e.tv_nsec - ts_b.tv_nsec)/(1000000000.0);
  // single write.
  //displayToFileMyStructArr( ofp, &msa );
  // coo format;
  c_s = clock();
  o_nz = 0;
  for( i=0; i<o_row; ++i ) {
    o_nz += msa.arr[i].col_cnt;
  }
  fprintf( ofp, "%d,%d,%d\n", o_row, o_col, o_nz );
  for( i=0; i<o_row; ++i ) {
    col_ind = msa.arr[i].col_ind;
    val = msa.arr[i].val;
    for( j=0; j<msa.arr[i].col_cnt; ++j ) {
      fprintf( ofp, "%d,%d,%lf\n", i, col_ind[j], val[j] );
    }
  }
  c_e = clock();
  clock_single_write += c_e - c_s;

  desMyStruct( &ms );
  desMyStructArr( &msa );
}


static void one_cpy( MyStruct *ms, int col_cnt, int *col_ind, double *val )
{
  desMyStruct( ms );
  ms->col_cnt = col_cnt;
  ms->col_ind = (int *) malloc( col_cnt * sizeof(int) );
  ms->val = (double *) malloc( col_cnt * sizeof(double) );
  memcpy( ms->col_ind, col_ind, col_cnt*sizeof(int) );
  memcpy( ms->val, val, col_cnt*sizeof(double) );
}


static int one_row_mm( int row_nr, MtxCrs *crs, MtxCcs *ccs, int *col_cnt, int *col_ind, double *val )
{
  int i, j;
  int k_i, k_j;
  int from_i, from_j, end_i, end_j;
  int o_col;
  double one_val;
  int cnt;

  cnt = 0;
  i = row_nr;
  o_col = ccs->col;
  from_i = crs->row_ptr[i];
  end_i = crs->row_ptr[i+1];
  for( j=0; j<o_col; ++j ) {
    from_j = ccs->col_ptr[j];
    end_j = ccs->col_ptr[j+1];
    // A[i][] * B[][j]
    // merge crs->(val[from_i...end_i], col_ind[from_i...end_i])
    // with  ccs->(val[from_j...end_j], row_ind[from_j...end_j])
    one_val = 0;
    for( k_i=from_i, k_j=from_j; k_i<end_i && k_j<end_j; ) {
      if( crs->col_ind[k_i] < ccs->row_ind[k_j] ) ++k_i;
      else if( crs->col_ind[k_i] == ccs->row_ind[k_j] ) {
        //printf( "%d %d %lf  %d %d %lf %lf\n", i, crs->col_ind[k_i], crs->val[k_i], ccs->row_ind[k_j], j, ccs->val[k_j], crs->val[k_i]*ccs->val[k_j] );
        one_val += crs->val[k_i] * ccs->val[k_j];
        ++k_i,++k_j;
      }
      else if( crs->col_ind[k_i] > ccs->row_ind[k_j] ) ++k_j;
    }
    //if( one_val != 0 ) fprintf( ofp, "%d,%d,%lf\n", i, j, one_val );
    if( one_val != 0 ) {
      col_ind[ cnt ] = j;
      val[ cnt ] = one_val;
      ++ (cnt);
    }
  }
  *col_cnt = cnt;
  //fprintf( ofp, "\n" );
  return cnt;
}


static void ind_sort( int *ind, double *val, int cnt )
{
  // cnt is always very small, most of time less than 100, beacuse the matrix is sparse matrix;
  // so, insert sort is ok.
  int i, j;
  int ind_x;
  double val_x;
  for( i=1; i<cnt; ++i ) {
    ind_x = ind[i];
    val_x = val[i];
    for( j=i-1; j>=0; --j ) {
      if( ind_x >= ind[j] ) break;
      ind[j+1] = ind[j];
      val[j+1] = val[j];
    }
    ind[j+1] = ind_x;
    val[j+1] = val_x;
  }
}


