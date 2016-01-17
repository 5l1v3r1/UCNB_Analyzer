# Copyright 2016.  Los Alamos National Security, LLC.
# This file is part of UCNB_Analyzer.
# This program is distributed under the terms of the GNU General Public License, version 2.0.  See LICENSE.md included in top directory of this distribution.

vpath %.cpp src/
vpath %.hh inc/

ROOTCFLAGS  = $(shell root-config --cflags)
ROOTLIBS = $(shell root-config --libs)   -lNew -lMinuit -lSpectrum
ROOTGLIBS = $(shell root-config --glibs)

CXX = g++
CPPFLAGS = -I inc/ -D_FILE_OFFSET_BITS=64
LDFLAGS = -L/usr/local/lib -L/usr/include -L/root/lib

MAIN = Analyzer
SOURCES = BinFile.cpp NIFeb2015BinFile.cpp NIJune2015BinFile.cpp TreeFile.cpp RawTreeFile.cpp TrapTreeFile.cpp TrigTreeFile.cpp WaveformAnalyzer.cpp TriggerList.cpp EventTreeFile.cpp
OBJECTS = $(SOURCES:.cpp=.o) $(MAIN).o 
INCLUDES = $(SOURCES:.cpp=.hh)

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
