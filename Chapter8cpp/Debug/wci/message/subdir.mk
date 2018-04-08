################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../wci/message/Message.cpp \
../wci/message/MessageHandler.cpp 

OBJS += \
./wci/message/Message.o \
./wci/message/MessageHandler.o 

CPP_DEPS += \
./wci/message/Message.d \
./wci/message/MessageHandler.d 


# Each subdirectory must supply rules for building sources it contributes
wci/message/%.o: ../wci/message/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -I/usr/local/include/boost -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


