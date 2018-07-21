
CC              = g++
AR              = ar
CFLAGS          = -c -Wall -std=gnu++14

PRODSOURCES     = $(filter-out %MockMain.cpp,$(filter-out %Tester.cpp,$(SOURCES)))
PRODOBJECTS     = $(PRODSOURCES:.cpp=.o)

GMOCKDIR        = ./../../../External/gmock/googlemock
GMOCKSOURCES    = $(GMOCKDIR)/src/gmock-all.cc $(GMOCKDIR)/gtest/src/gtest-all.cc 
GMOCKOBJECTS    = $(GMOCKSOURCES:.cc=.o)
GMOCKINCLUDES   = -I$(GMOCKDIR) -I$(GMOCKDIR)/include -I$(GMOCKDIR)/gtest -I$(GMOCKDIR)/gtest/include 
TESTOBJECTS     = $(SOURCES:.cpp=.o)
TESTTARGET      = GoogleTest

INCLUDES+=\
    -I$(SOURCEDIR) \
    $(GMOCKINCLUDES) \
    $(patsubst %,-I$(LIBDIR)/%/Src,$(DEPENDENCIES)) 

LIBPATHS=\
    $(patsubst %,-L$(LIBDIR)/%/Make,$(DEPENDENCIES))

LIBRARIES=\
    $(patsubst %,-l%,$(shell echo $(DEPENDENCIES) | tr A-Z a-z))
   
# --- RULES ---

$(DEPENDENCIES):
	make all -C $(LIBDIR)/$@/Make

$(LIBTARGET): $(DEPENDENCIES) $(PRODOBJECTS)  
	$(AR) $(ARFLAGS) $@ $(PRODOBJECTS) 

$(TARGET): $(DEPENDENCIES) $(PRODOBJECTS)  
	$(CC) -o $@ $(PRODOBJECTS) $(LIBPATHS) $(LIBRARIES) $(LINKS)
    
$(TESTTARGET): $(DEPENDENCIES) $(TESTOBJECTS) $(GMOCKOBJECTS) 
	$(CC) -o $@ $(TESTOBJECTS) $(GMOCKOBJECTS) $(LIBPATHS) $(LIBRARIES) $(LINKS)
    
.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@

.cc.o:
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@

# --- TARGETS ---

all: $(LIBTARGET) $(TARGET)

clean:
	for lib in $(DEPENDENCIES) ; do \
		make clean -C $(LIBDIR)/$$lib/Make ; \
	done
	rm -rf $(TESTOBJECTS) $(GMOCKOBJECTS) $(LIBTARGET) $(TARGET) $(TESTTARGET)
    
test: $(TESTTARGET)
	./$(TESTTARGET) --gtest_filter="*"
