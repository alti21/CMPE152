################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include wci/util/subdir.mk
-include wci/message/subdir.mk
-include wci/intermediate/symtabimpl/subdir.mk
-include wci/intermediate/icodeimpl/subdir.mk
-include wci/intermediate/subdir.mk
-include wci/frontend/pascal/tokens/subdir.mk
-include wci/frontend/pascal/parsers/subdir.mk
-include wci/frontend/pascal/subdir.mk
-include wci/frontend/subdir.mk
-include wci/backend/interpreter/executors/subdir.mk
-include wci/backend/interpreter/subdir.mk
-include wci/backend/compiler/subdir.mk
-include wci/backend/subdir.mk
-include wci/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: Chapter8cpp

# Tool invocations
Chapter8cpp: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MacOS X C++ Linker'
	g++  -o "Chapter8cpp" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(EXECUTABLES)$(OBJS)$(C_UPPER_DEPS)$(CXX_DEPS)$(CPP_DEPS)$(C_DEPS) Chapter8cpp
	-@echo ' '

.PHONY: all clean dependents

-include ../makefile.targets
