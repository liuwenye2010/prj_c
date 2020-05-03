#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define TOTAL 100000000.0

void main(){
  uint32_t sample;
  srand(time(0));
  uint32_t i=TOTAL, hits=0;
  while(i--){
    sample = rand()%1000+1;
    if (sample<=500)
      hits++;
  }
  printf("P(hit)=%g\n",hits/TOTAL);
}

