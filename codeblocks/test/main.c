#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define Q31_T(val)   (((val)>=  1.f)?((1U<<31)-1):(((val)<  -1.f)?(1<<31U):((int32_t)((val)*2*(float)(1<<30)))<<0))
#define Q23_T(val)   (((val)>=  1.f)?((1U<<31)-1):(((val)<  -1.f)?(1<<31U):((int32_t)((val)*(float)(1<<23)))<<8))
#define Q8_23_T(val) (((val)>=256.f)?((1U<<31)-1):(((val)<-256.f)?(1<<31U):((int32_t)((val)*(float)(1<<23)))<<0))

#define FLOAT_FROM_Q31_T(val)   ((float)(val)*(0.5f/(float)(1<<30)))
#define FLOAT_FROM_Q23_T(val)   ((float)(val)*(0.5f/(float)(1<<30)))
#define FLOAT_FROM_Q8_23_T(val) ((val&(int32_t)0x80000000) ? (((float)(val)*(1.0f/(float)(1<<23)))) : ((float)(val)*(1.0f/(float)(1<<23))))
//#define FLOAT_FROM_Q8_23_T(val) ((val&(int32_t)0x80000000) ? (((float)(val)*(1.0f/(float)(1<<23))) - 512.f) : ((float)(val)*(1.0f/(float)(1<<23))))


//typedef signed   short      int16_t   ;
//typedef unsigned short      uint16_t  ;
//typedef  signed int             int32_t   ;
//typedef  unsigned int           uint32_t  ;
//typedef signed   long long  int64_t   ;
//typedef unsigned long long  uint64_t  ;


int main()
{
#if  0
    int32_t error_code =0;
    int32_t error_cmd =0;
    uint32_t data =0x017bfbdf; //b32d2300

    printf("test begin\n");
    error_code =(int32_t) ((data | 0xFFFF0000));
    error_cmd = (int32_t) ((data & 0xFFFF0000)>>16);

    printf("error_code == 0x%08x\r\n", error_code);
    //printf("error_code == %d\r\n", error_code);
    printf("error_cmd  == 0x%08x\r\n", error_cmd);
    //printf("error_cmd  == %d\r\n", error_cmd);
#else


#endif
    float  fval_1 = 1.5;
    float  fval_2 = -1.5;
    float  fval_3 = 0.25;
    float  fval_4 = -0.25;

    printf("Q31_T(%f) == 0x%08x\r\n", fval_1, Q31_T(fval_1));
    printf("Q31_T(%f) == 0x%08x\r\n", fval_2, Q31_T(fval_2));
    printf("Q31_T(%f) == 0x%08x\r\n", fval_3, Q31_T(fval_3));
    printf("Q31_T(%f) == 0x%08x\r\n", fval_4, Q31_T(fval_4));

    int32_t  dval_1 = 0x7fffffff;  /* you can't use the  uint32_t , you should use int32_t for  Q31_t  */
    int32_t  dval_2 = 0x80000000;
    int32_t  dval_3 = 0x20000000;
    int32_t  dval_4 = 0xe0000000;

    printf("FLOAT_FROM_Q31_T(0x%x) == %f\r\n", dval_1, FLOAT_FROM_Q31_T(dval_1));
    printf("FLOAT_FROM_Q31_T(0x%x) == %f\r\n", dval_2, FLOAT_FROM_Q31_T(dval_2));
    printf("FLOAT_FROM_Q31_T(0x%x) == %f\r\n", dval_3, FLOAT_FROM_Q31_T(dval_3));
    printf("FLOAT_FROM_Q31_T(0x%x) == %f\r\n", dval_4, FLOAT_FROM_Q31_T(dval_4));



    /*  ================================================ */
      fval_1 = 1.5;
      fval_2 = -1.5;
      fval_3 = 0.25;
      fval_4 = -0.25;

    printf("Q8_23_T(%f) == 0x%08x\r\n", fval_1, Q8_23_T(fval_1));
    printf("Q8_23_T(%f) == 0x%08x\r\n", fval_2, Q8_23_T(fval_2));
    printf("Q8_23_T(%f) == 0x%08x\r\n", fval_3, Q8_23_T(fval_3));
    printf("Q8_23_T(%f) == 0x%08x\r\n", fval_4, Q8_23_T(fval_4));

#if 0
    uint32_t  udval_1 = 0x00c00000;  /* for the Q8_23_T , please consider it as the unsigned int  */
    uint32_t  udval_2 = 0xff400000;
    uint32_t  udval_3 = 0x00200000;
    uint32_t  udval_4 = 0xffe00000;
#else

    int32_t  udval_1 = 0x00c00000;  /* for the Q8_23_T , please consider it as the unsigned int  */
    int32_t  udval_2 = 0xff400000;
    int32_t  udval_3 = 0x00200000;
    int32_t  udval_4 = 0xffe00000;

#endif

    printf("FLOAT_FROM_Q8_23_T(0x%x) == %f\r\n", udval_1, FLOAT_FROM_Q8_23_T(udval_1));
    printf("FLOAT_FROM_Q8_23_T(0x%x) == %f\r\n", udval_2, FLOAT_FROM_Q8_23_T(udval_2));
    printf("FLOAT_FROM_Q8_23_T(0x%x) == %f\r\n", udval_3, FLOAT_FROM_Q8_23_T(udval_3));
    printf("FLOAT_FROM_Q8_23_T(0x%x) == %f\r\n", udval_4, FLOAT_FROM_Q8_23_T(udval_4));


    return 0;

}


