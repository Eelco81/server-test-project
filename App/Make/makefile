# create an executable

EXTERNAL        = ./../../External/nlohman
SOURCEDIR       = ./../Src
LIBDIR          = ./../../Lib
TOOLDIR         = ./../../Tools
SOURCES         = $(wildcard $(SOURCEDIR)/*.cpp)
DEPENDENCIES    = Websockets Router Http TcpServer SimComponents Simulation AppUtils Core
LINKS           = -lpthread
INCLUDES        = -I$(EXTERNAL)
TARGET          = serveraplication

-include $(TOOLDIR)/build.mk # DO NOT DELETE

integration-test : 
	cd ../Test; behave