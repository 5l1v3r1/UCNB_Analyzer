// File: RawFile.hh
// Name: Leah Broussard
// Date: 2015/5/6
// Purpose: Handles output files created by NI DAQ
//
// Revision History:
// 2015/5/6:  LJB  Find and open file given run #
// 2015/5/7:  LJB  Open multiple RIO files, requires consecutive RIO 
//                 files starting with RIO0
// 2015/5/10: LJB  Reads events into an NI_event structure
 
#ifndef RAW_FILE_HH__
#define RAW_FILE_HH__

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <math.h>
#include "Rtypes.h"
#include "LocalCFG.hh"
#include "EvType.hh"

using std::cout;
using std::endl;
using std::vector;
using std::streampos;

class RawFile
{
private:
  std::string fFilePath;
  std::string fFileName; //RIO0
  struct RIOch_t {
    Short_t Edaq;
    vector<Short_t> wave;
  };
  struct RIO_t{
    std::ifstream fFileStream;
    vector<streampos> ev;
    streampos filelength;
    Int_t date_len, comment_len;
    Int_t numch;
    char date[100];
    char comment[500];
    Int_t wavelen;
    RIOch_t ch[MAXCH];
  };
  RIO_t RIO[NRIO];
  streampos currentpos;
  Int_t numchan;

public:

  RawFile();
  RawFile(std::string path, std::string name);
  RawFile(std::string pathname);
  RawFile(int filenum);
  ~RawFile();
  bool Open(std::string path, std::string name);
  bool Open(std::string pathname);
  bool Open(int filenum);
  void Close();
  bool Scan();
  bool ReadEvent(ev_t &NI_event);
  inline bool IsOpen(){return RIO[0].fFileStream.is_open();}
  Int_t GetNumCh(){return numchan;}
  //  double GetPercent(){return (double)currentpos/(double)RIO[0].filelength;}
};

#endif // __RAW_FILE_HH__
