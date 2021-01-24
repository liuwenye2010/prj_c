#include <stdlib.h>
#include <string.h>
#include "clist.h"


/*Circular List init*/

void clist_init(Clist_T *list, void (*destory)(void* data))
{
    list->size = 0;
    list->destory = destory;
    list->head = NULL;
    return;
}

void clist_destory(Clist_T *list)
{
   void * data;
   while( clist_size(list) >0)
   {
       if((clist_rem_next(list,list->head,(void**)&data)== 0) &&  (list->destory != NULL) )
       {
           list->destory(data); // free memory by data pointer
       }
   }
   memset(list,0, sizeof(list));
   return;
}

// Note, different from slist, below element can not be NULL
int32_t clist_ins_next( Clist_T *list, ClistElmt_T *element, const void* data)
{
    ClistElmt_T *new_element ;
    if ((new_element = (ClistElmt_T *) malloc(sizeof(ClistElmt_T)))== NULL)
    {
        return -1;
    }
    new_element->data = (void*)data;
    if(clist_size(list)== 0) // insert into when  list is empty 
    {
        new_element->next = new_element; 
        list->head = new_element;
    }
    else  //if the list is not  empty
    {
        new_element->next = element->next;
        element->next = new_element;
    }

    //update the size
    list->size ++;
    return 0;
}

// Note, different from slist, below element can not be NULL
int32_t clist_rem_next( Clist_T *list, ClistElmt_T *element,  void** data)
{
    ClistElmt_T *old_element;
    if (clist_size(list) == 0)
        return -1;
    
    *data = element->next->data; 
    if(element->next == element )  //only one item
    {   //remove the last elment 
        old_element  = element->next;
        list->head = NULL; 
    }
    else
    {
        old_element = element->next;
        element->next = element->next->next;
        if(old_element = list->head)  //if item be remove is the head, then update the head.
            list->head = old_element->next; 
    }
    
    free(old_element);
    list->size--;
    return 0;

}


