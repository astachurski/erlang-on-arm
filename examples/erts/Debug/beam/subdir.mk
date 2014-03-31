################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../beam/atom.c \
../beam/beam_emu.c \
../beam/beam_load.c \
../beam/erl_init.c \
../beam/erl_process.c \
../beam/hash.c \
../beam/index.c 

OBJS += \
./beam/atom.o \
./beam/beam_emu.o \
./beam/beam_load.o \
./beam/erl_init.o \
./beam/erl_process.o \
./beam/hash.o \
./beam/index.o 

C_DEPS += \
./beam/atom.d \
./beam/beam_emu.d \
./beam/beam_load.d \
./beam/erl_init.d \
./beam/erl_process.d \
./beam/hash.d \
./beam/index.d 


# Each subdirectory must supply rules for building sources it contributes
beam/%.o: ../beam/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__USE_CMSIS=CMSISv1p30_LPC17xx -D__CODE_RED -D__REDLIB__ -I"/Users/Studnicki/work/mgr/examples/FreeRTOS_Library/demo_code" -I"/Users/Studnicki/work/mgr/examples/CMSISv1p30_LPC17xx/inc" -I"/Users/Studnicki/work/mgr/examples/FreeRTOS_Library/include" -I"/Users/Studnicki/work/mgr/examples/FreeRTOS_Library/portable" -O1 -g3 -fsigned-char -c -fmessage-length=0 -fno-builtin -ffunction-sections -mcpu=cortex-m3 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

