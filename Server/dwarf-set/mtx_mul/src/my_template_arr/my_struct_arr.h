/******************************************************************
 *       File Name: my_struct_arr.h
 *     Description: my_struct_arr;
 *          Author: He Xiwen
 *           Email: hexiwen2000@163.com 
 *    Created Time: 2016-01-04 21:25:11
 *   Modified Time: 2016-01-04 21:25:11
 *         License: 
 *****************************************************************/

// not thread safe; 
// bacause sometimes there is malloc() and free() calls in con(), des(), cpy(), append(), recapacity(), push(), pop(), insert(), delete(), sort(), so these funcs are strongly suggested not to be called in multi-thread funcs, unless you can make sure malloc() and free() will not be called in these funcs;
// maybe someday, i need to change all malloc() and free() in .c file to the sequential memory-management ops in my_seq_ops.h;


#ifndef MY_STRUCT_ARR_H
#define MY_STRUCT_ARR_H

#include <stdint.h>
//#include "type.h"

#include "my_struct.h"

// all MyStructArrr::arr[i] ( 0<=i<capacity ) is already conMyStruct();
typedef struct S_MyStructArr // array
{
  MyStruct *arr;
  uint32_t capacity;
  uint32_t size;
} MyStructArr;


extern int conMyStructArr( MyStructArr *msa );
extern int desMyStructArr( MyStructArr *msa );
extern int cpyMyStructArr( MyStructArr *msa1, MyStructArr *msa2 ); // *msa1 = *msa2;
extern int clearMyStructArr( MyStructArr *msa );
extern MyStruct* getMyStructArr( MyStructArr *msa1, uint32_t loc );
extern int pushMyStructArr( MyStructArr *msa1, MyStruct *ms );
extern int popMyStructArr( MyStructArr *msa1 );
extern int insertFirstMyStructArr( MyStructArr *msa1, MyStruct *ms );
extern int deleteFirstMyStructArr( MyStructArr *msa1 );
extern int recapacityMyStructArr( MyStructArr *msa1, uint32_t cap );
extern uint32_t findMyStructArr( MyStructArr *msa1, MyStruct *ms ); // first one found;
extern int sortMyStructArr( MyStructArr *msa ); // sort; ascending order;
#ifdef DEBUG_TEST
extern int displayMyStructArr( MyStructArr *msa );
#endif
extern int displayToFileMyStructArr( FILE *ofp, MyStructArr *msa );


#endif /* MY_STRUCT_ARR_H */
