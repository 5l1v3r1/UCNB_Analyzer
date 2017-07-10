# Copyright 2016.  Los Alamos National Security, LLC.
# Copyright 2016.  UT-Battelle, LLC.
# This file is part of UCNB_Analyzer.
# See LICENSE.md included in top directory of this distribution.

vpath %.cpp src/:src/TFileWrappers:src/Tasks
vpath %.hh inc/:inc/TFileWrappers:inc/Tasks

ROOTCFLAGS  = $(shell root-config --cflags)
ROOTLIBS = $(shell root-config --libs)  -lMinuit -lSpectrum
ROOTGLIBS = $(shell root-config --glibs)

CXX = g++
CPPFLAGS = -I inc/ -I inc/TFileWrappers -I inc/Tasks -D_FILE_OFFSET_BITS=64
LDFLAGS = -L/usr/local/lib -L/usr/include -L/root/lib

BUILDDIR = build

#Code for UCNB
UCNB = UCNBAnalyzer
BSOURCES = BinFile.cpp NIFeb2015BinFile.cpp NIJune2015BinFile.cpp NIMay2016BinFile.cpp  NIMay2017BinFile.cpp NIDec2015TrigBinFile.cpp NIMay2016TrigBinFile.cpp TreeFile.cpp RawTreeFile.cpp TrigTreeFile.cpp TrapTreeFile.cpp FitTreeFile.cpp WaveformAnalyzer.cpp WaveformAverage.cpp EventTreeFile.cpp CalibSource.cpp SiCalibrator.cpp CommandParser.cpp ReplayFile.cpp ReplayBinFile.cpp ReplayTrigFile.cpp ApplySingleTrap.cpp FitRCCR.cpp FindCoincidence.cpp BuildTemplateWaveform.cpp Calibrate.cpp ShapeScan.cpp
BOBJECTS = $(patsubst %.cpp, $(BUILDDIR)/%.o,$(BSOURCES)) $(BUILDDIR)/$(UCNB).o 
BINCLUDES = $(BSOURCES:.cpp=.hh)


all: $(UCNB)

$(UCNB): $(BOBJECTS) 
	$(CXX) $(CPPFLAGS) $(LDFLAGS) $(ROOTCFLAGS) -o $@ $(BOBJECTS) $(ROOTLIBS) $(ROOTGLIBS)

$(BUILDDIR)/$(UCNB).o : $(UCNB).cpp $(BINCLUDES)
	$(CXX) $(CPPFLAGS) $(LDFLAGS) $(ROOTCFLAGS) -c $< -o $@ $(ROOTLIBS) $(ROOTGLIBS)

$(BUILDDIR)/%.o : %.cpp %.hh 
	$(CXX) $(CPPFLAGS) $(LDFLAGS) $(ROOTCFLAGS) -c $< -o $@ $(ROOTLIBS) $(ROOTGLIBS)

clean :
	rm -f *.o $(BUILDDIR)/*.o *~ $(UCNB)

.PHONY : all clean
