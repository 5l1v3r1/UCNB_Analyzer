vpath %.cpp src/
vpath %.hh inc/

ROOTCFLAGS  = $(shell root-config --cflags)
ROOTLIBS = $(shell root-config --libs)   -lNew -lMinuit -lSpectrum
ROOTGLIBS = $(shell root-config --glibs)

CXX = g++
CPPFLAGS = -I inc/ -D_FILE_OFFSET_BITS=64
LDFLAGS = -L/usr/local/lib -L/usr/include -L/root/lib

MAIN = Analyzer
SOURCES = RawFile.cpp ROOTTreeFile.cpp TrigTreeFile.cpp WaveformAnalyzer.cpp TriggerList.cpp
OBJECTS = $(SOURCES:.cpp=.o) $(MAIN).o 
INCLUDES = $(SOURCES:.cpp=.hh) LocalCFG.hh EvType.hh

all: $(MAIN)

$(MAIN): $(OBJECTS) 
	$(CXX) $(CPPFLAGS) $(LDFLAGS) $(ROOTCFLAGS) -o $@ $(OBJECTS) $(ROOTLIBS) $(ROOTGLIBS)

$(MAIN).o : $(MAIN).cpp $(INCLUDES)
	$(CXX) $(CPPFLAGS) $(LDFLAGS) $(ROOTCFLAGS) -c $< -o $@ $(ROOTLIBS) $(ROOTGLIBS)

%.o : %.cpp %.hh
	$(CXX) $(CPPFLAGS) $(LDFLAGS) $(ROOTCFLAGS) -c $< -o $@ $(ROOTLIBS) $(ROOTGLIBS)

clean :
	rm -f *.o *~ $(MAIN)

.PHONY : all clean
