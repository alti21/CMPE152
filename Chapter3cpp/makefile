#
# Makefile for Chapter 3
#
# Copyright (c) 2017 by Ronald Mak
# For instructional purposes only.  No warranties.
#
COMPILER = Chapter3cpp
SRCS =  Java.cpp \
        wci/Globals.cpp \
        wci/backend/Backend.cpp \
        wci/backend/BackendFactory.cpp \
        wci/backend/compiler/CodeGenerator.cpp \
        wci/backend/interpreter/Executor.cpp \
        wci/frontend/FrontendFactory.cpp \
        wci/frontend/Parser.cpp \
        wci/frontend/Scanner.cpp \
        wci/frontend/Source.cpp \
        wci/frontend/Token.cpp \
        wci/frontend/java/JavaError.cpp \
        wci/frontend/java/JavaErrorHandler.cpp \
        wci/frontend/java/JavaParserTD.cpp \
        wci/frontend/java/JavaScanner.cpp \
        wci/frontend/java/JavaToken.cpp \
        wci/frontend/java/tokens/JavaErrorToken.cpp \
        wci/frontend/java/tokens/JavaNumberToken.cpp \
        wci/frontend/java/tokens/JavaSpecialSymbolToken.cpp \
        wci/frontend/java/tokens/JavaStringToken.cpp \
        wci/frontend/java/tokens/JavaWordToken.cpp \
        wci/message/Message.cpp \
        wci/message/MessageHandler.cpp
        
OBJS = $(SRCS:.cpp=.o)
CC = g++

# Replace with the path on your machine to the directory
# where you installed the Boost include files.
# See http://www.boost.org
BOOST_INCLUDE_DIR = /usr/local/include/boost

CFLAGS = -std=c++0x -O0 -g3 -Wall -I$(BOOST_INCLUDE_DIR)

.cpp.o :
	$(CC) $(CFLAGS) -c -o $@ $<

# Used to generate the header file dependencies.
# Compile utility program 'depends' from depends.cpp:
# g++ -o depends depends.cpp
DEPENDS_UTILITY = depends
RAW_DEPENDENCY_FILE = raw_dependencies.txt
DEPENDENCY_FILE = makefile_dependencies.txt

PFLAGS =

all: $(COMPILER)

clean:
	rm $(OBJS) $(COMPILER) $(RAW_DEPENDENCY_FILE)

$(COMPILER) : $(OBJS)
	$(CC) -o $(COMPILER) $(OBJS)

# Interpret a Java .pas program.
# Example: make interpret src=hello.pas
# runs the hello.pas program using the interpreter.
interpret: $(COMPILER) $(src)
	./$(COMPILER) execute $(PFLAGS) $(src)

# Compile a Java .pas program.
# Example: make compile src=hello.pas
# compiles the hello.pas program using the compiler.
compile: $(COMPILER)
	./$(COMPILER) compile $(src)

# Generate dependencies on the header files.
# You shouldn't need to run this unless
# you added new .cpp or .h files.
dependencies:
	$(CC) -MM $(SRCS) > $(RAW_DEPENDENCY_FILE)
	$(DEP) < $(RAW_DEPENDENCY_FILE) > $(DEPENDENCY_FILE)
	rm $(RAW_DEPENDENCY_FILE)

sinclude $(DEPENDENCY_FILE)
