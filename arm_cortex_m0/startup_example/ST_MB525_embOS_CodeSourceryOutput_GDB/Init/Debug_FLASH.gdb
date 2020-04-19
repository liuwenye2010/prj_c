#*************************************************
#
#  Connect to J-Link and debug application in flash on Cortex-M3
#
#  no download is performed.
#
# Connect to the J-Link gdb server
target remote localhost:2331
# Reset the target
monitor flash device = STM32F103VB
monitor speed 1000
load ST_MB525_FLASH.elf
monitor reset 0