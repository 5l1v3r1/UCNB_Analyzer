// Copyright 2016.  UT-Battelle, LLC.
// This file is part of UCNB_Analyzer.
// See LICENSE.md included in top directory of this distribution.

// File: NITrigBinFile.hh
// Purpose: Opens binary files in NI trig format

#ifndef NI_TRIG_BIN_FILE_CPP__
#define NI_TRIG_BIN_FILE_CPP__

#include "NITrigBinFile.hh"

/*************************************************************************/
//                           Constructors
/*************************************************************************/
NITrigBinFile::NITrigBinFile() {
}

NITrigBinFile::NITrigBinFile(std::string path, std::string name) {
  Open(path,name);
}

NITrigBinFile::NITrigBinFile(std::string filename) {
  Open(filename);
}


/*************************************************************************/
//                             Destructor
/*************************************************************************/
NITrigBinFile::~NITrigBinFile() {
  Close();
}

/*************************************************************************/
//                                Open   
/*************************************************************************/
bool NITrigBinFile::Open(int filenum, int rionum){
	char tempstr[255];
	sprintf(tempstr,"Run_%d.trig",filenum);
	std::string filename = tempstr;
    return Open(filename);
}

/*************************************************************************/
//                             ReadHeader
/*************************************************************************/
bool NITrigBinFile::ReadHeader() {
  if (!IsOpen()) 
    return false;
  char dummy = '0';
  int i = 0;
  while (dummy != 'M' && GetPosition()<25) {
	fFileStream.read(&dummy,1);
  }
  if (GetPosition()==25)
	Reset();
  readheader = true;
}

/*************************************************************************/
//                             ReadEvent
/*************************************************************************/
bool NITrigBinFile::ReadNextEvent(BinEv_t& NI_event) {
  TrigBinEv_t* Trig_event = dynamic_cast<TrigBinEv_t*>(&NI_event);
  if (!Trig_event) {
    cout << "NITrigBinFile::ReadNextEvent() requires TrigBinEv_t types" << endl;
    return false;
  }

  if (!IsOpen())
    return false;
  if (!readheader)
    ReadHeader();
  if (!CheckLength())
    return false;

  fFileStream.read(reinterpret_cast<char *>(&(Trig_event->board)),4);
  fFileStream.read(reinterpret_cast<char *>(&(Trig_event->channel)),1);
  fFileStream.read(reinterpret_cast<char *>(&(Trig_event->timestamp)),8);
  fFileStream.read(reinterpret_cast<char *>(&(Trig_event->adc)),2);
  
  return true;
}



#endif // __NI_TRIG_BIN_FILE_CPP__


