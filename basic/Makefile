.PHONY: all clean 

CC=g++
CFLAGS=-g -fno-short-enums -O2 -Os -fdata-sections -ffunction-sections -Wall -fno-exceptions 
DEPS= 

TARGET	= main.exe

HEADERS 	= $(wildcard *.h)
SRC 		= $(wildcard *.c)
OBJS 		= $(SRC:.c=.o)
cpp_check_rpt   = $(TARGET).cppcheck


dis-obj			= $(TARGET).dis
size-obj		= $(TARGET).size 
bin-obj			= $(TARGET).bin
bin-map         = $(TARGET).map

target_all      = $(cpp_check_rpt) $(TARGET) $(dis-obj) $(siz-obj_ns) $(bin-obj) $(bin-map)  



all: $(cpp_check_rpt) $(target_all)

%.o:%.c
	$(CC) $(CFLAGS) -o $@ -c $< 

$(TARGET) : $(OBJS)
	$(CC) -g -o $@ -Wl,-Map,$(TARGET).map  $^

#-g -mcpu=cortex-m4 -mthumb -fno-short-enums -O2 -Os -fdata-sections -ffunction-sections -Wall -fno-exceptions 
#$(target-output):main.c
#	g++ $(CFLAGS) $< -o $@  

#	cppcheck --enable=all &> $(SRC)  or  cppcheck --enable=all &> $(SRC) 2>&1

$(cpp_check_rpt):
	cppcheck --enable=all $(SRC) &>$@  
$(dis-obj):$(TARGET)
	objdump -D -S -l $< > $@

$(size-obj):$(TARGET)
	size --format=berkeley $< > $@

$(bin-obj):$(TARGET)
	objcopy -v -S -g -O binary $< $@   
   
clean:
	rm -rf *.o *.map *.dis *.size *.bin $(target_all)