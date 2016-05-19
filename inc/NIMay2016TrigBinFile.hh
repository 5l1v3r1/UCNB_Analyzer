// Copyright 2016.  UT Battelle, LLC.
// This file is part of UCNB_Analyzer.
// See LICENSE.md included in top directory of this distribution.

// File: NIMay2016TrigBinFile.hh
// Purpose: Opens binary files in NI May 2016 trig format
 
#ifndef NI_MAY_TRIG_BIN_FILE_HH__
#define NI_MAY_TRIG_BIN_FILE_HH__

#include <vector>

#include "BinFile.hh"

using std::cout;
using std::endl;
using std::vector;

class NIMay2016TrigBinFile : public BinFile
{
public: 
  struct MayTrigBinEv_t : public BinEv_t{
    unsigned long long int timestamp;
    int board;
    short channel;
    short adc;
  };
  unsigned long long int starttimestamp;
private:
  bool ReadHeader();
  bool readheader;
public: 
	NIMay2016TrigBinFile();
	NIMay2016TrigBinFile(std::string path, std::string name);
	NIMay2016TrigBinFile(std::string pathname);
	~NIMay2016TrigBinFile();
	using BinFile::Open;
	bool Open(int filenum, int rionum = -1);
	bool ReadNextEvent(BinEv_t& NI_event);

};

#endif // __NI_MAY_TRIG_BIN_FILE_HH__
