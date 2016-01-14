{
  gROOT->ProcessLine(".include inc/ "); 
  gROOT->ProcessLine(".L src/RawTreeFile.cpp");
  gROOT->ProcessLine(".L src/WaveformAnalyzer.cpp");
  gROOT->ProcessLine(".L src/TrigTreeFile.cpp");
  gROOT->ProcessLine(".L src/TreeFile.cpp");
  gROOT->ProcessLine(".L src/TrapTreeFile.cpp");
  gROOT->ProcessLine(".L src/EventTreeFile.cpp");
  
  gROOT->ProcessLine(".L scripts/Dec15.C");
}
