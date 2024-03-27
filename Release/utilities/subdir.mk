################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../utilities/fsl_debug_console.c 

C_DEPS += \
./utilities/fsl_debug_console.d 

OBJS += \
./utilities/fsl_debug_console.o 


# Each subdirectory must supply rules for building sources it contributes
utilities/%.o: ../utilities/%.c utilities/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MKL25Z128VLK4_cm0plus -DCPU_MKL25Z128VLK4 -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSDK_DEBUGCONSOLE_UART -DSDK_OS_FREE_RTOS -DFSL_RTOS_FREE_RTOS -D__MCUXPRESSO -D__USE_CMSIS -DNDEBUG -D__REDLIB__ -I"C:\Users\jithe\Documents\MCUXpressoIDE_11.8.0_1165\workspace\LIDAR_park_assist_PES\board" -I"C:\Users\jithe\Documents\MCUXpressoIDE_11.8.0_1165\workspace\LIDAR_park_assist_PES\source" -I"C:\Users\jithe\Documents\MCUXpressoIDE_11.8.0_1165\workspace\LIDAR_park_assist_PES" -I"C:\Users\jithe\Documents\MCUXpressoIDE_11.8.0_1165\workspace\LIDAR_park_assist_PES\freertos" -I"C:\Users\jithe\Documents\MCUXpressoIDE_11.8.0_1165\workspace\LIDAR_park_assist_PES\drivers" -I"C:\Users\jithe\Documents\MCUXpressoIDE_11.8.0_1165\workspace\LIDAR_park_assist_PES\CMSIS" -I"C:\Users\jithe\Documents\MCUXpressoIDE_11.8.0_1165\workspace\LIDAR_park_assist_PES\utilities" -I"C:\Users\jithe\Documents\MCUXpressoIDE_11.8.0_1165\workspace\LIDAR_park_assist_PES\startup" -Os -fno-common -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-utilities

clean-utilities:
	-$(RM) ./utilities/fsl_debug_console.d ./utilities/fsl_debug_console.o

.PHONY: clean-utilities

