
-include $(ROOT_DIR)/Tools/config.mk

CC              = g++
AR              = ar
# OPTFLAGS        = -03
CFLAGS          = -c -std=c++17 -std=gnu++17 -Wall $(OPTFLAGS)
LINKS           = -lpthread -lstdc++fs

#GCOVFLAGS       = -fprofile-arcs -ftest-coverage
#GCOV            = -lgcov
#HTMLDIR         = ./html
#BOOST_ROOT      = $(EXTERNAL_DIR)/boost

SOURCES         = $(wildcard $(SOURCE_DIR)/*.cpp)
PRODSOURCES     = $(filter-out %MockMain.cpp,$(filter-out %Tester.cpp,$(SOURCES)))
PRODOBJECTS     = $(PRODSOURCES:.cpp=.o)

GMOCKDIR        = $(EXTERNAL_DIR)/gmock/googlemock
GMOCKSOURCES    = $(GMOCKDIR)/src/gmock-all.cc $(GMOCKDIR)/gtest/src/gtest-all.cc 
GMOCKOBJECTS    = $(GMOCKSOURCES:.cc=.o)
GMOCKINCLUDES   = -I$(GMOCKDIR) -I$(GMOCKDIR)/include -I$(GMOCKDIR)/gtest -I$(GMOCKDIR)/gtest/include 
TESTOBJECTS     = $(SOURCES:.cpp=.o)
TESTTARGET      = GoogleTest

INCLUDES+=\
    -I$(SOURCE_DIR) \
    $(GMOCKINCLUDES) \
    -I$(EXTERNAL_DIR)\nlohman \
    -I$(EXTERNAL_DIR)\sha1 \
    $(patsubst %,-I$(LIB_DIR)/%/Src,$(DEPENDENCIES)) 

LIBPATHS=\
    $(patsubst %,-L$(LIB_DIR)/%/Make,$(DEPENDENCIES))

LIBRARIES=\
    $(patsubst %,-l%,$(shell echo $(DEPENDENCIES) | tr A-Z a-z))

# --- RULES ---

$(DEPENDENCIES):
	make all -C $(LIB_DIR)/$@/Make

$(LIBTARGET): $(DEPENDENCIES) $(PRODOBJECTS)  
	$(AR) $(ARFLAGS) $@ $(PRODOBJECTS) 

$(TARGET): $(DEPENDENCIES) $(PRODOBJECTS)  
	$(CC) -o $@ $(PRODOBJECTS) $(LIBPATHS) $(LIBRARIES) $(LINKS) $(GCOV)

$(TESTTARGET): $(DEPENDENCIES) $(TESTOBJECTS) $(GMOCKOBJECTS) 
	$(CC) -o $@ $(TESTOBJECTS) $(GMOCKOBJECTS) $(LIBPATHS) $(LIBRARIES) $(LINKS) $(GCOV)

.cpp.o:
	$(CC) $(CFLAGS) $(GCOVFLAGS) $(INCLUDES) $< -o $@

.cc.o:
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@

# --- TARGETS ---

all: $(LIBTARGET) $(TARGET)

clean:
	for lib in $(DEPENDENCIES) ; do \
		make clean -C $(LIB_DIR)/$$lib/Make ; \
	done
	rm -rf $(TESTOBJECTS) $(GMOCKOBJECTS) $(LIBTARGET) $(TARGET) $(TESTTARGET) $(TESTOBJECTS:.o=.gcda) $(TESTOBJECTS:.o=.gcno) $(HTMLDIR)

test: $(TESTTARGET)
	./$(TESTTARGET) --gtest_filter="*" --gtest_output="xml:GoogleTestOutput.xml"

#coverage: clean test
#	mkdir $(HTMLDIR)
#	gcovr $(SOURCEDIR) --html --html-details -o $(HTMLDIR)/$(TESTTARGET).html
