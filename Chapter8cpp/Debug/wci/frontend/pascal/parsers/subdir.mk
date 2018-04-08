################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../wci/frontend/pascal/parsers/AssignmentStatementParser.cpp \
../wci/frontend/pascal/parsers/CaseStatementParser.cpp \
../wci/frontend/pascal/parsers/CompoundStatementParser.cpp \
../wci/frontend/pascal/parsers/ExpressionParser.cpp \
../wci/frontend/pascal/parsers/ForStatementParser.cpp \
../wci/frontend/pascal/parsers/IfStatementParser.cpp \
../wci/frontend/pascal/parsers/RepeatStatementParser.cpp \
../wci/frontend/pascal/parsers/StatementParser.cpp \
../wci/frontend/pascal/parsers/WhenStatementParser.cpp \
../wci/frontend/pascal/parsers/WhileStatementParser.cpp 

OBJS += \
./wci/frontend/pascal/parsers/AssignmentStatementParser.o \
./wci/frontend/pascal/parsers/CaseStatementParser.o \
./wci/frontend/pascal/parsers/CompoundStatementParser.o \
./wci/frontend/pascal/parsers/ExpressionParser.o \
./wci/frontend/pascal/parsers/ForStatementParser.o \
./wci/frontend/pascal/parsers/IfStatementParser.o \
./wci/frontend/pascal/parsers/RepeatStatementParser.o \
./wci/frontend/pascal/parsers/StatementParser.o \
./wci/frontend/pascal/parsers/WhenStatementParser.o \
./wci/frontend/pascal/parsers/WhileStatementParser.o 

CPP_DEPS += \
./wci/frontend/pascal/parsers/AssignmentStatementParser.d \
./wci/frontend/pascal/parsers/CaseStatementParser.d \
./wci/frontend/pascal/parsers/CompoundStatementParser.d \
./wci/frontend/pascal/parsers/ExpressionParser.d \
./wci/frontend/pascal/parsers/ForStatementParser.d \
./wci/frontend/pascal/parsers/IfStatementParser.d \
./wci/frontend/pascal/parsers/RepeatStatementParser.d \
./wci/frontend/pascal/parsers/StatementParser.d \
./wci/frontend/pascal/parsers/WhenStatementParser.d \
./wci/frontend/pascal/parsers/WhileStatementParser.d 


# Each subdirectory must supply rules for building sources it contributes
wci/frontend/pascal/parsers/%.o: ../wci/frontend/pascal/parsers/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -I/usr/local/include/boost -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


