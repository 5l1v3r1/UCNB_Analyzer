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

struct evch_t {
  Int_t wavelen;
  std::vector<Short_t> wave;
  Short_t rio;
  Short_t ch;
  Short_t Edaq;
};
struct ev_t {
  Long64_t timestamp;
  Int_t numch;
  evch_t ch[MAXCH*NRIO];
};

#endif // EV_TYPE_HH__
