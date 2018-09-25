# Make a static library

EXTERNAL        = ./../../../External/nlohman
SOURCEDIR       = ./../Src
LIBDIR          = ./../..
TOOLDIR         = ./../../../Tools/
SOURCES         = $(wildcard $(SOURCEDIR)/*.cpp)
DEPENDENCIES    = Http TcpServer AppUtils Core 
LINKS           = -lpthread
INCLUDES        = -I$(EXTERNAL)
LIBTARGET       = libwebsocket.a

-include $(TOOLDIR)/build.mk