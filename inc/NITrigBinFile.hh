// Copyright 2016.  UT Battelle, LLC.
// This file is part of UCNB_Analyzer.
// See LICENSE.md included in top directory of this distribution.

// File: NITrigBinFile.hh
// Purpose: Opens binary files in NI trig format
 
#ifndef NI_TRIG_BIN_FILE_HH__
#define NI_TRIG_BIN_FILE_HH__

#include <vector>

#include "BinFile.hh"

using std::cout;
using std::endl;
using std::vector;

class NITrigBinFile : public BinFile
{
public: 
  struct TrigBinEv_t : public BinEv_t{
    unsigned long long int timestamp;
    int board;
    short channel;
    short adc;
  };
private:
  bool ReadHeader();
  bool readheader;
public: 
	NITrigBinFile();
	NITrigBinFile(std::string path, std::string name);
	NITrigBinFile(std::string pathname);
	~NITrigBinFile();
	using BinFile::Open;
	bool Open(int filenum, int rionum = -1);
	bool ReadNextEvent(BinEv_t& NI_event);

};

#endif // __NI_TRIG_BIN_FILE_HH__
