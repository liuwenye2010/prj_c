#include <stdio.h>

void main(){
  int counter=0;
  FILE *f=fopen("names.txt","r+"); 
  char c=getc(f); //get first char 
  while (c!=EOF) {
	  putchar(c); 
	  c=getc(f); //get nexe char 
	  counter ++ ;
  }
  fprintf(f,"\ncounter = %d\n",counter);
  fclose(f);
}
