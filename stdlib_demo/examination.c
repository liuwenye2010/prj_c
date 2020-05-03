#include <stdio.h>
#include <string.h>

void compare(char *lhs, char*rhs){  // write this function
}

void main(){
  char str1[100]="Strings in C are easy!";
  char str2[100]="Strings in Java are easy!";
  char str3[100]="Strings in Python are easy!";
  char str4[100]="Strings in C are easy!";
  compare(str1,str2);
  compare(str2,str1);
  compare(str2,str3);
  compare(str1,str4);
  compare(str3,str1);
  
  char substring[]="Java";
  // Look for substring
}