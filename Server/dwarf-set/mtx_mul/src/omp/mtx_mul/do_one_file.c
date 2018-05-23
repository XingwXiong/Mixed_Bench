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
#include <time.h>

#ifdef _OPENMP
#include <omp.h>
#else
#include "my_single_omp.h"
#endif

#include "my_seq_ops.h"
#include "mtx_mul.h"
#include "do_one_file.h"


void do_one_file( const char *ifpath_1, const char *ifpath_2, const char *ofpath )
{
  MtxCrs *ma;
  MtxCcs *mb;
  FILE *ifp_1, *ifp_2, *ofp;
  //size_t readed_size;
  //int lineno;
  //size_t writed_size;
  clock_t c_s, c_e;

  //readed_size = 0;
  //lineno = 0;
  //writed_size = 0;

  c_s = clock();
  ifp_1 = fopen(ifpath_1, "r");
  if (ifp_1 == NULL) {
    perror("Cannot open input file");
    exit(EXIT_FAILURE);
  }
  ma = readToCrs( ifp_1 );
  fclose( ifp_1 );
#ifdef DEBUG_TEST
  //displayCrs( ma );
#endif
  ifp_2 = fopen(ifpath_2, "r");
  if (ifp_2 == NULL) {
    perror("Cannot open input file");
    exit(EXIT_FAILURE);
  }
  mb = readToCcs( ifp_2 );
  //mb = (MtxCcs *) readToCrs( ifp_2 );
  fclose( ifp_2 );
#ifdef DEBUG_TEST
  //displayCcs( mb );
#endif
  c_e = clock();
  clock_single_read += c_e - c_s;
  ofp = fopen(ofpath, "w");
  if (ofp == NULL) {
    perror("Cannot open output file");
    exit(EXIT_FAILURE);
  }
  spa_mtx_mul_mtx_to_file( ofp, ma, mb );
  spa_mtx_mul_mtx_to_file( ofp, ma, mb );
  spa_mtx_mul_mtx_to_file( ofp, ma, mb );
  spa_mtx_mul_mtx_to_file( ofp, ma, mb );
  spa_mtx_mul_mtx_to_file( ofp, ma, mb );
  fclose( ofp );

  destroyCrs( ma );
  destroyCcs( mb );
}


