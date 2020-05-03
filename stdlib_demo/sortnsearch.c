#include <stdio.h>
#include <stdlib.h>

void print_array(int *arr, int count){
  while(count--)
    printf("  %i  ",*(arr++));
  printf("\n");
}

// Compare function
// Find function

void main(){
  int nums[10]={9,5,7,1,8,5,10,12,4,2};
  int d_size=sizeof(int);  
  int a_size=sizeof(nums)/d_size;
  
  // Print Array
  print_array(nums,a_size);
  // Sort Array
  // Print again
  // Search for a number in the array
  // Search for a number NOT in the array
}