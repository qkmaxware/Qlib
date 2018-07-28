#-------------------
#Compiler settings
#-------------------
GCC	= g++
COMPILE_FLAGS = -g -std=c++17 -static

#-------------------
#File/Folder settings
#-------------------
BIN_DIR	= bin
SRC_DIR = src
LIB_DIR = lib
OBJ_DIR = obj

LINUX_EXE_NAME = run.linux
LINUX32_EXE_NAME = run.l32
LINUX64_EXE_NAME = run.l64
DEB32_EXE_NAME = run.l32
DEB64_EXE_NAME = run.l64
WIN32_EXE_NAME = run.w32
WIN64_EXE_NAME = run.w64

PATH_SEPARATOR = /

#-------------------
#Source files
#-------------------
CPP_SRC = $(wildcard $(SRC_DIR)$(PATH_SEPARATOR)*.cpp)
C_SRC = $(wildcard $(SRC_DIR)$(PATH_SEPARATOR)*.c)
RUNTIME_SRC = $(wildcard $(SRC_DIR)$(PATH_SEPARATOR)runtime$(PATH_SEPARATOR)*.cpp)

SOURCES = $(CPP_SRC) $(C_SRC) $(RUNTIME_SRC)

#-------------------
#Evaluation Modifyable Parameters
#-------------------
ADDITIONAL_FLAGS = 
EXE_NAME = $(LINUX_EXE_NAME)

#-------------------
#Rules
#-------------------
TOCOMPILE = $(SOURCES:.cpp=.o)
BASENAMES = $(subst $(PATH_SEPARATOR),., $(SOURCES))
LINKLIST = $(patsubst %.cpp, $(OBJ_DIR)$(PATH_SEPARATOR)%.o, $(BASENAMES))

.cpp.o:
	$(GCC) $(COMPILE_FLAGS) $(ADDITIONAL_FLAGS) -c $< -o $(OBJ_DIR)$(PATH_SEPARATOR)$(subst $(PATH_SEPARATOR),.,$@) 

#-------------------
#Targets
#-------------------

build: compile $(BIN_DIR)

deb: build
	rm -rf $(BIN_DIR)$(PATH_SEPARATOR)deb_$(EXE_NAME) || true
	mkdir -p $(BIN_DIR)$(PATH_SEPARATOR)deb_$(EXE_NAME)
	mkdir -p $(BIN_DIR)$(PATH_SEPARATOR)deb_$(EXE_NAME)$(PATH_SEPARATOR)DEBIAN
	mkdir -p $(BIN_DIR)$(PATH_SEPARATOR)deb_$(EXE_NAME)$(PATH_SEPARATOR)usr$(PATH_SEPARATOR)local$(PATH_SEPARATOR)bin
	chmod -R 0775 $(BIN_DIR)$(PATH_SEPARATOR)deb_$(EXE_NAME)
	cp $(BIN_DIR)$(PATH_SEPARATOR)$(EXE_NAME) $(BIN_DIR)$(PATH_SEPARATOR)deb_$(EXE_NAME)$(PATH_SEPARATOR)usr$(PATH_SEPARATOR)local$(PATH_SEPARATOR)bin
	
	touch $(BIN_DIR)$(PATH_SEPARATOR)deb_$(EXE_NAME)$(PATH_SEPARATOR)DEBIAN$(PATH_SEPARATOR)control 
	echo "Package: $(EXE_NAME)" >> $(BIN_DIR)$(PATH_SEPARATOR)deb_$(EXE_NAME)$(PATH_SEPARATOR)DEBIAN$(PATH_SEPARATOR)control
	echo "Version: 1" >> $(BIN_DIR)$(PATH_SEPARATOR)deb_$(EXE_NAME)$(PATH_SEPARATOR)DEBIAN$(PATH_SEPARATOR)control
	echo "Maintainer: self" >> $(BIN_DIR)$(PATH_SEPARATOR)deb_$(EXE_NAME)$(PATH_SEPARATOR)DEBIAN$(PATH_SEPARATOR)control
	echo "Architecture: all" >> $(BIN_DIR)$(PATH_SEPARATOR)deb_$(EXE_NAME)$(PATH_SEPARATOR)DEBIAN$(PATH_SEPARATOR)control
	echo "Description: my app" >> $(BIN_DIR)$(PATH_SEPARATOR)deb_$(EXE_NAME)$(PATH_SEPARATOR)DEBIAN$(PATH_SEPARATOR)control
	
	touch $(BIN_DIR)$(PATH_SEPARATOR)deb_$(EXE_NAME)$(PATH_SEPARATOR)DEBIAN$(PATH_SEPARATOR)postinst
	chmod 0775 $(BIN_DIR)$(PATH_SEPARATOR)deb_$(EXE_NAME)$(PATH_SEPARATOR)DEBIAN$(PATH_SEPARATOR)postinst
	echo "export PATH=$$PATH:/usr/local/bin" >> $(BIN_DIR)$(PATH_SEPARATOR)deb_$(EXE_NAME)$(PATH_SEPARATOR)DEBIAN$(PATH_SEPARATOR)postinst

	dpkg-deb --build $(BIN_DIR)$(PATH_SEPARATOR)deb_$(EXE_NAME)
	rm -rf $(BIN_DIR)$(PATH_SEPARATOR)deb_$(EXE_NAME)

build-linux: 
	make build EXE_NAME=$(LINUX32_EXE_NAME) ADDITIONAL_FLAGS=-m32
	make build EXE_NAME=$(LINUX64_EXE_NAME) ADDITIONAL_FLAGS=-m64

build-win:
	make build GCC=i686-w64-mingw32-g++ EXE_NAME=$(WIN32_EXE_NAME).exe
	make build GCC=x86_64-w64-mingw32-g++ EXE_NAME=$(WIN64_EXE_NAME).exe

build-deb:
	make deb EXE_NAME=$(DEB32_EXE_NAME) ADDITIONAL_FLAGS=-m32
	make deb EXE_NAME=$(DEB64_EXE_NAME) ADDITIONAL_FLAGS=-m64

build-all: build build-linux build-win

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
	$(GCC) $(LINKLIST) -o $(BIN_DIR)/$(EXE_NAME) $(COMPILE_FLAGS) $(ADDITIONAL_FLAGS)

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

install-linux-tools:
	sudo apt-get install build-essential gcc-multilib g++-multilib

install-doxygen:
	sudo apt-get install graphviz
	sudo apt-get install doxygen
	sudo apt-get install doxygen-gui

compress:
	zip -r compressed.zip .