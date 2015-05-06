// File: RawFile.hh
// Name: Leah Broussard
// Date: 2015/5/6
// Purpose: Handles output files created by NI DAQ
//
// Revision History:
// 2015/5/6:  LJB  Find and open file given run #
 
#ifndef RAW_FILE_HH__
#define RAW_FILE_HH__

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include "Rtypes.h"
#include "LocalCFG.hh"

using std::cout;
using std::endl;
using std::vector;
using std::streampos;


class RawFile
{

private:
  std::string fFilePath;
  std::string fFileName;
  std::ifstream fFileStream;
  streampos filelength;
  //  vector<streampos> packet_list;
  //  void FillDataBlocks(UChar_t buffer[RAWDATA_LENGTH],int size,std::vector<Data_Block_t> &blck);
  //  bool CheckStream(std::ifstream& stream);
  //  void SwapEndian(UInt_t& swapme, int bytes);

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
  //  bool ReadEvent(int packetno, output_header& header, std::vector<Data_Block_t> &datablck);
  //  bool ReadEvent(output_header& header, std::vector<Data_Block_t> &datablck);
  //  bool ReadHeader(output_header& header);
  //  bool ReadData(Int_t datasize, std::vector<Data_Block_t> &datablck);
  //  void PrintHeader(output_header header);
  inline bool IsOpen(){return fFileStream.is_open();}
  //  inline int GetNumPackets(){return packet_list.size();}
};

#endif // __RAW_FILE_HH__
