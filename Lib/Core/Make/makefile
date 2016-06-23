# Make a static library

SOURCEDIR       = ./../Src
LIBDIR          = ./../..
SOURCES         = $(wildcard $(SOURCEDIR)/*.cpp)
DEPENDENCIES    = 
LINKS           = -lpthread
TARGET          = libcore.a

CC              = g++
AR              = ar
CFLAGS          = -c -Wall -std=gnu++14

LIBSOURCES      = $(filter-out %Main.cpp,$(filter-out %Tester.cpp,$(SOURCES)))
LIBOBJECTS      = $(LIBSOURCES:.cpp=.o)

GMOCKDIR        = ./../../../External/gmock/googlemock
GMOCKSOURCES    = $(GMOCKDIR)/src/gmock-all.cc $(GMOCKDIR)/gtest/src/gtest-all.cc 
GMOCKOBJECTS    = $(GMOCKSOURCES:.cc=.o)
GMOCKINCLUDES   = -I$(GMOCKDIR) -I$(GMOCKDIR)/include -I$(GMOCKDIR)/gtest -I$(GMOCKDIR)/gtest/include 
TESTOBJECTS     = $(SOURCES:.cpp=.o)
TESTTARGET      = GoogleTest

INCLUDES=\
    -I$(SOURCEDIR) \
    $(GMOCKINCLUDES) \
    $(patsubst %,-I$(LIBDIR)/%/Src,$(DEPENDENCIES)) 

LIBPATHS=\
    $(patsubst %,-L$(LIBDIR)/%/Make,$(DEPENDENCIES))

LIBRARIES=\
    $(patsubst %,-l%,$(shell echo $(DEPENDENCIES) | tr A-Z a-z))
   
# --- RULES ---

$(DEPENDENCIES):
	make -C $(LIBDIR)/$@/Make

$(TARGET): $(DEPENDENCIES) $(LIBOBJECTS)  
	$(AR) $(ARFLAGS) $@ $(LIBOBJECTS) 

$(TESTTARGET): $(DEPENDENCIES) $(TESTOBJECTS) $(GMOCKOBJECTS) 
	$(CC) -o $@ $(TESTOBJECTS) $(GMOCKOBJECTS) $(LIBPATHS) $(LIBRARIES) $(LINKS)
    
.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@

.cc.o:
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@

# --- TARGETS ---

all: $(TARGET)

clean:
	for lib in $(DEPENDENCIES) ; do \
		make clean -C $(LIBDIR)/$$lib/Make ; \
	done
	rm -rf $(TESTOBJECTS) $(GMOCKOBJECTS) $(TARGET) $(TESTTARGET)
    
test: $(TESTTARGET)
	./$(TESTTARGET)
