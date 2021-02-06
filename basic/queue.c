#include <stdio.h> 
#include "queue.h"

int32_t      queue_enqueue  (Queue_T *queue, void *data)
{

    return slist_ins_next(queue,slist_tail(queue),data);

}
int32_t      queue_dequeue  (Queue_T *queue, void **data )
{
        return slist_rem_next(queue,NULL,data);

}


