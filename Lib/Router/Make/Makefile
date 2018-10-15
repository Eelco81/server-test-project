# Make a static library

EXTERNAL        = ./../../../External/nlohman
SOURCEDIR       = ./../Src
LIBDIR          = ./../..
TOOLDIR         = ./../../../Tools/
SOURCES         = $(wildcard $(SOURCEDIR)/*.cpp)
DEPENDENCIES    = Http TcpServer Simulation AppUtils Core 
LINKS           = -lpthread
INCLUDES        = -I$(EXTERNAL)
LIBTARGET       = librouter.a

-include $(TOOLDIR)/build.mk