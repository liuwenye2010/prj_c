#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>



#if 0

#define NARGS(...) (sizeof((int[]){0,##__VA_ARGS__})/sizeof(int)-1)

#define SENDCMD(cmd,app_module_id, command_id, ...) \
       SendCmd(cmd,app_module_id, command_id, NARGS (__VA_ARGS__), ##__VA_ARGS__)






#define COMMAND_OF_SIZE(n)   \
struct {                     \
  int           num_32b_words:16;\
  unsigned int  command_id:15;   \
  unsigned int  reply:1;         \
  unsigned int  app_module_id;   \
  unsigned int  data[n] ;        \
}

#define MAX_COMMAND_SIZE 13
//
typedef COMMAND_OF_SIZE(MAX_COMMAND_SIZE) Command ;

#define uint32_t unsigned int
#define int32_t  signed int

int SendCmd (Command *cmd, uint32_t  app_module_id, uint32_t  command_id, uint32_t num_32b_words, ...)
{
  va_list     args;
  uint32_t    n         ;
   uint32_t  cmd_data;
  va_start (args , num_32b_words) ;


  for (n = 0 ; n < num_32b_words ; n++)
  {
    cmd_data = va_arg(args, int32_t);
  }
  va_end (args) ;

  return 0;
}


#endif



#define _NARGN(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13,_14,_15,_16,_17, N, ...) N
#define _GRANN  17,16,15,14,13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0


#if defined(MSC_VER)



#define LEFT_PAREN (
#define RIGHT_PAREN )
#define NARGS(...) (_NARGN LEFT_PAREN __VA_ARGS__##_GRANN, _GRANN RIGHT_PAREN)

#elif  defined(GNU_C)


#define T1_NARGS(...)  sizeof((int[]){0,##__VA_ARGS__})

#define NARGS(...) (sizeof((int[]){0,##__VA_ARGS__})/sizeof(int)-1)

#else


#define NARGS(...) \
              (sizeof(#__VA_ARGS__) == sizeof("")?0:_NARG_(__VA_ARGS__, _GRANN))

#define _NARG_(...) _NARGN(__VA_ARGS__)


#endif


// below typedef located in the stdint.h
//typedef signed   short      int16_t   ;
//typedef unsigned short      uint16_t  ;
//typedef signed   long       int32_t   ;
//typedef unsigned long       uint32_t  ;
//typedef signed   long long  int64_t   ;
//typedef unsigned long long  uint64_t  ;

typedef struct CommandHeaderTag {
  int32_t   num_32b_words:16;
  uint32_t  command_id   :15;
  uint32_t  reply        : 1;
  uint32_t  mgr_module_id   ;
} CommandHeader;

typedef  uint32_t rsize_t ;
int my_printf( const char *format, ... );
void sys_print   (const char * format, ... );
int my_sprintf_s(char * buffer, rsize_t bufsz,const char *format, ...);



int main()
{

#if 0
    printf("start!\n");
    //printf("T1_NARGS %d\r\n",  T1_NARGS()  );
    unsigned int ui_a = 32;

#ifdef  GNU_C
    printf("T1_NARGS %d\r\n",  T1_NARGS(ui_a,2,3)   );
#endif

    printf("NARGS %d\r\n",NARGS(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17));

    printf("NARGS %d\r\n",NARGS());

    printf("size of (CommandHeader) = %d \n",sizeof(CommandHeader));
    //printf("NARGS %d", NARGS("0, 1, 0, 0, 0"));
#else

	int a = 0 ;
	int b = 1 ;
	my_printf("my_printf the value is %d, %d \r\n",a, b);
	printf("printf the value is %d, %d \r\n",a, b);
	sys_print("sys_print the value is %d, %d \r\n",a, b);

   char  buffer[200], s[] = "computer", c = 'l';
   int   i = 35, j;
   float fp = 1.7320534f;
#if 0
   // Format and print various data:
   j  = sprintf_s( buffer, 200,     "   String:    %s\n", s );
   j += sprintf_s( buffer + j, 200 - j, "   Character: %c\n", c );
   j += sprintf_s( buffer + j, 200 - j, "   Integer:   %d\n", i );
   j += sprintf_s( buffer + j, 200 - j, "   Real:      %f\n", fp );
   printf( "Output:\n%s\ncharacter count = %d\n", buffer, j );
#else
   j  = my_sprintf_s( buffer, 200,     "  my_sprintf_s String:    %s\n", s );
   j += my_sprintf_s( buffer + j, 200 - j, "  my_sprintf_s Character: %c\n", c );
   j += my_sprintf_s( buffer + j, 200 - j, "  my_sprintf_s Integer:   %d\n", i );
   j += my_sprintf_s( buffer + j, 200 - j, "  my_sprintf_s Real:      %f\n", fp );

   printf( "Output:\n%s\ncharacter count = %d\n", buffer, j );
#endif




	getchar();


#endif


    return 0;
}




/*
int snprintf( char *restrict buffer, size_t bufsz,
               const char *restrict format, ... );
(4)  (since C99)

*/

/*

int sprintf_s(char *restrict buffer, rsize_t bufsz,
               const char *restrict format, ...);
(7)  (since C11)

*/

int my_sprintf_s(char * buffer, rsize_t bufsz,const char *format, ...)
{

  va_list ap;
  int str_l;
  va_start(ap, format);
  //str_l = vsprintf_s(buffer, bufsz, format, ap); // since C11
  str_l = vsnprintf(buffer, bufsz, format, ap); // since c99


  va_end(ap);
  return str_l;

}






int my_printf( const char *format, ... )
{
  va_list ap;
  int str_l;
  va_start(ap, format);
  str_l =vprintf(format, ap);  //call vprintf() instead of printf() using the variable arguments "varargs" capabilities of C.
  va_end(ap);
  return str_l;
}




void sys_print   (const char * format, ... )
{
  //const int MAX_BUF_SIZE = 2000;
  #define MAX_BUF_SIZE 2000
  char szInfo[MAX_BUF_SIZE]="";
  va_list va;
  va_start(va, format);

  int nShift = strlen(szInfo);


  //vsprintf_s(szInfo + nShift,MAX_BUF_SIZE-nShift,format, va); //since c11

  vsnprintf(szInfo + nShift,MAX_BUF_SIZE-nShift,format, va); //since c99



  printf(szInfo);
  //OutputDebugStringA(szInfo);  //print to debugview

  va_end(va);
}




