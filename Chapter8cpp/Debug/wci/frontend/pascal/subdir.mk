################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../wci/frontend/pascal/PascalError.cpp \
../wci/frontend/pascal/PascalErrorHandler.cpp \
../wci/frontend/pascal/PascalParserTD.cpp \
../wci/frontend/pascal/PascalScanner.cpp \
../wci/frontend/pascal/PascalToken.cpp 

OBJS += \
./wci/frontend/pascal/PascalError.o \
./wci/frontend/pascal/PascalErrorHandler.o \
./wci/frontend/pascal/PascalParserTD.o \
./wci/frontend/pascal/PascalScanner.o \
./wci/frontend/pascal/PascalToken.o 

CPP_DEPS += \
./wci/frontend/pascal/PascalError.d \
./wci/frontend/pascal/PascalErrorHandler.d \
./wci/frontend/pascal/PascalParserTD.d \
./wci/frontend/pascal/PascalScanner.d \
./wci/frontend/pascal/PascalToken.d 


# Each subdirectory must supply rules for building sources it contributes
wci/frontend/pascal/%.o: ../wci/frontend/pascal/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -I/usr/local/include/boost -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


