# Make a static library

SOURCEDIR   = ./../Src
LIBDIR      = ./../..

CC          = g++
AR          = ar
CFLAGS      = -c -Wall -std=c++14
ARFLAGS     = -qsv
ALLSOURCES  = $(wildcard $(SOURCEDIR)/*.cpp)
LIBSOURCES  = $(filter-out %Main.cpp,$(filter-out %Tester.cpp,$(ALLSOURCES)))
LIBOBJECTS  = $(LIBSOURCES:.cpp=.o)
TARGET      = libapputils.a

DEPENDENCIES=\
    Core

INCLUDES=\
    -I$(SOURCEDIR) \
    $(patsubst %,-I$(LIBDIR)/%/Src,$(DEPENDENCIES))

all: $(LIBSOURCES) $(TARGET)

$(TARGET): $(LIBOBJECTS)
	$(AR) $(ARFLAGS) $@ $(LIBOBJECTS) 

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@

clean:
	rm -rf $(SOURCEDIR)/*.o $(TARGET)
