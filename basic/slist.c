#include <stdlib.h>
#include <string.h>
#include "slist.h"


/*Single List init*/


/** \brief slist_init for one single list
 *
 * \param data void*
 * \return void
 *
 */
void slist_init(Slist_T *list, void (*destory)(void* data))
{
    list->size = 0;
    list->destory = destory;
    list->head = NULL;
    list->tail = NULL;
    return;
}
/**
 * @brief slist_destory 
 * 
 * @param list 
 */
void slist_destory(Slist_T *list)
{
   void * data;
   while( slist_size(list) >0)
   {
       if((slist_rem_next(list,NULL,(void**)&data)== 0) &&  (list->destory != NULL) )
       {
           list->destory(data); // free memory by data pointer
       }
   }
   memset(list,0, sizeof(list));
   return;
}
/**
 * @brief insert one item after the element in the list 
 * 
 * @param list 
 * @param element 
 * @param data 
 * @return int32_t 
 */

int32_t slist_ins_next( Slist_T *list, SlistElmt_T *element, const void* data)
{
    SlistElmt_T *new_element ;
    if ((new_element = (SlistElmt_T *) malloc(sizeof(SlistElmt_T)))== NULL)
    {
        return -1;
    }

    new_element->data = (void*)data;
    if(element == NULL) // insert into at the head of list
    {
        if(slist_size(list)  == 0 )
            list->tail = new_element;

        new_element->next = list->head;
        list->head = new_element;
    }
    else  //other than head  (middle or tail)
    {
        if (element->next == NULL)
            list->tail = new_element;

        new_element->next = element->next;
        element->next = new_element;
    }

    //update the size
    list->size ++;
    return 0;
}



/** \brief
 *
 * \param list Slist_T*
 * \param element SlistElmt_T*
 * \param data void**
 * \return int32_t
 *
 */
int32_t slist_rem_next( Slist_T *list, SlistElmt_T *element,  void** data)
{
    SlistElmt_T *old_element;
    if (slist_size(list) == 0)
        return -1;
    if(element == NULL)  //remove from head (if you like you can remove from tail)
    {
        *data = list->head->data;
        old_element  = list->head;
        list->head = list->head->next;
        if(slist_size(list)== 1)
            list->tail = NULL;
    }
    else
    {
        if (element->next == NULL)//remove from other than head
            return -1;
        *data = element->next->data;
        old_element = element->next;
        element->next = element->next->next;

        if(element->next ==NULL)
        {
            list->tail = element;
        }
    }

    free(old_element);
    list->size--;
    return 0;

}


