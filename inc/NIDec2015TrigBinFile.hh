// Copyright 2016.  UT Battelle, LLC.
// This file is part of UCNB_Analyzer.
// See LICENSE.md included in top directory of this distribution.

// File: NIDec2015TrigBinFile.hh
// Purpose: Opens binary files in NI Dec 2015 trig format
 
#ifndef NI_DEC_TRIG_BIN_FILE_HH__
#define NI_DEC_TRIG_BIN_FILE_HH__

#include <vector>

#include "BinFile.hh"

using std::cout;
using std::endl;
using std::vector;

class NIDec2015TrigBinFile : public BinFile
{
public: 
  struct DecTrigBinEv_t : public BinEv_t{
    unsigned long long int timestamp;
    int board;
    short channel;
    short adc;
  };
private:
  bool ReadHeader();
public: 
	NIDec2015TrigBinFile();
	NIDec2015TrigBinFile(std::string path, std::string name);
	NIDec2015TrigBinFile(std::string pathname);
	~NIDec2015TrigBinFile();
	using BinFile::Open;
	bool Open(int filenum, int rionum = -1);
	bool ReadNextEvent(BinEv_t& NI_event);

};

#endif // __NI_DEC_TRIG_BIN_FILE_HH__
