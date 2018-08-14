# Make a static library

SOURCEDIR       = ./../Src
LIBDIR          = ./../..
TOOLDIR         = ./../../../Tools/
SOURCES         = $(wildcard $(SOURCEDIR)/*.cpp)
DEPENDENCIES    = TcpServer AppUtils Core 
LINKS           = -lpthread
LIBTARGET       = libhttp.a

-include $(TOOLDIR)/build.mk