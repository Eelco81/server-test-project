# Make a static library

EXTERNAL        = ./../../../External/nlohman
SOURCEDIR       = ./../Src
LIBDIR          = ./../..
TOOLDIR         = ./../../../Tools/
SOURCES         = $(wildcard $(SOURCEDIR)/*.cpp)
DEPENDENCIES    = Simulation AppUtils Math Core
LINKS           = -lpthread
INCLUDES        = -I$(EXTERNAL)
LIBTARGET       = libsimcomponents.a

-include $(TOOLDIR)/build.mk