vpath %.cpp src/
vpath %.hh inc/

ROOTCFLAGS  = $(shell root-config --cflags)
ROOTLIBS = $(shell root-config --libs)   -lNew -lMinuit -lSpectrum
ROOTGLIBS = $(shell root-config --glibs)

CXX = g++
CPPFLAGS = -I inc/ -D_FILE_OFFSET_BITS=64
LDFLAGS = -L/usr/local/lib -L/usr/include -L/root/lib

MAIN = Analyzer
SOURCES = TreeFile.cpp RawFileJune.cpp ROOTTreeFileJune.cpp TrigTreeFile.cpp WaveformAnalyzer.cpp TriggerList.cpp EventTreeFile.cpp RawFileFeb.cpp ROOTTreeFileFeb.cpp NIFeb2015BinFile.cpp NIJune2015BinFile.cpp BinFile.cpp
OBJECTS = $(SOURCES:.cpp=.o) $(MAIN).o 
INCLUDES = $(SOURCES:.cpp=.hh) LocalCFG.hh EvType.hh

REPLAY = Replay
REPLAYSRC = NIFeb2015BinFile.cpp NIJune2015BinFile.cpp BinFile.cpp RawTreeFile.cpp TreeFile.cpp 
REPLAYOBJ = $(REPLAYSRC:.cpp=.o) $(REPLAY).o
REPLAYINC = $(REPLAYSRC:.cpp=.hh) LocalCFG.hh EvType.hh

TRAP = DoTrapFilter
TRAPSRC = TreeFile.cpp ROOTTreeFileJune.cpp TrapTreeFile.cpp TriggerList.cpp WaveformAnalyzer.cpp
TRAPOBJ = $(TRAPSRC:.cpp=.o) $(TRAP).o
TRAPINC = $(TRAPSRC:.cpp=.hh) LocalCFG.hh EvType.hh

all: $(MAIN) $(REPLAY) $(TRAP)

$(MAIN): $(OBJECTS) 
	$(CXX) $(CPPFLAGS) $(LDFLAGS) $(ROOTCFLAGS) -o $@ $(OBJECTS) $(ROOTLIBS) $(ROOTGLIBS)

$(REPLAY): $(REPLAYOBJ)
	$(CXX) $(CPPFLAGS) $(LDFLAGS) $(ROOTCFLAGS) -o $@ $(REPLAYOBJ) $(ROOTLIBS) $(ROOTGLIBS)

$(TRAP): $(TRAPOBJ)
	$(CXX) $(CPPFLAGS) $(LDFLAGS) $(ROOTCFLAGS) -o $@ $(TRAPOBJ) $(ROOTLIBS) $(ROOTGLIBS)


$(MAIN).o : $(MAIN).cpp $(INCLUDES)
	$(CXX) $(CPPFLAGS) $(LDFLAGS) $(ROOTCFLAGS) -c $< -o $@ $(ROOTLIBS) $(ROOTGLIBS)

$(REPLAY).o : $(REPLAY).cpp $(REPLAYINC)
	$(CXX) $(CPPFLAGS) $(LDFLAGS) $(ROOTCFLAGS) -c $< -o $@ $(ROOTLIBS) $(ROOTGLIBS)

$(TRAP).o : $(TRAP).cpp $(TRAPINC)
	$(CXX) $(CPPFLAGS) $(LDFLAGS) $(ROOTCFLAGS) -c $< -o $@ $(ROOTLIBS) $(ROOTGLIBS)


%.o : %.cpp %.hh LocalCFG.hh #hack, need to fix dependencies
	$(CXX) $(CPPFLAGS) $(LDFLAGS) $(ROOTCFLAGS) -c $< -o $@ $(ROOTLIBS) $(ROOTGLIBS)

clean :
	rm -f *.o *~ $(MAIN)

.PHONY : all clean
