// File: SimpleAnalyzer.cpp
// Name: Leah Broussard
// Date: 2013/4/4
// Purpose: Analyze data from NI DAQ
//
// Revision History:
// 2015/5/6:  LJB  Created 
// 2015/5/10: LJB  Converts raw files into ROOT files

#ifndef ANALYZER_CPP__
#define ANALYZER_CPP__

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>

#include "RawFile.hh"
#include "ROOTTreeFile.hh"


/*************************************************************************/
//                            Main Function
/*************************************************************************/
int main (int argc, char *argv[]) {  
  using std::cout;
  using std::endl;
  
  cout << "Welcome to the NI DAQ Analyzer " << endl;
  if (argc != 2) {
    cout << "Usage: " << argv[0] << " filenum" << endl;
    return 1;
  }
  int filenum = atoi(argv[1]);


  //-----Open input/output files
  RawFile InputFile(filenum);
  if (!InputFile.IsOpen()) {
    cout << "File Not Open!" << endl;
    return 0;
  }
  ROOTTreeFile RootFile;
  RootFile.Create(filenum);
  RootFile.SetupTree(InputFile.GetNumCh());
  //-----Read events
  while (InputFile.ReadEvent(RootFile.NI_event)) {
    RootFile.FillTree();
  }
  RootFile.Write();
  /*
  cout << "Found ts " << NI_event.timestamp << endl;
  cout << "Found numch " << NI_event.numch << endl;
  for (int ch=0;ch<NI_event.numch;ch++) {
    cout << "ch: " << ch << " is rio " << NI_event.ch[ch].rio << " and ch " << NI_event.ch[ch].ch << endl;
    cout << "wavelen " << NI_event.ch[ch].wavelen << " samp 0 " << NI_event.ch[ch].wave[0] << " and E " << NI_event.ch[ch].Edaq << endl;
  }
  */
  cout << "Done." << endl;
  return 0; 

}

#endif // __ANALYZER_CPP__
