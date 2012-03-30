ifndef config
    config=debug
endif
export config

.PHONY: all make clean unit-tests

PREMAKE:=../config/premake4/premake4-$(shell uname -m)

BUILD_DIR=build
LIB_DIR=${BUILD_DIR}

EXECUTABLE=${BUILD_DIR}/GeconPC

SET_LIB_PATH=LD_LIBRARY_PATH=${LIB_DIR}

ifeq ($(config),debug)
    GDB=gdb -ex 'r'
endif

all: build make

build: premake4.lua src/premake4.lua
	@rm -rf build
	@mkdir -p build
	@cd build && ${PREMAKE} --file=../premake4.lua --to gmake

make:
	@cd build && ${MAKE} config=$(config)

run: 
	-@${SET_LIB_PATH} ${GDB} ${EXECUTABLE}

clean:
	@echo "Cleaning..."
	@rm -rf build
	@echo "Done."
