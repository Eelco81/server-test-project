
-include $(ROOT_DIR)/Tools/config.mk

CC              = g++
AR              = ar
CFLAGS          = -c -std=c++17 -std=gnu++17 -Wall
LINKS           = -lpthread -lstdc++fs
DEFINES         = -DOS_FAMILY=$(OS_FAMILY) -DBUILD_TARGET=$(BUILD_TARGET)

ifeq ($(MAKECMDGOALS), release-all) 
	BUILD_TARGET=release
	CFLAGS += -O3
else ifeq ($(MAKECMDGOALS), install) 
	BUILD_TARGET=release
	CFLAGS += -O3 
else 
	BUILD_TARGET=debug
	GCOVFLAGS = -fprofile-arcs -ftest-coverage
	GCOV = -lgcov
	HTMLDIR = $(BUILD_TARGET)/html
endif

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

$(BINLIBTARGET): $(PRODOBJECTS)
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
	./$(TESTTARGET) --gtest_filter="*" --gtest_output="xml:$(BUILD_TARGET)/GoogleTestOutput.xml"

install: release-all
	cp $(BINTARGET) $(INSTALL_DIR)

coverage:
	rm -rf $(HTMLDIR)
	mkdir $(HTMLDIR)
	gcovr --filter=../Src --exclude=.*Tester.cpp --html --html-details --output=$(HTMLDIR)/index.html --object-directory=./$(BUILD_TARGET)
