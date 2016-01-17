// Copyright 2016.  Los Alamos National Security, LLC.
// This file is part of UCNB_Analyzer.
// This program is distributed under the terms of the GNU General Public License, version 2.0.  See LICENSE.md included in top directory of this distribution.

// File: NIJune2015BinFile.hh
// Purpose: Opens binary files in NI June 2015 format
 
#ifndef NI_JUN15_BIN_FILE_HH__
#define NI_JUN15_BIN_FILE_HH__

#include <vector>

#include "BinFile.hh"

using std::cout;
using std::endl;
using std::vector;

class NIJune2015BinFile : public BinFile
{
public: 
  struct JuneBinEv_t : public BinEv_t{
    short result;
    int eventID;
    int board;
    int channel;
    unsigned long long int timestamp;
    vector<short> wave;
  };
public: 
	NIJune2015BinFile();
	NIJune2015BinFile(std::string path, std::string name);
	NIJune2015BinFile(std::string pathname);
	~NIJune2015BinFile();
	using BinFile::Open;
	bool Open(int filenum, int rionum = -1);
	bool ReadNextEvent(BinEv_t& NI_event);

};

#endif // __NI_JUN15_BIN_FILE_HH__
