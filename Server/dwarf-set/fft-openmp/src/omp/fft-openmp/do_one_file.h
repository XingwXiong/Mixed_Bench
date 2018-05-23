/******************************************************************
 *       File Name: do_one_file.h
 *     Description: 
 *          Author: He Xiwen
 *           Email: hexiwen2000@163.com 
 *    Created Time: 2015-11-23 17:27:19
 *   Modified Time: 
 *          License: 
 *****************************************************************/

#ifndef _DO_ONE_FILE_H_
#define _DO_ONE_FILE_H_

#include <time.h>

extern clock_t clock_single_read;
extern clock_t clock_single_write;
extern clock_t clock_parallel;
extern double walltime_parallel;


void do_one_file( const char *ifpath, const char *ofpath );

#endif /* _DO_ONE_FILE_H_ */
