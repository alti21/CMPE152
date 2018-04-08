################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../wci/frontend/pascal/tokens/PascalErrorToken.cpp \
../wci/frontend/pascal/tokens/PascalNumberToken.cpp \
../wci/frontend/pascal/tokens/PascalSpecialSymbolToken.cpp \
../wci/frontend/pascal/tokens/PascalStringToken.cpp \
../wci/frontend/pascal/tokens/PascalWordToken.cpp 

OBJS += \
./wci/frontend/pascal/tokens/PascalErrorToken.o \
./wci/frontend/pascal/tokens/PascalNumberToken.o \
./wci/frontend/pascal/tokens/PascalSpecialSymbolToken.o \
./wci/frontend/pascal/tokens/PascalStringToken.o \
./wci/frontend/pascal/tokens/PascalWordToken.o 

CPP_DEPS += \
./wci/frontend/pascal/tokens/PascalErrorToken.d \
./wci/frontend/pascal/tokens/PascalNumberToken.d \
./wci/frontend/pascal/tokens/PascalSpecialSymbolToken.d \
./wci/frontend/pascal/tokens/PascalStringToken.d \
./wci/frontend/pascal/tokens/PascalWordToken.d 


# Each subdirectory must supply rules for building sources it contributes
wci/frontend/pascal/tokens/%.o: ../wci/frontend/pascal/tokens/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -I/usr/local/include/boost -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


