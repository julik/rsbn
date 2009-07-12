SDK=/opt/x-plane/SDK

TARGET=rsbn.xpl
HEADERS=$(wildcard *.h)
SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:.cpp=.o)

SDKROOT=/Developer/SDKs/MacOSX10.4u.sdk
OSXC=-arch i386 -arch ppc -universal -mmacosx-version-min=10.4 -isysroot $(SDKROOT)
OSXL=-universal
DEFS=-dynamic -fPIC -Wall -DAPL=1 -m32 -fno-stack-protector -DXPLM200 -ggdb

CXXFLAGS+=-I$(SDK)/CHeaders/XPLM -I$(SDK)/CHeaders/Widgets $(DEFS) $(OSXC)
# # XPLM is loaded dynamically on OS X hence the flag, -dynamiclib is used to get a .a instead of a .bundle
# -undefined warning -flat_namespace -dynamiclib
LNFLAGS+=-undefined warning -flat_namespace  -ggdb $(OSXL)

all: $(TARGET)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<
	
$(TARGET): $(OBJECTS) ../libavionics/libxavionics.a
	libtool -dynamic -o $(TARGET) $(LNFLAGS) $(OBJECTS) $(LIBS)
	ranlib $(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET)
