CXXFLAGS = -fno-exceptions -fno-rtti
LDFLAGS = -municode -static -s
LDLIBS = -luuid

ConsoleApplication1.exe: ConsoleApplication1.cpp

%.exe: %.cpp
	$(LINK.cpp) $^ $(LDLIBS) -o $@
