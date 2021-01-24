#include <stdint.h> 
#include <stdio.h> 
#include <string.h>

#define INCLUDE_SLIST_DMEO  1
#define INCLUDE_CLIST_DEMO  1

#if  INCLUDE_SLIST_DMEO 
extern int32_t slist_demo(void);
#endif 

#if  INCLUDE_CLIST_DEMO 
extern int32_t clist_demo(void);
#endif 

#define false 0 
#define true  1
//show progress 
#define InitShowProgress()  fprintf(stdout, "Processing ... %3d%%", 0); fflush(stdout);
#define ShowProgress(curPos,MaxPos) fprintf(stdout, "\b\b\b\b%3d%%", (char) (((curPos) *100)/MaxPos) ); fflush(stdout);


#define ARRRY_SIZE(a) (int32_t)(sizeof(a) / sizeof(a[0]))

void delay(uint32_t cnt)
{
	while(cnt>0)
		cnt--;
}

void usage(void )
{
	printf("Usage:\n");
	printf("demo_tool.exe <Options>\n");
	printf("  -c    :  enable option c \n");
	printf("  -v  value  :  enable option v with value \n");
}


int32_t main(int32_t argc, char * argv[])
{
	
	int32_t c_opt = false; 
	int32_t v_opt = false;
	int32_t v_val = 0; 
 do
  {
    fprintf(stdout,"demo tool\n\n");
    if (argc > 1)
    {
      for (int32_t i=1; i < argc; i++) 
	  {
        char *argu_opt = NULL;
        argu_opt = argv[i];
        if (strcmp(argu_opt, "-c") == 0)
        {
          c_opt = true;
        } 
		else if (strcmp(argu_opt, "-v") == 0)
        {
		  v_opt = true ;
		  if((i+1) <argc)
			sscanf(argv[i+1], "%d", &v_val);
	      else
		  {
		    printf("err: missing value with -v option.");
		    usage();
			return -1; 
		  }
		}
		else if (strcmp(argu_opt, "-h") == 0)
        {
		  usage();
		  return 0; 
          
	    }
	  }
	}
	else
	{
		usage();
		printf("[DEFAULT-DEMO]:");
	}
	
	if (c_opt)  printf(" c_opt is enabled\n");
	
	if (v_opt)  printf(" v_opt is enabled with value 0x%x\n",v_val);
    
    //do something here for demo demo 
#if INCLUDE_SLIST_DMEO
	slist_demo();
#endif 

#if INCLUDE_CLIST_DEMO
	clist_demo();
#endif 
	
	//end; 
	
  }while(0);
   
  return 0 ;
  
}
