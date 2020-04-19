#*************************************************
#
#  Connect to J-Link and debug application in flash on Cortex-M3
#
#  Download to flash is performed.
#
# Connect to the J-Link gdb server
target remote localhost:2331
# Reset the target
monitor reset 0
monitor speed 1000
load ST_MB525_RAM.elf
monitor reg r13 = (0x20000000)
monitor reg pc = (0x20000004)