// File: SimpleAnalyzer.cpp
// Name: Leah Broussard
// Date: 2013/4/4
// Purpose: Analyze data from NI DAQ
//
// Revision History:
// 2015/5/6:  LJB  Created 

#ifndef ANALYZER_CPP__
#define ANALYZER_CPP__

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>

#include "RawFile.hh"


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

  //-----Open input file
  RawFile InputFile(filenum);
  if (!InputFile.IsOpen()) {
    cout << "File Not Open!" << endl;
    return 0;
  }

  cout << "Done." << endl;
  return 0; 

}

#endif // __ANALYZER_CPP__
