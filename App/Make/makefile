# create an executable

SOURCEDIR   = ./../Src
LIBDIR      = ./../../Lib

CC          = g++
CFLAGS      = -c -std=c++14
LFLAGS      = -o
ALLSOURCES  = $(wildcard $(SOURCEDIR)/*.cpp)
APPSOURCES  = $(ALLSOURCES)
APPOBJECTS  = $(APPSOURCES:.cpp=.o)
TARGET      = serveraplication

DEPENDENCIES=\
    TcpServer \
    Router \
    AppUtils \
    Xml \
    Core

INCLUDES=\
    -I$(SOURCEDIR) \
    $(patsubst %,-I$(LIBDIR)/%/Src,$(DEPENDENCIES))

LIBPATHS=\
    $(patsubst %,-L$(LIBDIR)/%/Make,$(DEPENDENCIES))

LIBRARIES=\
    $(patsubst %,-l%,$(shell echo $(DEPENDENCIES) | tr A-Z a-z))

all: buildlibs $(APPSOURCES) $(TARGET)

$(TARGET): $(APPOBJECTS)
	$(CC) $(LFLAGS) $@ $(APPOBJECTS) $(LIBPATHS) $(LIBRARIES) 

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@

buildlibs: 
	for lib in $(DEPENDENCIES) ; do \
		make -C $(LIBDIR)/$$lib/Make ; \
	done

cleanlibs: 
	for lib in $(DEPENDENCIES) ; do \
		make clean -C $(LIBDIR)/$$lib/Make ; \
	done

clean:
	rm -rf $(SOURCEDIR)/*.o $(TARGET)

distclean: cleanlibs clean
