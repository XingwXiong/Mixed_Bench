/******************************************************************
 *       File Name: my_struct.h
 *     Description: my_struct template;
 *          Author: He Xiwen
 *           Email: hexiwen2000@163.com 
 *    Created Time: 2016-01-04 21:25:11
 *   Modified Time: 2016-01-04 21:25:11
 *         License: 
 *****************************************************************/

// not thread safe; 
// bacause sometimes there is malloc() and free() calls in con(), des(), cpy(), append(), recapacity(), push(), pop(), insert(), delete(), sort(), so these funcs are strongly suggested not to be called in multi-thread funcs, unless you can make sure malloc() and free() will not be called in these funcs;
// maybe someday, i need to change all malloc() and free() in .c file to the sequential memory-management ops in my_seq_ops.h;


#ifndef _MY_STRUCT_H_
#define _MY_STRUCT_H_ 1

#include <stdint.h>
//#include "type.h"

typedef struct S_MyStruct
{
  int col_cnt;
  int *col_ind;
  double *val;
} MyStruct;


extern int conMyStruct( MyStruct *ms );
extern int desMyStruct( MyStruct *ms );
extern int cpyMyStruct( MyStruct *ms1, MyStruct *ms2 ); // *ms1 = *ms2;
extern int clearMyStruct( MyStruct *ms );
extern int cmpMyStruct( const MyStruct *ms1, const MyStruct *ms2 ); // compare;
#ifdef DEBUG_TEST
extern int displayMyStruct( MyStruct *ms );
#endif
extern int displayToFileMyStruct( FILE *ofp, MyStruct *ms );


#endif /* _MY_STRUCT_H_ */
