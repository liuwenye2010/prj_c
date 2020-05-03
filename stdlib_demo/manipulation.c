#include <stdio.h>
#include <string.h>

void main(){
  char str1[10]="Hi ";
  char str2[10]="There!"; 
  char str3[10]="---"; 
  
  printf("     str1 = \"%s\"\n",str1);
  printf("     str2 = \"%s\"\n",str2);
  printf("     str3 = \"%s\"\n",str3);
  //concatenate str1 and str2
  //copy str1 -> str3
  printf("     str1 = \"%s\"\n",str1);
  printf("     str2 = \"%s\"\n",str2);
  printf("     str3 = \"%s\"\n",str3);
}