#-------------------
#Compiler settings
#-------------------
GCC	= g++
COMPILE_FLAGS = -g -std=c++17

#-------------------
#File/Folder settings
#-------------------
BIN_DIR	= bin
SRC_DIR = src
LIB_DIR = lib
OBJ_DIR = obj

EXE_NAME = run

PATH_SEPARATOR = /

#-------------------
#Source files
#-------------------
SRC_SRC = $(wildcard $(SRC_DIR)$(PATH_SEPARATOR)*.cpp)
RUNTIME_SRC = $(wildcard $(SRC_DIR)$(PATH_SEPARATOR)runtime$(PATH_SEPARATOR)*.cpp)

SOURCES = $(SRC_SRC) $(RUNTIME_SRC)

#-------------------
#Rules
#-------------------
TOCOMPILE = $(SOURCES:.cpp=.o)
BASENAMES = $(subst $(PATH_SEPARATOR),., $(SOURCES))
LINKLIST = $(patsubst %.cpp, $(OBJ_DIR)$(PATH_SEPARATOR)%.o, $(BASENAMES))

.cpp.o:
	$(GCC) $(COMPILE_FLAGS) -c $< -o $(OBJ_DIR)$(PATH_SEPARATOR)$(subst $(PATH_SEPARATOR),.,$@) 

#-------------------
#Targets
#-------------------

build: compile $(BIN_DIR)

build-win:
	make build GCC=i686-w64-mingw32-g++ EXE_NAME=$(EXE_NAME).w32.exe
	make build GCC=x86_64-w64-mingw32-g++ EXE_NAME=$(EXE_NAME).w64.exe

run: 
	.$(PATH_SEPARATOR)$(BIN_DIR)$(PATH_SEPARATOR)$(EXE_NAME)

compile: init $(TOCOMPILE)

clean:
	rm -f $(LINKLIST) || true
	rm -rf $(BIN_DIR)$(PATH_SEPARATOR)$(EXE_NAME) ||true

init: 
	mkdir bin -p
	mkdir lib -p
	mkdir obj -p
	mkdir src -p
	mkdir doc -p

get: clean init
	rm -r $(LIB_DIR) || true
	mkdir lib

docs: 
	doxywizard doxyfile

$(BIN_DIR): init $(TOCOMPILE)
	$(GCC) $(LINKLIST) -o $(BIN_DIR)/$(EXE_NAME) $(COMPILE_FLAGS)

#-------------------
#Compound Targets
#-------------------
build-run: build run

clean-build: clean build

all: clean build run

install-run: init get all

#--------------------
#Utilities
#--------------------
install-win-tools:
	sudo apt-get remove mingw-w64
	sudo apt-get autoremove
	sudo apt-get update
	sudo apt-cache search mingw | grep "Win32"
	sudo apt-cache search mingw | grep "Win64"
	sudo apt-get install mingw-w64 binutils-mingw-w64

install-doxygen:
	sudo apt-get install graphviz
	sudo apt-get install doxygen
	sudo apt-get install doxygen-gui

compress:
	zip -r compressed.zip .