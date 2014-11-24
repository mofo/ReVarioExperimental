################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/FreeRTOS/Source/portable/MemMang/heap_2.c 

OBJS += \
./Sources/FreeRTOS/Source/portable/MemMang/heap_2.o 

C_DEPS += \
./Sources/FreeRTOS/Source/portable/MemMang/heap_2.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/FreeRTOS/Source/portable/MemMang/%.o: ../Sources/FreeRTOS/Source/portable/MemMang/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -I"/Users/gian/Applications/eclipse kepler/ProcessorExpert/lib/Kinetis/pdd/inc" -I"/Users/gian/Applications/eclipse kepler/ProcessorExpert/lib/Kinetis/iofiles" -I"/Users/gian/Projects/Microcontrollers/Development/ReVarioExperimental/Sources" -I"/Users/gian/Projects/Microcontrollers/Development/ReVarioExperimental/Generated_Code" -I"/Users/gian/Projects/Microcontrollers/Development/ReVarioExperimental/Sources/FreeRTOS/Source/include" -I"/Users/gian/Projects/Microcontrollers/Development/ReVarioExperimental/Sources/FreeRTOS/Source/portable/GCC/ARM_CM3" -I"/Users/gian/Projects/Microcontrollers/Development/ReVarioExperimental/Sources/Drivers" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


