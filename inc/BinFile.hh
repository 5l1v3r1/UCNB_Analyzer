// File: BinFile.hh
// Name: Leah Broussard
// Date: 2015/7/16
// Purpose: Opens binary files
//
// Revision History:
// 2015/7/16:  LJB  Created
// 2015/11/21: LJB  SetPath and Open(filenum) methods
 
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

#define MAXWAVE 6000
#define MAXCH 8
#define MAXRIO 16

class BinFile
{
private:
  std::string fFilePath;
  std::string fFileName; 
  std::streampos fFileLength;
protected:
  std::ifstream fFileStream;
  std::string mypath;
  bool pathset;
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
	virtual bool Open(int filenum, int rionum=-1) {}
	void Close();
	inline bool IsOpen(){return fFileStream.is_open();}
	bool CheckLength();
	std::streampos GetPosition();
	std::streampos GetLength() {return fFileLength;}
	virtual bool ReadNextEvent(BinEv_t& NI_event) {}
	void SetPath(std::string newpath) {mypath = newpath; pathset = true;}
};

#endif // __BIN_FILE_HH__
