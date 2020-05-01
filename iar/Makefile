.PHONY: main clean 

#-g -mcpu=cortex-m4 -mthumb -fno-short-enums -O2 -Os -fdata-sections -ffunction-sections -Wall -fno-exceptions 
main: 
	iccarm --c++ --endian=little --cpu=Cortex-M3 --fpu=None --cpu_mode=thumb --no_exceptions --debug   main.cpp
	iarchive --verbose --create -o main.so main.o
	iarchive --verbose --symbols main.so 
	ilinkarm main.o --no_exceptions -o main
#	isymexport	main main.sysbols
#	ielfdumparm main 
	
#g++ -g -fno-short-enums -O2 -Os -fdata-sections -ffunction-sections -Wall -fno-exceptions  main.c  -o main 
   

clean:
	rm -rf *.o main.exe 

#compiling 
#iccarm.exe D:\Program Files (x86)\IAR Systems\Embedded Workbench 8.1\arm\tutorials\GettingStarted\Fibonacci.c -o D:\Program Files (x86)\IAR  
#Systems\Embedded Workbench 8.1\arm\tutorials\GettingStarted\Debug\Obj --no_cse --no_unroll --no_inline --no_code_motion --no_tbaa --no_clustering  
#--no_scheduling --debug --endian=little --cpu=Cortex-M3 -e --fpu=None --dlib_config D:\Program Files (x86)\IAR Systems\Embedded Workbench 8.1\arm\ 
#inc\c\DLib_Config_Normal.h -Ol 
	
	
#Linking 
#ilinkarm.exe D:\Program Files (x86)\IAR Systems\Embedded Workbench 8.1\arm\tutorials\GettingStarted\Debug\Obj\Fibonacci.o D:\Program Files (x86)\ 
#IAR Systems\Embedded Workbench 8.1\arm\tutorials\GettingStarted\Debug\Obj\Utilities.o --no_out_extension -o D:\Program Files (x86)\IAR Systems\ 
#Embedded Workbench 8.1\arm\tutorials\GettingStarted\Debug\Exe\BasicDebugging.out --map D:\Program Files (x86)\IAR Systems\Embedded  
#Workbench 8.1\arm\tutorials\GettingStarted\Debug\List\BasicDebugging.map --config D:\Program Files (x86)\IAR Systems\Embedded Workbench 8.1\ 
#arm\CONFIG\generic_cortex.icf --semihosting --entry __iar_program_start --vfe --text_out locale 
	