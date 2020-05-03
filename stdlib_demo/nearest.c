#include <stdio.h>
#include <math.h>

void main(){
  double hours=3.1, age=14.8;
  
  printf("I parked for %g hours, so I got %g hours charged.\n",hours,/*fill in*/);

  printf("Jake is %g years old, so he's %g.\n\n",age,/*fill in*/);
  
  printf("I parked for %g hours, so I stayed a little longer than %g hours.\n",
          hours,/*fill in*/);
  
  printf("Jake is %g years old, so he's practically %g.\n",age,/*fill in*/);
  
  double x=-5.6;
  printf("x = %g, ceil(x)=%g, floor(x)=%g, trunc(x)=%g, round()x.=%g\n",
          x,ceil(x),floor(x),trunc(x),round(x));
}