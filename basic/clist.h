
#ifndef _CLIST_H_INCLUDED_
#define _CLIST_H_INCLUDED_

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include <stdlib.h> 

//Single Link node
typedef  struct  ClistElmt_S 
{
    void * data;        // data field
    struct  ClistElmt_S *next; // pointer filed
} ClistElmt_T;

//Single List list structures
typedef struct Clist_S
{
    int32_t size; 
    int32_t (*match)(const void* key1, const void *key2);
    void (*destory)(void* data); 
    ClistElmt_T *head;
}Clist_T; 

//Single List public interface 

void  clist_init ( Clist_T *list, void(*destroy)(void*data));
void  clist_destory ( Clist_T *list); 
int32_t clist_ins_next( Clist_T *list, ClistElmt_T *element, const void* data);//insert one element to the list 
int32_t clist_rem_next( Clist_T *list, ClistElmt_T *element, void** data);//remove one element to the list

//int32_t clist_size(const Clist_T *list); 
//ClistElmt_T  clist_head(const Clist_T * list);
//ClistElmt_T  clist_tail(const Clist_T * list);
//int32_t  clist_is_head(const ClistElmt_T *element);
//int32_t  clist_is_tail(const ClistElmt_T *element);
//void * clist_data(const ClistElmt_T *element); 
//ClistElmt_T clist_next (const ClistElmt_T *element); 

#define clist_size(list)  ((list)->size)
#define clist_head(list)  ((list)->head)
#define clist_is_head(list,element)  ((element) == (list)-->head ? 1 :0)
#define clist_data(element)   ((element)->data)
#define clist_next(element)   ((element)->next)


#ifdef __cplusplus
}
#endif

#endif 
