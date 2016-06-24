# Make a static library

EXTERNAL        = ./../../../External/tinyxml
SOURCEDIR       = ./../Src
LIBDIR          = ./../..
TOOLDIR         = ./../../../Tools/
SOURCES         = $(wildcard $(SOURCEDIR)/*.cpp) $(filter-out %test.cpp,$(wildcard $(EXTERNAL)/*.cpp))
DEPENDENCIES    = 
LINKS           = 
INCLUDES        = -I$(EXTERNAL)
LIBTARGET       = libxml.a

-include $(TOOLDIR)/build.mk
