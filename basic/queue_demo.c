#include <stdio.h> 
#include "queue.h" 


int32_t queue_demo(void)
{   
     printf("\n====================== DEMO Queue START=================================\n");
     
     int32_t err_no = 0;
     #define BUF_SIZE   100
     Queue_T   queue;
     Queue_T   *pqueue = &queue;
     void *data_buf1 = NULL;
     void *data_buf2 = NULL;
    
     do
     {
         //init one queue
         queue_init(pqueue, free);

         //add two items to the queue
         data_buf1 = (void *)malloc(BUF_SIZE * sizeof(int32_t));
         if (data_buf1 == NULL)
         {
             err_no = -1;
             break;
         }
         queue_enqueue(pqueue, data_buf1);
         printf("\ninsert one item into the queue\n");
         printf("queue Size:%d\n", queue_size(pqueue));

         data_buf2 = (void *)malloc(BUF_SIZE * sizeof(int32_t));
         if (data_buf2 == NULL)
         {
             err_no = -1;
             break;
         }
         queue_enqueue(pqueue, data_buf2); if (err_no<0) break;
         printf("insert another item into the queue\n");
         printf("queue Size:%d\n", queue_size(pqueue));
       
        //remove/get  one items from the queue
         void *recv_buf = NULL;
         err_no = queue_dequeue(pqueue, &recv_buf); if (err_no<0) break;
         printf("remove/get one item from the queue\n");
         free(recv_buf);
         printf("free the memory after use\n");
         printf("queue Size:%d\n", queue_size(pqueue));
        
         //destory whole queue and free the data allocate before
         queue_destory(pqueue);
         printf("Destory whole queue (incluing free the node + data buffer)\n");
         printf("queue Size:%d\n", queue_size(pqueue));

     } while (0);

     // free the memory in case if there is one error. 
     if(err_no <0)
     {
        printf("free the memory after meeting error\n");
        if (data_buf1) free(data_buf1);
        if (data_buf2) free(data_buf2);
     }
     printf("\n====================== DEMO Queue  END=================================\n");

     return err_no;
//error:
//    return -1;
}

