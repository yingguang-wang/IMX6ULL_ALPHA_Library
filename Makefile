CROSS_COMPILE 	?= arm-linux-gnueabihf-
TARGET		  	?= code

CC 				:= $(CROSS_COMPILE)gcc
LD				:= $(CROSS_COMPILE)ld
OBJCOPY 		:= $(CROSS_COMPILE)objcopy
OBJDUMP 		:= $(CROSS_COMPILE)objdump

LIBPATH			:= 	-lgcc -L /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/lib/gcc/arm-linux-gnueabihf/4.9.4
#所有头文件目录
INCDIRS 		:=	code \
					imx6ull_libraries/common \
					imx6ull_libraries/device \
					imx6ull_libraries/driver \
					imx6ull_libraries/MCIMX6Y2_libraries \
					imx6ull_libraries/MCIMX6Y2_libraries/stdio/include \
					user
					
#所有源码目录				   			   
SRCDIRS			:=	code \
					user \
					imx6ull_libraries/common \
					imx6ull_libraries/device \
					imx6ull_libraries/driver \
					imx6ull_libraries/MCIMX6Y2_libraries \
					imx6ull_libraries/MCIMX6Y2_libraries/stdio/lib \

				   				   
INCLUDE			:= $(patsubst %, -I %, $(INCDIRS))

SFILES			:= $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.S))
CFILES			:= $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.c))

SFILENDIR		:= $(notdir  $(SFILES))
CFILENDIR		:= $(notdir  $(CFILES))

SOBJS			:= $(patsubst %, Debug/%, $(SFILENDIR:.S=.o))
COBJS			:= $(patsubst %, Debug/%, $(CFILENDIR:.c=.o))
OBJS			:= $(SOBJS) $(COBJS)

VPATH			:= $(SRCDIRS)

.PHONY: clean
	
$(TARGET).bin : $(OBJS)
	$(LD) -Timx6ul.lds -o $(TARGET).elf $^  $(LIBPATH) 
	$(OBJCOPY) -O binary -S $(TARGET).elf $@
	$(OBJDUMP) -D -m arm $(TARGET).elf > $(TARGET).dis

$(SOBJS) : Debug/%.o : %.S
	$(CC) -Wall -nostdlib -fno-builtin -c -O2  $(INCLUDE) -o $@ $<

$(COBJS) : Debug/%.o : %.c
		$(CC) -Wall -march=armv7-a -mfpu=neon-vfpv4 -mfloat-abi=hard -Wa,-mimplicit-it=thumb -nostdlib -fno-builtin -c -O2  $(INCLUDE) -o $@ $<
	
clean:
	rm -rf $(TARGET).elf $(TARGET).dis $(TARGET).bin $(COBJS) $(SOBJS)

	
