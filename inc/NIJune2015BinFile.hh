// File: NIJune2015BinFile.hh
// Name: Leah Broussard
// Date: 2015/5/6
// Purpose: Opens binary files in NI June 2015 format
//
// Revision History:
// 2015/5/6:   LJB  Find and open file given run #
// 2015/5/7:   LJB  Open multiple RIO files, requires consecutive RIO 
//                  files starting with RIO0
// 2015/5/11:  LJB  Reads events into a NI_event structure
// 2015/7/15:  LJB  Specialized to June 2015 file format
// 2015/7/16:  LJB  Split methods: single file class
// 2015/11/21: LJB  Open(filenum)
 
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
	bool Open(int filenum, int rionum = -1); //better approach for overloading?
	bool ReadNextEvent(BinEv_t& NI_event);

};

#endif // __NI_JUN15_BIN_FILE_HH__
