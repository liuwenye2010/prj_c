#include <stdio.h> 
#include "stack.h" 


/*stack push*/ 
int32_t  stack_push (Stack_T *stack, const void* data)
{
    return slist_ins_next(stack, NULL, data); 
}


int32_t stack_pop  (Stack_T *stack, void** data)
{
    return  slist_rem_next(stack,NULL,data);

}