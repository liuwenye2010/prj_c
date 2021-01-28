
INCROOT:=.
V:=@

empty:=
space:=$(empty) $(empty)
comma:=,

MAKE := make
MKDIR := mkdir -p
srctree:= .
BUILD_DIR := $(srctree)/build
STARTUP_DEFS=-D__STARTUP_CLEAR_BSS -D__START=main -D__NO_SYSTEM_INIT

mcu-y:=cortex-m0
MCU:=$(mcu-y)
TRUE=y

CMSIS:=4.2.0
CROSS:="C:/Program Files (x86)""/GNU Tools ARM Embedded/7 2017-q4-major/bin/arm-none-eabi-"


MCUCC=$(CROSS)gcc
MCUCPP=$(CROSS)cpp
MCUAS=$(CROSS)gcc
MCUAR=$(CROSS)ar
MCULN=$(CROSS)ln
MCULD=$(CROSS)ld
MCUNM=$(CROSS)nm
MCUOBJCOPY=$(CROSS)objcopy
MCUOBJDUMP=$(CROSS)objdump
MCUSTRIP=$(CROSS)strip
MCUREF=$(CROSS)readelf -a
MCUDIS=$(CROSS)objdump --disassemble
MCUSIZE=$(CROSS)size 
MCUREADELF=$(CORSS)readelf

MCUCFLAGS:=-g -O0
#save all the .i,.s,.o files 
#MCUCFLAGS+=-save-temps 
MCUCFLAGS+=-fstack-usage  
MCUCFLAGS+=-mcpu=$(MCU)
MCUCFLAGS+=-mthumb
MCUCFLAGS+=-fsigned-char
MCUCFLAGS+=-ffunction-sections
MCUCFLAGS+=-fdata-sections
#MCUCFLAGS+=-ffreestanding
#MCUCFLAGS+=-Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wpadded -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal 
#MCUCFLAGS+=-Wmissing-prototypes  -Wbad-function-cast
#MCUCFLAGS+=-std=gnu11

MCUCMMFLAGS:=-MM $(MCUCFLAGS)
MCUASFLAGS=-mcpu=$(MCU) -mthumb

# Get the location of libc.a from the GCC front-end.
LIBC="$(shell $(MCUCC) -mcpu=$(MCU) -mthumb -print-file-name=libc.a)"
$(info "LIBC $(LIBC)")

# Get the location of libcgcc.a from the GCC front-end.
LIBGCC="$(shell $(MCUCC) -mcpu=$(MCU) -mthumb -print-libgcc-file-name)"
$(info "LIBGCC $(LIBGCC)")

# Get libm.a libnosys.a
LIBM=
LIBM="$(shell $(MCUCC) -mcpu=$(MCU) -mthumb -print-file-name=libm.a)"
$(info "LIBM $(LIBM)")

LIBNOSYS=
LIBNOSYS="$(shell $(MCUCC) -mcpu=$(MCU) -mthumb -print-file-name=libnosys.a)"
$(info "LIBNOSYS $(LIBNOSYS)")



MCULDFLAGS=--gc-sections
MCUARFLAGS=-rcs

clean-mcu-objs+=$(mcu-objs) $(rtos-objs) $(system-objs)



###
src:=.
dst:=./debug
$(shell mkdir -p $(dst) )
mem_ld:=$(src)/ldscripts/mem.ld
ram_ld:=$(src)/ldscripts/ram.ld
gcc_arm_ld:=$(src)/ldscripts/gcc_arm.ld
MCUCFLAGS+=-nostdlib
MCUCFLAGS+=$(STARTUP_DEFS)


CONFIG_USE_SELF_LD:=y
ifeq ($(CONFIG_USE_SELF_LD),y)
$(info "!!!Using self Link Scripts")
else 
$(info "!!!Using system default Link Scripts")
endif 


mcu_objs:=$(src)/startup_ARMCM0plus.o
mcu_objs+=$(src)/main.o
mcu_objs+=$(src)/fun.o
mcu_objs:=$(patsubst $(src)/%.o, $(BUILD_DIR)/%.o, $(mcu_objs))
#$(info "mcu_objs --> $(mcu_objs)")
mcu_src:=$(patsubst %.o,%.c,$(mcu_objs))
#$(info "mcu_src --> $(mcu_src)")
mcu_as:=$(patsubst %.o,%.s,$(mcu_objs))
mcu_dep:=$(patsubst %.o,%.dep,$(mcu_objs))

mcu_elf:=$(dst)/mcu.elf 
mcu_dis:=$(dst)/mcu.dis 
mcu_nm:=$(dst)/mcu.nm 
mcu_size:=$(dst)/mcu.size 
mcu_map:=$(dst)/mcu.elf.map 
mcu_bin:=$(dst)/mcu.bin
mcu_elf_rpt:=$(dst)/mcu.elf.rpt

build-dir:=$(sort $(dir $(mcu_objs)))

targets:=  $(build-dir) $(mcu_as) $(mcu_objs) $(mcu_elf)  $(mcu_dis) $(mcu_nm)  $(mcu_size) $(mcu_map) $(mcu_bin) $(mcu_elf_rpt) $(mcu_dep)

all: $(targets)

$(build-dir):
	@mkdir -p $@

#mcu rules 
#%.o::%.c
$(BUILD_DIR)/%.o::$(src)/%.c
	$(V)echo 'MCU CC -c $<'
	$(V)$(MCUCC) $(MCUCFLAGS) $(MCUEXTRA_CFLAGS) -MMD -MP -MF"$(@:%.o=%.dep)" -MT"$(@)" -c -o $@ $<

$(BUILD_DIR)/%.s::$(src)/%.c
	$(V)echo 'MCU CC -S $<'
	$(MCUCC) $(MCUCFLAGS) -S -o $@  $< 


$(BUILD_DIR)/%.dep::$(src)/%.c 

$(mcu_map):$(mcu_elf)

	
ifeq ($(CONFIG_USE_SELF_LD),y)
$(mcu_elf): $(mcu_objs) $(mem_ld) $(ram_ld)
	$(V)echo 'MCU LD  -o $@ $(notdir $(main_obj))'
	$(V)$(MCULD) $(MCULDFLAGS) -T $(mem_ld) -T $(ram_ld)  -Map $(mcu_map) -o $@ $(mcu_objs) $(LIBC) $(LIBGCC)
	#$(V)$(MCULD) $(MCULDFLAGS) -T $(gcc_arm_ld)  -Map $(mcu_map) -o $@ $(mcu_objs) $(LIBC) $(LIBGCC) $(LIBM)  $(LIBNOSYS)
else

$(mcu_elf) $(mcu_map): $(mcu_src)
	$(MCUCC) $(MCUCFLAGS) $(mcu_src) -Wl,-Map=$(mcu_map)  -o $(mcu_elf)  $(LIBC) $(LIBGCC)

endif 

$(mcu_dis): $(mcu_elf)
	$(V)$(MCUOBJDUMP) -D -S $< >$@
	
$(mcu_bin):  $(mcu_elf)
	$(V)$(MCUOBJCOPY) -v -S -g -O binary $<  $@
	
$(mcu_size): $(mcu_elf)
	$(V)$(MCUSIZE) -x $<  >$@
	
$(mcu_elf_rpt): $(mcu_elf)  
	$(V)$(MCUREADELF) -S $<  >$@
	
$(mcu_nm):$(mcu_elf)
	$(V)$(MCUNM) -S $<  >$@
	
clean: 
	rm -rf *.o *.i *.su  $(targets)

.PHONY=all clean 


