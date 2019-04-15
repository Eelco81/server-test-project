
-include $(ROOT_DIR)/Tools/config.mk
-include $(ROOT_DIR)/Tools/external.mk

CC      = g++
AR      = ar
CFLAGS  = -c -std=c++17 -std=gnu++17 -Wall
LINKS   = -lpthread -lstdc++fs

ifeq ($(MAKECMDGOALS), release-all) 
	BUILD_TARGET=release
	CFLAGS += -O3
else ifeq ($(MAKECMDGOALS), install) 
	BUILD_TARGET=release
	CFLAGS += -O3 
else 
	BUILD_TARGET = debug
	CFLAGS       += -fprofile-arcs -ftest-coverage
	LINKS        += -lgcov
	HTMLDIR      = $(BUILD_TARGET)/html
endif

ifeq ($(OS_FAMILY), WINDOWS) 
	LINKS += -lntdll
endif

BINTARGET       = $(addprefix $(BUILD_TARGET)/, $(TARGET))
BINLIBTARGET    = $(addprefix $(BUILD_TARGET)/, $(LIBTARGET))
TESTTARGET      = $(BUILD_TARGET)/GoogleTest
SOURCES         = $(wildcard $(SOURCE_DIR)/*.cpp)
GMOCKSOURCES    = $(GMOCK_DIR)/src/gmock-all.cc $(GTEST_DIR)/src/gtest-all.cc 
PRODSOURCES     = $(filter-out %MockMain.cpp, $(filter-out %Tester.cpp, $(SOURCES)))
PRODOBJECTS     = $(addprefix $(BUILD_TARGET)/, $(notdir $(PRODSOURCES:.cpp=.o)))
TESTOBJECTS     = $(addprefix $(BUILD_TARGET)/, $(notdir $(SOURCES:.cpp=.o)))
TESTOBJECTS     += $(addprefix $(BUILD_TARGET)/, $(notdir $(GMOCKSOURCES:.cc=.o)))

DEFINES=\
	-DOS_FAMILY=$(OS_FAMILY)\
	-DOS_COMPILER=$(OS_COMPILER)\
	-DBUILD_TARGET=$(BUILD_TARGET)

INCLUDES+=\
	-I$(SOURCE_DIR) \
	-I$(EXTERNAL_DIR)/include \
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
	$(CC) -o $@ $(PRODOBJECTS) $(LIBPATHS) $(LIBLINKS) $(LINKS)

$(TESTTARGET): $(DEPENDENCIES) $(TESTOBJECTS)
	$(CC) -o $@ $(TESTOBJECTS) $(LIBPATHS) $(LIBLINKS) $(LINKS)

$(BUILD_TARGET)/%.o: $(SOURCE_DIR)/%.cpp
	$(CC) $(DEFINES) $(CFLAGS) $(INCLUDES) $< -o $@

$(BUILD_TARGET)/%.o: $(GMOCK_DIR)/src/%.cc
	$(CC) $(CFLAGS) $(INCLUDES) -I$(GMOCK_DIR) -I$(GTEST_DIR) $< -o $@

$(BUILD_TARGET)/%.o: $(GTEST_DIR)/src/%.cc 
	$(CC) $(CFLAGS) $(INCLUDES) -I$(GMOCK_DIR) -I$(GTEST_DIR) $< -o $@

# --- TARGETS ---

setup:
	mkdir $(BUILD_TARGET) || :

%-all: setup $(BINTARGET) $(BINLIBTARGET)
	

clean:
	rm -rf debug release

distclean: clean
	for lib in $(DEPENDENCIES) ; do \
		$(MAKE) $@ -C $(LIB_DIR)/$$lib/Make ; \
	done

test: setup $(TESTTARGET)
	./$(TESTTARGET) --gtest_filter="*" --gtest_output="xml:$(BUILD_TARGET)/GoogleTestOutput.xml"

install: release-all
	cp $(BINTARGET) $(INSTALL_DIR)

coverage:
	rm -rf $(HTMLDIR)
	mkdir $(HTMLDIR)
	gcovr --filter=../Src --exclude=.*Tester.cpp --html --html-details --output=$(HTMLDIR)/index.html --object-directory=./$(BUILD_TARGET)
