#include <stdio.h>
#include <math.h>

#define PI 3.14159

void main(){
  double A_x, A_y, B_radius, B_angle;
  printf("Enter X coordinate of Point A: "); 
  scanf("%lf",&A_x);
  printf("Enter Y coordinate of Point A: ");
  scanf("%lf",&A_y);
  
  printf("Point A is represented in polar coordinates as (%g, %g deg)\n\n",
          hypot(A_x,A_y),atan2(A_y,A_x)*180.0/PI);

  printf("Enter Radius of Point B: ");
  scanf("%lf",&B_radius);
  printf("Enter Angle of Point B in Degrees: ");
  scanf("%lf",&B_angle);
  
  printf("Point B is represented in cartesian coordinates as (%g, %g)\n\n",
          B_radius*cos(B_angle*PI/180.0),B_radius*sin(B_angle*PI/180.0));
}




