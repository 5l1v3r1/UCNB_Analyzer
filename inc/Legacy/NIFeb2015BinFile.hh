// Copyright 2016.  Los Alamos National Security, LLC.
// This file is part of UCNB_Analyzer.
// This program is distributed under the terms of the GNU General Public License, version 2.0.  See LICENSE.md included in top directory of this distribution.

// File: NIFeb2015BinFile.hh
// Purpose: Opens binary files in NI February 2015 format
 
#ifndef NI_FEB15_BIN_FILE_HH__
#define NI_FEB15_BIN_FILE_HH__

#include <vector>

#include "BinFile.hh"


using std::cout;
using std::endl;
using std::vector;

class NIFeb2015BinFile: public BinFile
{
public:
  struct FebBinEvch_t {
    short Edaq;
    int wavelen;
    vector <short> wave;
  };
  struct FebBinEv_t : BinEv_t{
    long long int timestamp;
    vector <FebBinEvch_t> ch;
  };
private:
  bool ReadHeader();
  int date_len, comment_len;
  char date[100];
  char comment[500];
public:
	NIFeb2015BinFile();
	NIFeb2015BinFile(std::string path, std::string name);
	NIFeb2015BinFile(std::string pathname);
	~NIFeb2015BinFile();
	using BinFile::Open;
	bool Open(int filenum, int rionum);
	bool ReadNextEvent(BinEv_t& NI_event);  
};

#endif // __NI_FEB15_BIN_FILE_HH__
