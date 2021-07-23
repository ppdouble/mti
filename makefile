-include ../makefile.init

CC := gcc
RM := rm -rf
CP := cp
# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
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
all: libmti.so

# Tool invocations
libmti.so: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	$(CC)  -shared -o "libmti.so" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo 'Finished make.'
	@echo ' '

# Other Targets
install:
	-$(CP) libmti.so /usr/lib
	-$(CP) ./include/mti.h /usr/include/mti.h
	-@echo 'Finished make install.'
	-@echo ' '
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) libmti.so
	-@echo ' '


.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
