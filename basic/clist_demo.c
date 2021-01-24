#include "clist.h"

#include <stdio.h>

/**
 * @brief demo how to use the circular list
 *
 * @return int32_t
 */

int32_t clist_demo(void)
{

     printf("\n====================== DEMO Cicular List  START=================================\n");
     int32_t err_no = 0;
     #define BUF_SIZE   100
     #define ITEM_NUM   5 
     Clist_T   list;
     Clist_T   *plist = &list;
     void *data_buf = NULL;
     do
     {
         //init one list
         clist_init(plist, free);

         //add  5 intems into  the circular list
         ClistElmt_T * pelmt_pos = plist->head; //the plist->head == NULL the first time 
         for (int i = 0; i < ITEM_NUM; i++)
         {
             data_buf = (void *)malloc(BUF_SIZE * sizeof(int32_t));
             if (data_buf == NULL)
             {
                 err_no = -1;
                 break;
             }
             ((int32_t*)data_buf)[0] = i; 
             //((int32_t*)data_buf)[1] = i*i; 
            clist_ins_next(plist,pelmt_pos,data_buf);
            if(pelmt_pos == NULL)
                pelmt_pos = plist->head; //after this time , all items will insert sequnentially
            else
                pelmt_pos = pelmt_pos->next; //update insert posit for next time

            printf("\ninsert item (%d) into the list, buf:0x%x, value:%d\n",i,(uint32_t)pelmt_pos->data,((int32_t*)pelmt_pos->data)[0]);
            printf("List Size:%d\n", clist_size(plist));
         }

        //go through the circular list with iterate 100 times 
         void *recv_buf = NULL;
         pelmt_pos = plist->head;
         for (int i = 0; i < 100; i++)
         {
             recv_buf = pelmt_pos->data; 
             printf("\n read item (%d) into the list, buf:0x%x, value:%d\n",i,(uint32_t)recv_buf,((int32_t*)recv_buf)[0]);
             pelmt_pos = pelmt_pos->next; 
         }

         //destory whole list and free the data allocate before
         clist_destory(plist);
         printf("Destory whole list (incluing free the node + data buffer)\n");
         printf("List Size:%d\n", clist_size(plist));

     } while (0);

     // free the memory in case if there is one error. 
     if(err_no <0)
     {
        printf("?? free the memory after meeting error\n");
        //how to ? 
     }

     printf("\n====================== DEMO Cicular List  END =================================\n");

     return err_no;
//error:
//    return -1;


}

