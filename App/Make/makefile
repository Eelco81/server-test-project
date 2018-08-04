# create an executable

SOURCEDIR       = ./../Src
LIBDIR          = ./../../Lib
TOOLDIR         = ./../../Tools
SOURCES         = $(wildcard $(SOURCEDIR)/*.cpp)
DEPENDENCIES    = AppUtils TcpServer Core
LINKS           = -lpthread
TARGET          = serveraplication

-include $(TOOLDIR)/build.mk# DO NOT DELETE

integration-test : 
	cd ../Test; behave