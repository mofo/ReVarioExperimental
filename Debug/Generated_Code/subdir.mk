################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Generated_Code/AS0.c \
../Generated_Code/AS1.c \
../Generated_Code/ASerialLdd1.c \
../Generated_Code/ASerialLdd2.c \
../Generated_Code/CI2C0.c \
../Generated_Code/Cpu.c \
../Generated_Code/IntI2cLdd1.c \
../Generated_Code/PE_LDD.c \
../Generated_Code/SM1.c \
../Generated_Code/SMasterLdd1.c \
../Generated_Code/Vectors.c 

OBJS += \
./Generated_Code/AS0.o \
./Generated_Code/AS1.o \
./Generated_Code/ASerialLdd1.o \
./Generated_Code/ASerialLdd2.o \
./Generated_Code/CI2C0.o \
./Generated_Code/Cpu.o \
./Generated_Code/IntI2cLdd1.o \
./Generated_Code/PE_LDD.o \
./Generated_Code/SM1.o \
./Generated_Code/SMasterLdd1.o \
./Generated_Code/Vectors.o 

C_DEPS += \
./Generated_Code/AS0.d \
./Generated_Code/AS1.d \
./Generated_Code/ASerialLdd1.d \
./Generated_Code/ASerialLdd2.d \
./Generated_Code/CI2C0.d \
./Generated_Code/Cpu.d \
./Generated_Code/IntI2cLdd1.d \
./Generated_Code/PE_LDD.d \
./Generated_Code/SM1.d \
./Generated_Code/SMasterLdd1.d \
./Generated_Code/Vectors.d 


# Each subdirectory must supply rules for building sources it contributes
Generated_Code/%.o: ../Generated_Code/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -I"/Users/gian/Applications/eclipse kepler/ProcessorExpert/lib/Kinetis/pdd/inc" -I"/Users/gian/Applications/eclipse kepler/ProcessorExpert/lib/Kinetis/iofiles" -I"/Users/gian/Projects/Microcontrollers/Development/ReVarioExperimental/Sources" -I"/Users/gian/Projects/Microcontrollers/Development/ReVarioExperimental/Generated_Code" -I"/Users/gian/Projects/Microcontrollers/Development/ReVarioExperimental/Sources/FreeRTOS/Source/include" -I"/Users/gian/Projects/Microcontrollers/Development/ReVarioExperimental/Sources/FreeRTOS/Source/portable/GCC/ARM_CM3" -I"/Users/gian/Projects/Microcontrollers/Development/ReVarioExperimental/Sources/Drivers" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


