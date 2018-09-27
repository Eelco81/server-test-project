# Make a static library

EXTERNAL        = ./../../../External/sha1
SOURCEDIR       = ./../Src
LIBDIR          = ./../..
TOOLDIR         = ./../../../Tools/
SOURCES         = $(wildcard $(SOURCEDIR)/*.cpp)
DEPENDENCIES    = Http TcpServer AppUtils Core 
LINKS           = -lpthread
INCLUDES        = -I$(EXTERNAL)
LIBTARGET       = libwebsockets.a

-include $(TOOLDIR)/build.mk