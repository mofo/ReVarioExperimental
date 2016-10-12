################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/FreeRTOS/Source/croutine.c \
../Sources/FreeRTOS/Source/event_groups.c \
../Sources/FreeRTOS/Source/list.c \
../Sources/FreeRTOS/Source/queue.c \
../Sources/FreeRTOS/Source/tasks.c \
../Sources/FreeRTOS/Source/timers.c 

OBJS += \
./Sources/FreeRTOS/Source/croutine.o \
./Sources/FreeRTOS/Source/event_groups.o \
./Sources/FreeRTOS/Source/list.o \
./Sources/FreeRTOS/Source/queue.o \
./Sources/FreeRTOS/Source/tasks.o \
./Sources/FreeRTOS/Source/timers.o 

C_DEPS += \
./Sources/FreeRTOS/Source/croutine.d \
./Sources/FreeRTOS/Source/event_groups.d \
./Sources/FreeRTOS/Source/list.d \
./Sources/FreeRTOS/Source/queue.d \
./Sources/FreeRTOS/Source/tasks.d \
./Sources/FreeRTOS/Source/timers.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/FreeRTOS/Source/%.o: ../Sources/FreeRTOS/Source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -I"/Applications/KDS_v3.app/Contents/eclipse/ProcessorExpert/lib/Kinetis/pdd/inc" -I"/Applications/KDS_v3.app/Contents/eclipse/ProcessorExpert/lib/Kinetis/iofiles" -I"/Users/gian/Documents/MyDocuments/Technical/Projects/Microcontrollers/Development/ReVarioExperimental/Sources" -I"/Users/gian/Documents/MyDocuments/Technical/Projects/Microcontrollers/Development/ReVarioExperimental/Generated_Code" -I"/Users/gian/Documents/MyDocuments/Technical/Projects/Microcontrollers/Development/ReVarioExperimental/Sources/FreeRTOS/Source/include" -I"/Users/gian/Documents/MyDocuments/Technical/Projects/Microcontrollers/Development/ReVarioExperimental/Sources/FreeRTOS/Source/portable/GCC/ARM_CM3" -I"/Users/gian/Documents/MyDocuments/Technical/Projects/Microcontrollers/Development/ReVarioExperimental/Sources/Drivers" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


