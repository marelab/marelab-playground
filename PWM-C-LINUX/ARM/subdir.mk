################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../I2Cdev.o \
../PCA9685.o \
../main.o 

CPP_SRCS += \
../I2Cdev.cpp \
../PCA9685.cpp \
../main.cpp 

OBJS += \
./I2Cdev.o \
./PCA9685.o \
./main.o 

CPP_DEPS += \
./I2Cdev.d \
./PCA9685.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


