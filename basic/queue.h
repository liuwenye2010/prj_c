
#ifndef _QUEUE_H_INCLUDED_
#define _QUEUE_H_INCLUDED_

#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>
#include <stdlib.h> 
#include "slist.h"

typedef  Slist_T  Queue_T ;


#define       queue_init slist_init
#define       queue_destory  slist_destory
int32_t      queue_enqueue  (Queue_T *queue, void *data); 
int32_t      queue_dequeue  (Queue_T *queue, void **data ); 



#define  queue_peek  ((queue)->head == NULL ? NULL :(queue)->head->data)
#define  queue_size slist_size 



#ifdef __cplusplus
}
#endif

#endif 
