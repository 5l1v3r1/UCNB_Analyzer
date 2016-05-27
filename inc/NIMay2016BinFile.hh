// Copyright 2016.  UT Battelle, LLC.
// This file is part of UCNB_Analyzer.
// This program is distributed under the terms of the GNU General Public License, version 2.0.  See LICENSE.md included in top directory of this distribution.

// File: NIMay2016BinFile.hh
// Purpose: Opens binary files in NI May 2016 format
 
#ifndef NI_MAY16_BIN_FILE_HH__
#define NI_MAY16_BIN_FILE_HH__

#include <vector>

#include "BinFile.hh"

using std::cout;
using std::endl;
using std::vector;

class NIMay2016BinFile : public BinFile
{
public: 
  struct MayBinEv_t : public BinEv_t{
    short result;
    int eventID;
    int board;
    int channel;
    unsigned long long int timestamp;
    vector<short> wave;
  };
  unsigned long long int starttimestamp;
private:
  bool ReadHeader();
public: 
	NIMay2016BinFile();
	NIMay2016BinFile(std::string path, std::string name);
	NIMay2016BinFile(std::string pathname);
	~NIMay2016BinFile();
	using BinFile::Open;
	bool Open(int filenum, int rionum = -1);
	bool ReadNextEvent(BinEv_t& NI_event);

};

#endif // __NI_MAY16_BIN_FILE_HH__
