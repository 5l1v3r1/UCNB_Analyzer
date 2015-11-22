{
  gROOT->ProcessLine(".include inc/ "); 
  gROOT->ProcessLine(".L src/ROOTTreeFileJune.cpp");
  gROOT->ProcessLine(".L src/WaveformAnalyzer.cpp");
  gROOT->ProcessLine(".L src/TrigTreeFile.cpp");
  gROOT->ProcessLine(".L src/TreeFile.cpp");
  gROOT->ProcessLine(".L src/TrapTreeFile.cpp");
  gROOT->ProcessLine(".L src/EventTreeFile.cpp");
  //  gROOT->ProcessLine(".L src/Quick.C");  
  // gROOT->ProcessLine(".L src/Calib.C");
  /*
  gROOT->ProcessLine(".L src/RawFile.cpp");
  gROOT->ProcessLine(".L src/Quick2.C");  
  gROOT->ProcessLine(".L src/ChPlots.C");
  */
}
