# Make a static library

SOURCEDIR   = ./../Src
LIBDIR      = ./../..
EXTDIR      = ./../../../External/tinyxml

CC          = g++
AR          = ar
CFLAGS      = -c -Wall -std=c++14
ARFLAGS     = -qsv
ALLSOURCES  = $(wildcard $(SOURCEDIR)/*.cpp $(EXTDIR)/*.cpp)
LIBSOURCES  = $(filter-out %test.cpp, $(filter-out %Main.cpp,$(filter-out %Tester.cpp,$(ALLSOURCES))))
LIBOBJECTS  = $(LIBSOURCES:.cpp=.o)
TARGET      = libxml.a

DEPENDENCIES=
    
INCLUDES=\
    -I$(SOURCEDIR) \
    -I$(EXTDIR) \
    $(patsubst %,-I$(LIBDIR)/%/Src,$(DEPENDENCIES))

all: $(LIBSOURCES) $(TARGET)

$(TARGET): $(LIBOBJECTS)
	$(AR) $(ARFLAGS) $@ $(LIBOBJECTS) 

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@

clean:
	rm -rf $(SOURCEDIR)/*.o $(EXTDIR)/*.o $(TARGET)
