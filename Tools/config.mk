
ROOT_DIR?=.

INSTALL_DIR=${ROOT_DIR}/Install
DOWNLOAD_DIR=${ROOT_DIR}/Download
EXTERNAL_DIR=${ROOT_DIR}/External
APP_DIR=${ROOT_DIR}/App
WEB_DIR=${ROOT_DIR}/Web
LIB_DIR=${ROOT_DIR}/Lib
TEST_DIR=${ROOT_DIR}/Test

ifeq ($(OS),Windows_NT)
	OS_FAMILY = WINDOWS
	UNAME := $(shell uname -s)
	ifeq ($(UNAME),CYGIN_NT-6.1)
		OS_COMPILER = CYGWIN
	endif
else 
	UNAME := $(shell uname -s)
	ifeq ($(UNAME),Linux)
		OS_FAMILY = LINUX
		OS_COMPILER = GCC
	endif
endif
