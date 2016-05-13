// Copyright 2016.  Los Alamos National Security, LLC.
// This file is part of UCNB_Analyzer.
// This program is distributed under the terms of the GNU General Public License, version 2.0.  See LICENSE.md included in top directory of this distribution.

// Simple loading script for use in ROOT

{
  gROOT->ProcessLine(".include inc/ "); 
  gROOT->ProcessLine(".L src/RawTreeFile.cpp");
  gROOT->ProcessLine(".L src/WaveformAnalyzer.cpp");
  gROOT->ProcessLine(".L src/TrigTreeFile.cpp");
  gROOT->ProcessLine(".L src/FitTreeFile.cpp");
  gROOT->ProcessLine(".L src/TreeFile.cpp");
  gROOT->ProcessLine(".L src/TrapTreeFile.cpp");
  gROOT->ProcessLine(".L src/EventTreeFile.cpp");
//  gROOT->ProcessLine(".L src/SiCalibrator.cpp");
 // ROOT.gInterpreter.GenerateDictionary("vector<SiCalibrator::CalibData_t>","inc/SiCalibrator.h;vector");
 // ROOT.gInterpreter.GenerateDictionary("vector<SiCalibrator::ChData_t>","inc/SiCalibrator.h;vector");
 // ROOT.gInterpreter.GenerateDictionary("vector<SiCalibrator::RunLog_t>","inc/SiCalibrator.h;vector");
  
  gROOT->ProcessLine(".L scripts/CheckCal.C");
  gROOT->ProcessLine(".L scripts/Example.C");
}
