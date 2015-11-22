// File: EvType.hh
// Name: Leah Broussard
// Date: 2015/5/10
// Purpose: Waveform file data structure
//
// Revision History:
// 2015/5/10: LJB  Created


#ifndef EV_TYPE_HH__
#define EV_TYPE_HH__

#include "LocalCFG.hh"
#include "TSystem.h"

struct evch_t {
  Int_t wavelen;
  //std::vector<short> wave;  --LJB  TTree fails?
  Short_t wave[MAXWAVE];
  Short_t rio;
  Short_t ch;
  Short_t Edaq;
};
struct ev_t {
  Long64_t timestamp;
  Int_t numch;
  evch_t ch[MAXCH*NRIO];
};

struct singleev_t { //June data format
  ULong64_t timestamp;
  Int_t board;
  Int_t channel;
  Int_t ch;
  Int_t eventID;
  Int_t length;
  Short_t wave[MAXWAVE];
  Short_t result;
};

#endif // EV_TYPE_HH__
