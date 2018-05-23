/******************************************************************
 *       File Name: mtx_mul.h
 *     Description: 
 *          Author: He Xiwen
 *           Email: hexiwen2000@163.com 
 *    Created Time: 2015-12-23 19:39:07
 *   Modified Time: 
 *         License: 
 *****************************************************************/

#ifndef _MTX_MUL_H_
#define _MTX_MUL_H_ 1


extern clock_t clock_single_read;
extern clock_t clock_single_write;
extern clock_t clock_parallel;
extern double walltime_parallel;


typedef struct S_MtxCrs
{
  int row, col, nz;
  double *val; // nz;
  int *col_ind; // nz;
  int *row_ptr; // row+1;
} MtxCrs;

typedef struct S_MtxCcs
{
  int row, col, nz;
  double *val; // nz;
  int *row_ind; // nz;
  int *col_ptr; // col+1;
} MtxCcs;


extern MtxCrs *readToCrs( FILE *ifp ); // malloc a MtxCrs, return it;
extern void destroyCrs( MtxCrs *crs ); // free;
extern void displayCrs( MtxCrs *crs ); // display;
extern MtxCcs *readToCcs( FILE *ifp ); // malloc a MtxCcs, return it;
extern void destroyCcs( MtxCcs *ccs ); // free;
extern void displayCcs( MtxCcs *ccs ); // display;

extern void spa_mtx_mul_mtx_to_file( FILE *ofp, MtxCrs *crs, MtxCcs *ccs );

#endif /* _MTX_MUL_H_ */
