//want one link list to chain request 

#include <stdint.h> 
#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include "list.h"

//Static HW define 
struct i2c_s
{
  struct list_s head;      // head of list of request
  /*
  other control members 
  */
} g_i2c;

//request list nodes 
typedef struct request_hdr_s         
{                                       
  struct list_s      list; //list node 
  //members of each request 
  uint32_t			 id;
  uint32_t*          data_buf; 
  uint32_t           buf_size; 
  //
} request_hdr_t;

//该链表的设计, head 是占位符号, 可以选择使用
//return number of nodes 
int32_t print_out_dlist_nodes (struct list_s* head) {
    int32_t n_num = 0 ;
    if ((head == NULL) || head->next ==NULL) 
    {
        return  -1;  
    }
    struct list_s* p_node  = head; 
    while(p_node->next != head) //here does not pring head itself, head just as one marker,you can use do while if want to  print out
    {
        printf("p_node: ???");
        n_num ++; 
    }
    return n_num;
}

void double_link_list_demo(void)  {
    //some data_buffer pool 
    #define BUF_NUM   5
    #define BUF_SIZE  10
    uint32_t data_buf_pool[BUF_NUM][BUF_SIZE];
    
    //init the link list 's head
     _list_initialize(&(g_i2c.head));

    //check the list is empty or not 
    
    //create on new node and add list node into the list
    
     request_hdr_t  req1 ;
     req1.data_buf  = (&(data_buf_pool[0][0]));
     req1.buf_size  =  BUF_SIZE; 
     // Take care the function type _list_add_after (node, list)
     _list_add_after(&req1.list,g_i2c.head.prev);  //  head <--> req1 
     request_hdr_t  req2 ;
     req2.data_buf  = (&(data_buf_pool[1][0]));
     req2.buf_size  =  BUF_SIZE;                    //   |>---------------------------  <|
     _list_add_after(&req2.list,g_i2c.head.prev);   //   |<--head <--> req1 <---> req2-->|
     request_hdr_t  req3 ;
     req3.data_buf  = (&(data_buf_pool[2][0]));     //Note:  i2cm.head.prev always point to the last list node in the doulbe list
     req3.buf_size  =  BUF_SIZE;                    //   |>--------------------------- --------- <|
     _list_add_after(&req3.list,g_i2c.head.prev);   //   |<--head <--> req1 <---> req2-->req3---->|
    
     //Print out the list nodes  
     
     //consume the item in the list 

     //delete item from the list
     
     //delete the double list  (note still keep the head node)
    
	
	
}