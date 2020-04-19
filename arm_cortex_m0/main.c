#include <stdio.h>
#include "fun.h"
#define printf(...)  


int add(int a , int b)
{
    printf("%s: %d\n", __FUNCTION__, __LINE__);
	int c = a+b;
    return c;
 
}

int sub(int a , int b)
{
    printf("%s: %d\n", __FUNCTION__, __LINE__);
	int c = a-b;
    return c;
}

int mul(int a , int b)
{
    printf("%s: %d\n", __FUNCTION__, __LINE__);
	int c = a*b;
    return c;
}

int div(int a , int b)
{
    printf("%s: %d\n", __FUNCTION__, __LINE__);
	int c = a/b;
    return c;
}

long sum(long *start ,long count)
{
	long sum =0; 
	while(count){
		
		sum +=*start; 
		start++; 
		count--; 
	}
	return sum; 
}


int control_isa(int a, int b)
{
	if(a> 10)
		a=0; 
	else
		a=a+b; 

	return a ; 
}


int for_loop_isa(void)
{
	int total = 0; 
	int i  = 0; 
	for (i =0; i< 5; i++)
		total+=i; 
	return total; 
}


int switch_isa(x)
{
	int c=0; 
	switch (x) {
	case 0: printf("x is 0");c =1;  break;
	case 1: printf("x is 1");c=2 ; break;
	case 2: printf("x is 0");c =3;  break;
	case 3: printf("x is 1");c=4 ; break;
	case 4: printf("x is 0");c =5;  break;
	case 5: printf("x is 1");c=6; break;
	default: printf("x is not 0 or 1"); c=10;  break;
	}
	return c; 
}

int fun_b(void)
{
    return 1; 
}

int fun_c(void)
{
	int a =1; 
	int b =2; 
	int c =3; 
	int d= 4; 
	int e= 5; 
	int f= 6; 
	f= a+b+c+d+e; 
	return f; 
}
int fun_a(void)
{
	fun_b(); 
	fun_c();
	return 0; 
}


int main(void)
{
	    int a =2; 
		int b= 3; 
		int c =0;
	    long x[10]={1,2,3,4,5,6,7,8,9,10};
		long y=0; 
        c =add(a,b);
		c= sub(a,b); 
        c= mul(a,b);
		c= div(a,b);
		y=sum(x,10); 
		control_isa(a,b);
		for_loop_isa(); 
		switch_isa(a);
		fun_a();
		printf("sum:%d\n",b); 
		
		sub_fun_add(a,b); 
		
		
		return 0; 
}

