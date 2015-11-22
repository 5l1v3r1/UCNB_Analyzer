// File: NIFeb2015BinFile.hh
// Name: Leah Broussard
// Date: 2015/5/6
// Purpose: Opens binary files in NI February 2015 format
//
// Revision History:
// 2015/5/6:   LJB   Find and open file given run #
// 2015/5/7:   LJB   Open multiple RIO files, requires consecutive RIO 
//                   files starting with RIO0
// 2015/5/11:  LJB   Reads events into a NI_event structure
// 2015/7/15:  LJB   Specialized to February 2015 file format
// 2015/7/16:  LJB   Split methods: single file classclass
// 2015/11/21: LJB   Open(filenum)
 
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
  bool readheader;
  //FebBinEv_t NI_event;
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
