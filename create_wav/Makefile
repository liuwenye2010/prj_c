.PHONY: all clean 

CC=gcc
CFLAGS=-g -fno-short-enums -O2 -Os -fdata-sections -ffunction-sections -Wall -fno-exceptions 
DEPS= 

TARGET	= create_mono_wave.exe

HEADERS 	= $(wildcard *.h)
SRC 		= $(wildcard *.c)
OBJS 		= $(SRC:.c=.o)

dis-obj			= $(TARGET).dis
size-obj		= $(TARGET).size 
bin-obj			= $(TARGET).bin
bin-map         = $(TARGET).map

target_all      = $(TARGET) $(dis-obj) $(siz-obj_ns) $(bin-obj) $(bin-map)  


all:$(target_all)

%.o:%.c
	$(CC) $(CFLAGS) -o $@ -c $< 

$(TARGET) : $(OBJS)
	$(CC) -g -o $@ -Wl,-Map,$(TARGET).map  $^

   
$(dis-obj):$(TARGET)
	objdump -D -S -l $< > $@

$(size-obj):$(TARGET)
	size --format=berkeley $< > $@

$(bin-obj):$(TARGET)
	objcopy -v -S -g -O binary $< $@   
   
clean:
	rm -rf *.o *.map *.dis *.size *.bin *.wav $(target_all)