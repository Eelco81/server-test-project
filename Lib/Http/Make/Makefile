# Make a static library

EXTERNAL        = ./../../../External/nlohman
SOURCEDIR       = ./../Src
LIBDIR          = ./../..
TOOLDIR         = ./../../../Tools/
SOURCES         = $(wildcard $(SOURCEDIR)/*.cpp)
DEPENDENCIES    = TcpServer AppUtils Core 
LINKS           = -lpthread
INCLUDES        = -I$(EXTERNAL)
LIBTARGET       = libhttp.a

-include $(TOOLDIR)/build.mk