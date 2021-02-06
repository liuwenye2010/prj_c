
#ifndef _STACK_H_INCLUDED_
#define _STACK_H_INCLUDED_

#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>
#include <stdlib.h>
#include "slist.h"

typedef  Slist_T  Stack_T ;

#define  stack_init     slist_init
#define  stack_destory  slist_destory
int32_t      stack_push (Stack_T *stack, const void* data);
int32_t      stack_pop  (Stack_T *stack, void** data);

#define  stack_peek  ((stack)->head == NULL ? NULL :(stack)->head->data)
#define  stack_size list_size



#ifdef __cplusplus
}
#endif

#endif
