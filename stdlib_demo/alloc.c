#include <stdio.h>
#include <stdlib.h>

char *str;
  
void main(){
  scanf("%[^\n]s",str);
  printf("\n%s\n",str);                        
}