{
  gROOT->ProcessLine(".include inc/ ");
  gROOT->ProcessLine(".L src/ROOTTreeFile.cpp");
  gROOT->ProcessLine(".L src/WaveformAnalyzer.cpp");
  gROOT->ProcessLine(".L src/Quick.C");
}
