################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/biblioteka_ipv6.c \
../src/biblioteka_tcp.c \
../src/ipv6_proj.c 

OBJS += \
./src/biblioteka_ipv6.o \
./src/biblioteka_tcp.o \
./src/ipv6_proj.o 

C_DEPS += \
./src/biblioteka_ipv6.d \
./src/biblioteka_tcp.d \
./src/ipv6_proj.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


