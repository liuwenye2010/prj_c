#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define ARRAY_SIZE 100

typedef struct{
  int x;
  char c;
} my_pair;

void main(){
  my_pair a,b;
  uint8_t array[ARRAY_SIZE];
  int i;
  i=ARRAY_SIZE;
  while(i--) 
    printf(" %i ",array[i]);
  printf("\n\n");
  
  memset(array,0,ARRAY_SIZE);// Initialize the whole array to 0
  
  i=ARRAY_SIZE;
  while(i--) 
    printf(" %i ",array[i]);
  printf("\n\n");
  
  a.x=10;
  a.c=20;
  printf("a.x=%i and a.c=%i\n",a.x,a.c);
  printf("b.x=%i and b.c=%i\n\n",b.x,b.c);
  
// copy a->b
  
  printf("a.x=%i and a.c=%i\n",a.x,a.c);
  printf("b.x=%i and b.c=%i\n\n",b.x,b.c);
}