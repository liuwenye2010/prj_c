#include <stdint.h> 
#include <stdio.h> 
#include <string.h>

//show progress 
#define InitShowProgress()  fprintf(stdout, "Processing ... %3d%%", 0); fflush(stdout);
#define ShowProgress(curPos,MaxPos) fprintf(stdout, "\b\b\b\b%3d%%", (char) (((curPos) *100)/MaxPos) ); fflush(stdout);

struct message_s
{
	int32_t   cmd:6;			
	uint32_t  err:1;			
	uint32_t  repl:1;			
	uint32_t  blk:8;			
	uint32_t  len:16;			
	uint32_t  crc;			       
} ;			       

#define SIZE(a) (int32_t)(sizeof(a) / sizeof(a[0]))

//show pass and failure 
static const char *const szPass[] = {
  "                                        \n",
  "  ########     ###     ######   ######  \n",
  "  ##     ##   ## ##   ##    ## ##    ## \n",
  "  ##     ##  ##   ##  ##       ##       \n",
  "  ########  ##     ##  ######   ######  \n",
  "  ##        #########       ##       ## \n",
  "  ##        ##     ## ##    ## ##    ## \n",
  "  ##        ##     ##  ######   ######  \n",
};

static const char *const szFail[] = {
  "                          \n",
  "   ######   ##   # #      \n",
  "   #       #  #  # #      \n",
  "   #####  #    # # #      \n",
  "   #      ###### # #      \n",
  "   #      #    # # #      \n",
  "   #      #    # # ###### \n",
};


void show_result(bool p)
{
  if (p)
  {
    //set_txt_color(COLOR_PASS);
    for (int32_t i = 0; i < SIZE(szPass); i++)
    {
      fprintf(stdout, "%s", szPass[i]);
    }
  }
  else
  {
    //set_txt_color(COLOR_ERROR);
    for (int32_t i = 0; i < SIZE(szFail); i++)
    {
      fprintf(stdout, "%s", szFail[i]);
    }
  }
  //set_txt_color(COLOR_NORMAL);
}



int32_t endianess_is_little()
{
    uint32_t endianness = 1;
    return ((char*)(&endianness))[0]==1;

}

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
			show_result(false);
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
	}
	
	if (c_opt)  printf(" c_opt is enabled\n");
	
	if (v_opt)  printf(" v_opt is enabled with value 0x%x\n",v_val);
	InitShowProgress();
	delay(100000);
	ShowProgress(5,10);
	delay(100000);
	ShowProgress(10,10);
	
	show_result(true);
	return 0;
	
  }while(0);
   
  show_result(false);
  return 0 ;
  
}
