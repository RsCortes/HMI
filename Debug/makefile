################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include utilities/subdir.mk
-include startup/subdir.mk
-include source/GPIO/subdir.mk
-include source/subdir.mk
-include freertos/subdir.mk
-include drivers/subdir.mk
-include board/subdir.mk
-include CMSIS/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: ArqSwAutomotriz_HMI.axf

# Tool invocations
ArqSwAutomotriz_HMI.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -Xlinker -Map="ArqSwAutomotriz_HMI.map" -Xlinker --gc-sections -Xlinker -print-memory-usage -Xlinker --sort-section=alignment -mcpu=cortex-m0plus -mthumb -T ArqSwAutomotriz_HMI_Debug.ld -o "ArqSwAutomotriz_HMI.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) ArqSwAutomotriz_HMI.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "ArqSwAutomotriz_HMI.axf"; # arm-none-eabi-objcopy -v -O binary "ArqSwAutomotriz_HMI.axf" "ArqSwAutomotriz_HMI.bin" ; # checksum -p MKW36Z512xxx4 -d "ArqSwAutomotriz_HMI.bin";
	-@echo ' '

.PHONY: all clean dependents post-build

-include ../makefile.targets
