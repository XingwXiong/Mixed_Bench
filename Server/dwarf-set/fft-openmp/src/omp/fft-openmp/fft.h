/******************************************************************
 *       File Name: fft.h
 *     Description: 
 *          Author: He Xiwen
 *           Email: hexiwen2000@163.com 
 *    Created Time: 2016-01-06 21:47:28
 *   Modified Time: 
 *         License: 
 *****************************************************************/

#ifndef _FFT_H_
#define _FFT_H_ 1

#include <fftw3.h>

extern void fft_init();
extern fftwf_complex *readToMem( FILE *ifp, int *row, int *col, int *nz );
extern void single_write( FILE *ofp, fftwf_complex *data, int row, int col );


#endif /* _FFT_H_ */
