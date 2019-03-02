
-include $(ROOT_DIR)/Tools/config.mk

ifeq ($(MAKECMDGOALS),release-all) 
    BUILD_TARGET=release
    OPTFLAGS=-O3
else 
    BUILD_TARGET=debug
endif

CC              = g++
AR              = ar
CFLAGS          = -c -std=c++17 -std=gnu++17 -Wall $(OPTFLAGS)
LINKS           = -lpthread -lstdc++fs

ifeq ($(OS_FAMILY), WINDOWS) 
    LINKS += -lntdll
endif

BINTARGET       = $(addprefix $(BUILD_TARGET)/, $(TARGET))
BINLIBTARGET    = $(addprefix $(BUILD_TARGET)/, $(LIBTARGET))
TESTTARGET      = $(BUILD_TARGET)/GoogleTest
SOURCES         = $(wildcard $(SOURCE_DIR)/*.cpp)
PRODSOURCES     = $(filter-out %MockMain.cpp, $(filter-out %Tester.cpp, $(SOURCES)))
PRODOBJECTS     = $(addprefix $(BUILD_TARGET)/, $(notdir $(PRODSOURCES:.cpp=.o)))
TESTOBJECTS     = $(addprefix $(BUILD_TARGET)/, $(notdir $(SOURCES:.cpp=.o)))

GMOCKDIR        = $(EXTERNAL_DIR)/gmock/googlemock
GMOCKSOURCES    = $(GMOCKDIR)/src/gmock-all.cc $(GMOCKDIR)/gtest/src/gtest-all.cc 
GMOCKOBJECTS    = $(GMOCKSOURCES:.cc=.o)
GMOCKINCLUDES   = -I$(GMOCKDIR) -I$(GMOCKDIR)/include -I$(GMOCKDIR)/gtest -I$(GMOCKDIR)/gtest/include 

DEFINES+=\
	-DOS_FAMILY=$(OS_FAMILY)\
	-DBUILD_TARGET=$(BUILD_TARGET)

INCLUDES+=\
	-I$(SOURCE_DIR) \
	$(GMOCKINCLUDES) \
	-I$(EXTERNAL_DIR)\nlohman \
	-I$(EXTERNAL_DIR)\sha1 \
	$(patsubst %, -I$(LIB_DIR)/%/Src, $(DEPENDENCIES)) 

LIBPATHS=\
	$(patsubst %, -L$(LIB_DIR)/%/Make/$(BUILD_TARGET), $(DEPENDENCIES))

LIBLINKS=\
	$(patsubst %,-l%,$(shell echo $(DEPENDENCIES) | tr A-Z a-z))

# --- RULES ---

$(DEPENDENCIES):
	$(MAKE) $(BUILD_TARGET)-all -C $(LIB_DIR)/$@/Make

$(BINLIBTARGET): $(DEPENDENCIES) $(PRODOBJECTS)
	$(AR) $(ARFLAGS) $@ $(PRODOBJECTS)

$(BINTARGET): $(DEPENDENCIES) $(PRODOBJECTS)
	$(CC) -o $@ $(PRODOBJECTS) $(LIBPATHS) $(LIBLINKS) $(LINKS) $(GCOV)

$(TESTTARGET): $(DEPENDENCIES) $(TESTOBJECTS) $(GMOCKOBJECTS) 
	$(CC) -o $@ $(TESTOBJECTS) $(GMOCKOBJECTS) $(LIBPATHS) $(LIBLINKS) $(LINKS) $(GCOV)

$(BUILD_TARGET)/%.o: $(SOURCE_DIR)/%.cpp
	$(CC) $(DEFINES) $(CFLAGS) $(GCOVFLAGS) $(INCLUDES) $< -o $@

.cc.o:
	$(CC) $(DEFINES) $(CFLAGS) $(GCOVFLAGS) $(INCLUDES) $< -o $@

# --- TARGETS ---

setup:
	mkdir $(BUILD_TARGET) || :

%-all: setup $(BINTARGET) $(BINLIBTARGET)
	

clean:
	for lib in $(DEPENDENCIES) ; do \
		$(MAKE) $@ -C $(LIB_DIR)/$$lib/Make ; \
	done
	rm -rf debug release $(GMOCKOBJECTS) *.xml

test: setup $(TESTTARGET)
	./$(TESTTARGET) --gtest_filter="*" --gtest_output="xml:GoogleTestOutput.xml"


# --- WIP ---

# $(TESTOBJECTS) $(GMOCKOBJECTS) $(LIBTARGET) $(TARGET) $(TESTTARGET) $(TESTOBJECTS:.o=.gcda) $(TESTOBJECTS:.o=.gcno) $(HTMLDIR)

#GCOVFLAGS       = -fprofile-arcs -ftest-coverage
#GCOV            = -lgcov
#HTMLDIR         = ./html

#coverage: clean test
#	mkdir $(HTMLDIR)
#	gcovr $(SOURCEDIR) --html --html-details -o $(HTMLDIR)/$(TESTTARGET).html
