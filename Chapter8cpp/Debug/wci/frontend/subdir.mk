################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../wci/frontend/FrontendFactory.cpp \
../wci/frontend/Parser.cpp \
../wci/frontend/Scanner.cpp \
../wci/frontend/Source.cpp \
../wci/frontend/Token.cpp 

OBJS += \
./wci/frontend/FrontendFactory.o \
./wci/frontend/Parser.o \
./wci/frontend/Scanner.o \
./wci/frontend/Source.o \
./wci/frontend/Token.o 

CPP_DEPS += \
./wci/frontend/FrontendFactory.d \
./wci/frontend/Parser.d \
./wci/frontend/Scanner.d \
./wci/frontend/Source.d \
./wci/frontend/Token.d 


# Each subdirectory must supply rules for building sources it contributes
wci/frontend/%.o: ../wci/frontend/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -I/usr/local/include/boost -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


