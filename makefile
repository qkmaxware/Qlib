#-------------------
#Compiler settings
#-------------------
GCC	= g++
COMPILE_FLAGS = -g

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

SOURCES = $(SRC_SRC)

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
compress:
	zip -r compressed.zip .

build: compile $(BIN_DIR)

run: 
	.$(PATH_SEPARATOR)$(BIN_DIR)$(PATH_SEPARATOR)$(EXE_NAME)

compile: mkdir $(TOCOMPILE)

clean:
	rm -f $(LINKLIST) || true
	rm -rf $(BIN_DIR)$(PATH_SEPARATOR)$(EXE_NAME) ||true

mkdir: 
	mkdir bin -p
	mkdir lib -p
	mkdir obj -p
	mkdir src -p

get: clean mkdir
	rm -r $(LIB_DIR) || true
	mkdir lib

$(BIN_DIR): mkdir $(TOCOMPILE)
	$(GCC) $(LINKLIST) -o $(BIN_DIR)/$(EXE_NAME) -std=c++0x

#-------------------
#Compound Targets
#-------------------
build-run: build run

clean-build: clean build

all: clean build run

install-run: mkdir get all
