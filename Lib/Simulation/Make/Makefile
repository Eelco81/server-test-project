# Make a static library

EXTERNAL        = ./../../../External/nlohman
SOURCEDIR       = ./../Src
LIBDIR          = ./../..
TOOLDIR         = ./../../../Tools/
SOURCES         = $(wildcard $(SOURCEDIR)/*.cpp)
DEPENDENCIES    = AppUtils Math Core
LINKS           = -lpthread
INCLUDES        = -I$(EXTERNAL)
LIBTARGET       = libsimulation.a

-include $(TOOLDIR)/build.mk