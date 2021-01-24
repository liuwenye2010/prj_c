#include "slist.h"

#include <stdio.h>

/**
 * @brief demo how to use the single list
 *
 * @return int32_t
 */

int32_t slist_demo(void)
{
     int32_t err_no = 0;
     #define BUF_SIZE   100
     Slist_T   list;
     Slist_T   *plist = &list;
     void *data_buf1 = NULL;
     void *data_buf2 = NULL;

     do
     {
         //init one list
         slist_init(plist, free);

         //add two items to the list
         data_buf1 = (void *)malloc(BUF_SIZE * sizeof(int32_t));
         if (data_buf1 == NULL)
         {
             err_no = -1;
             break;
         }
         slist_ins_next(plist, NULL, data_buf1);
         printf("\ninsert one item into the list\n");
         printf("List Size:%d\n", slist_size(plist));

         data_buf2 = (void *)malloc(BUF_SIZE * sizeof(int32_t));
         if (data_buf2 == NULL)
         {
             err_no = -1;
             break;
         }
         slist_ins_next(plist, NULL, data_buf2); if (err_no<0) break;
         printf("insert another item into the list\n");
         printf("List Size:%d\n", slist_size(plist));
       
        //remove/get  one items from the list
         void *recv_buf = NULL;
         err_no = slist_rem_next(plist, NULL, &recv_buf); if (err_no<0) break;
         printf("remove/get one item from the list\n");
         free(recv_buf);
         printf("free the memory after use\n");
         printf("List Size:%d\n", slist_size(plist));
        
         //destory whole list and free the data allocate before
         slist_destory(plist);
         printf("Destory whole list (incluing free the node + data buffer)\n");
         printf("List Size:%d\n", slist_size(plist));

     } while (0);

     // free the memory in case if there is one error. 
     if(err_no <0)
     {
        printf("free the memory after meeting error\n");
        if (data_buf1) free(data_buf1);
        if (data_buf2) free(data_buf2);
     }

     return err_no;
//error:
//    return -1;
}

