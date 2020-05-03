#include <stdio.h>

void main(){
  char source[50], target[50];
  
  printf("Source Filename: ");
  scanf("%s",source);
  printf("Target Filename: ");
  scanf("%s",target);
  
  FILE *src_file=fopen(source,"r");
  FILE *tgt_file=fopen(target,"w");
  char c=getc(src_file);
  while(c!=EOF){
    putc(c,tgt_file);
    c=getc(src_file);
  }  
}

