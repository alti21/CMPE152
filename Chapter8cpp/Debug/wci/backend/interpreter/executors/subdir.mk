################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../wci/backend/interpreter/executors/AssignmentExecutor.cpp \
../wci/backend/interpreter/executors/CompoundExecutor.cpp \
../wci/backend/interpreter/executors/ExpressionExecutor.cpp \
../wci/backend/interpreter/executors/IfExecutor.cpp \
../wci/backend/interpreter/executors/LoopExecutor.cpp \
../wci/backend/interpreter/executors/SelectExecutor.cpp \
../wci/backend/interpreter/executors/StatementExecutor.cpp \
../wci/backend/interpreter/executors/WhenExecutor.cpp 

OBJS += \
./wci/backend/interpreter/executors/AssignmentExecutor.o \
./wci/backend/interpreter/executors/CompoundExecutor.o \
./wci/backend/interpreter/executors/ExpressionExecutor.o \
./wci/backend/interpreter/executors/IfExecutor.o \
./wci/backend/interpreter/executors/LoopExecutor.o \
./wci/backend/interpreter/executors/SelectExecutor.o \
./wci/backend/interpreter/executors/StatementExecutor.o \
./wci/backend/interpreter/executors/WhenExecutor.o 

CPP_DEPS += \
./wci/backend/interpreter/executors/AssignmentExecutor.d \
./wci/backend/interpreter/executors/CompoundExecutor.d \
./wci/backend/interpreter/executors/ExpressionExecutor.d \
./wci/backend/interpreter/executors/IfExecutor.d \
./wci/backend/interpreter/executors/LoopExecutor.d \
./wci/backend/interpreter/executors/SelectExecutor.d \
./wci/backend/interpreter/executors/StatementExecutor.d \
./wci/backend/interpreter/executors/WhenExecutor.d 


# Each subdirectory must supply rules for building sources it contributes
wci/backend/interpreter/executors/%.o: ../wci/backend/interpreter/executors/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -I/usr/local/include/boost -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


