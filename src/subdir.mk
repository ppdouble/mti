# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
./src/mti.c \
./src/mtiimpl.c \
./src/mtithread.c \
./src/util.c 

OBJS += \
./mti.o \
./mtiimpl.o \
./mtithread.o \
./util.o 

C_DEPS += \
./mti.d \
./mtiimpl.d \
./mtithread.d \
./util.d 


# Each subdirectory must supply rules for building sources it contributes
./%.o: ./src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"include" -O3 -Wall -fpic -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


