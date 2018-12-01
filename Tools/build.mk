
CC              = g++
AR              = ar
CFLAGS          = -c -std=c++17 -std=gnu++17 -Wall  
#GCOVFLAGS       = -fprofile-arcs -ftest-coverage
#GCOV            = -lgcov
HTMLDIR         = ./html

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
	$(CC) -o $@ $(PRODOBJECTS) $(LIBPATHS) $(LIBRARIES) $(LINKS) -lstdc++fs $(GCOV)
    
$(TESTTARGET): $(DEPENDENCIES) $(TESTOBJECTS) $(GMOCKOBJECTS) 
	$(CC) -o $@ $(TESTOBJECTS) $(GMOCKOBJECTS) $(LIBPATHS) $(LIBRARIES) $(LINKS) -lstdc++fs $(GCOV)
    
.cpp.o:
	$(CC) $(CFLAGS) $(GCOVFLAGS) $(INCLUDES) $< -o $@

.cc.o:
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@

# --- TARGETS ---

all: $(LIBTARGET) $(TARGET)

clean:
	for lib in $(DEPENDENCIES) ; do \
		make clean -C $(LIBDIR)/$$lib/Make ; \
	done
	rm -rf $(TESTOBJECTS) $(GMOCKOBJECTS) $(LIBTARGET) $(TARGET) $(TESTTARGET) $(TESTOBJECTS:.o=.gcda) $(TESTOBJECTS:.o=.gcno) $(HTMLDIR)

test: $(TESTTARGET)
	./$(TESTTARGET) --gtest_filter="*"

coverage: clean test
	mkdir $(HTMLDIR)
	gcovr $(SOURCEDIR) --html --html-details -o $(HTMLDIR)/$(TESTTARGET).html
