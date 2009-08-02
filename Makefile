SDK=/opt/x-plane/SDK
PLUGINS=/Users/julik/Desktop/X-Plane-Beta/Resources/plugins/rsbn/
TARGET=mac.xpl
HEADERS=$(wildcard *.h)
SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:.cpp=.o)

# Build for 10.4 compatibility, Intel and PPC. Assume X-Plane SDK is in /opt/x-plane
SDKROOT=/Developer/SDKs/MacOSX10.4u.sdk
OSXC=-arch i386 -arch ppc -universal -mmacosx-version-min=10.4 -isysroot $(SDKROOT)
OSXL=-arch i386 -arch ppc -syslibroot=$(SDKROOT)
DEFS= -DAPL=1 -fPIC -m32 -fno-stack-protector -DXPLM200 -ggdb

CXXFLAGS+=-Wall -I$(SDK)/CHeaders/XPLM -I$(SDK)/CHeaders/Widgets $(DEFS) $(OSXC)

# XPLM is loaded dynamically on OS X hence the -undefined option, -dynamiclib is used to get a .a file instead of a .bundle
# The magiс incantation is -undefined warning -flat_namespace -dynamiclib -shared -rdynamic -nodefaultlibs,
# -m32 is to force 32bit since recent OS X is mostly 64bit AND it will build 64bit by default
LNFLAGS+=-undefined warning -flat_namespace -dynamiclib -shared -rdynamic -nodefaultlibs -m32 -ggdb  $(OSXL)
LIBS+=-lm

all: $(TARGET)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<
	
$(TARGET): $(OBJECTS)
	$(CXX) -o $(TARGET) $(LNFLAGS) $(OBJECTS) $(LIBS)

clean:
	rm -f $(OBJECTS) $(TARGET)
	rm -r dist

install: $(TARGET)
	cp -f $(TARGET) $(PLUGINS)
	mkdir -p $(PLUGINS)/data
	cp -f data/ussr.dat $(PLUGINS)/data/
	cp -f data/cis.dat $(PLUGINS)/data/

dist: $(TARGET)
	mkdir -p dist
	mv -f $(TARGET) dist/
	ruby write_docs.rb	

