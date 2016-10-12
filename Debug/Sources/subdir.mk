################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/Events.c \
../Sources/main.c 

OBJS += \
./Sources/Events.o \
./Sources/main.o 

C_DEPS += \
./Sources/Events.d \
./Sources/main.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/%.o: ../Sources/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -I"/Applications/KDS_v3.app/Contents/eclipse/ProcessorExpert/lib/Kinetis/pdd/inc" -I"/Applications/KDS_v3.app/Contents/eclipse/ProcessorExpert/lib/Kinetis/iofiles" -I"/Users/gian/Documents/MyDocuments/Technical/Projects/Microcontrollers/Development/ReVarioExperimental/Sources" -I"/Users/gian/Documents/MyDocuments/Technical/Projects/Microcontrollers/Development/ReVarioExperimental/Generated_Code" -I"/Users/gian/Documents/MyDocuments/Technical/Projects/Microcontrollers/Development/ReVarioExperimental/Sources/FreeRTOS/Source/include" -I"/Users/gian/Documents/MyDocuments/Technical/Projects/Microcontrollers/Development/ReVarioExperimental/Sources/FreeRTOS/Source/portable/GCC/ARM_CM3" -I"/Users/gian/Documents/MyDocuments/Technical/Projects/Microcontrollers/Development/ReVarioExperimental/Sources/Drivers" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


