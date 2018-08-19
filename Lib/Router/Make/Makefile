# Make a static library

SOURCEDIR       = ./../Src
LIBDIR          = ./../..
TOOLDIR         = ./../../../Tools/
SOURCES         = $(wildcard $(SOURCEDIR)/*.cpp)
DEPENDENCIES    = Http TcpServer AppUtils Core 
LINKS           = -lpthread
LIBTARGET       = librouter.a

-include $(TOOLDIR)/build.mk