// Copyright 2016.  Los Alamos National Security, LLC.
// This file is part of UCNB_Analyzer.
// This program is distributed under the terms of the GNU General Public License, version 2.0.  See LICENSE.md included in top directory of this distribution.

// Simple loading script for use in ROOT

{
  gROOT->ProcessLine(".include inc/ "); 
  gROOT->ProcessLine(".include inc/BinFileWrappers/ "); 
  gROOT->ProcessLine(".include inc/Legacy/ "); 
  gROOT->ProcessLine(".include inc/Tasks "); 
  gROOT->ProcessLine(".include inc/TFileWrappers/ "); 
  gROOT->ProcessLine(".L src/TFileWrappers/RawTreeFile.cpp");
  gROOT->ProcessLine(".L src/Tasks/WaveformAnalyzer.cpp");
  gROOT->ProcessLine(".L src/TFileWrappers/TrigTreeFile.cpp");
  gROOT->ProcessLine(".L src/TFileWrappers/FitTreeFile.cpp");
  gROOT->ProcessLine(".L src/TFileWrappers/TreeFile.cpp");
  gROOT->ProcessLine(".L src/TFileWrappers/TrapTreeFile.cpp");
  gROOT->ProcessLine(".L src/TFileWrappers/EventTreeFile.cpp");
}
