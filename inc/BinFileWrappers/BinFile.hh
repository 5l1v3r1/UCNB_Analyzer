// Copyright 2016.  Los Alamos National Security, LLC.
// This file is part of UCNB_Analyzer.
// This program is distributed under the terms of the GNU General Public License, version 2.0.  See LICENSE.md included in top directory of this distribution.

// File: BinFile.hh
// Purpose: Opens and reads events from binary files
 
#ifndef BIN_FILE_HH__
#define BIN_FILE_HH__

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using std::cout;
using std::endl;

#define MAXWAVE 65536
#define MAXCH 8
#define MAXRIO 6

class BinFile
{
private:
  std::string fFilePath;
  std::string fFileName; 
  std::streampos fFileLength;
protected:
  std::ifstream fFileStream;
  std::string mypath;
  int fileno;
  bool pathset;
  bool readheader;
public:
	struct BinEv_t{
		virtual ~BinEv_t(){}
	};
	BinFile();
	BinFile(std::string path, std::string name);
	BinFile(std::string pathname);
	~BinFile();
	void init();
	bool Open(const char* path, const char* name);
	bool Open(std::string path, std::string name);
	bool Open(std::string filename);
	virtual bool Open(int filenum, int rionum=-1) {return false;}
    virtual bool Open(std::string path, int filenum, int rionum=-1) {return false;}
	void Close();
	inline bool IsOpen(){return fFileStream.is_open();}
	bool CheckLength();
	void Reset() {if (IsOpen()) {fFileStream.seekg(0,fFileStream.beg);}}
	std::streampos GetPosition();
	std::streampos GetLength() {return fFileLength;}
        virtual bool ReadNextEvent(BinEv_t& NI_event) {return false;}
	void SetPath(std::string newpath) {mypath = newpath; pathset = true;}
};

#endif // __BIN_FILE_HH__
