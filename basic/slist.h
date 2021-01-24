
#ifndef _SLIST_H_INCLUDED_
#define _SLIST_H_INCLUDED_

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include <stdlib.h> 

//Single Link node
typedef  struct  SlistElmt_S 
{
    void * data;        // data field
    struct  SlistElmt_S *next; // pointer filed
} SlistElmt_T;

//Single List list structures
typedef struct Slist_S
{
    int32_t size; 
    int32_t (*match)(const void* key1, const void *key2);
    void (*destory)(void* data); 
    SlistElmt_T *head;
    SlistElmt_T *tail;
}Slist_T; 

//Single List public interface 
void  slist_init ( Slist_T *list, void(*destroy)(void*data));
void  slist_destory ( Slist_T *list); 
int32_t slist_ins_next( Slist_T *list, SlistElmt_T *element, const void* data);//insert one element to the list 
int32_t slist_rem_next( Slist_T *list, SlistElmt_T *element, void** data);//remove one element to the list

//int32_t slist_size(const Slist_T *list); 
//SlistElmt_T  slist_head(const Slist_T * list);
//SlistElmt_T  slist_tail(const Slist_T * list);
//int32_t  slist_is_head(const SlistElmt_T *element);
//int32_t  slist_is_tail(const SlistElmt_T *element);
//void * slist_data(const SlistElmt_T *element); 
//SlistElmt_T slist_next (const SlistElmt_T *element); 

#define slist_size(list)  ((list)->size)
#define slist_head(list)  ((list)->head)
#define slist_tail(list)  ((list)->tail)
#define slist_is_head(list,element)  ((element) == (list)-->head ? 1 :0)
#define slist_is_tail(element) ((element)->next ==NULL ? 1: 0 )
#define slist_data(element)   ((element)->data)
#define slist_next(element)   ((element)->next)


#ifdef __cplusplus
}
#endif

#endif 
