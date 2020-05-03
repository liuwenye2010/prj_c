#include <stdio.h>
#include <string.h>

void main(){
  char sentence[100], word[50], temp[100];
  printf("Enter long string: ");
  scanf("%[^\n]s",sentence);
  getchar();
  printf("Enter substring to remove: ");
  scanf("%[^\n]s",word);

  char *str=strstr(sentence,word);
  if(str!=NULL){
    *str='\0';
    str += strnlen(word,50);
    strncpy(temp,str,100);
    strncat(sentence,temp,100);
    printf("Removing the word: \"%s\"\n",sentence);
  }
  else{
    printf("\"%s\" not found in \"%s\"!\n",word,sentence);
  }
}





