# Make a static library

EXTERNAL        = ./../../../External/nlohman
SOURCEDIR       = ./../Src
LIBDIR          = ./../..
TOOLDIR         = ./../../../Tools/
SOURCES         = $(wildcard $(SOURCEDIR)/*.cpp)
DEPENDENCIES    = Core
LINKS           = -lpthread
INCLUDES        = -I$(EXTERNAL)
LIBTARGET       = libmath.a

-include $(TOOLDIR)/build.mk