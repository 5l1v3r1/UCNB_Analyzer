// Copyright 2016.  UT-Battelle, LLC.
// This file is part of UCNB_Analyzer.
// See LICENSE.md included in top directory of this distribution.

// File: NIDec2015TrigBinFile.hh
// Purpose: Opens binary files in NI trig format

#ifndef NI_DEC_TRIG_BIN_FILE_CPP__
#define NI_DEC_TRIG_BIN_FILE_CPP__

#include "NIDec2015TrigBinFile.hh"

/*************************************************************************/
//                           Constructors
/*************************************************************************/
NIDec2015TrigBinFile::NIDec2015TrigBinFile() {
}

NIDec2015TrigBinFile::NIDec2015TrigBinFile(std::string path, std::string name) {
  Open(path,name);
}

NIDec2015TrigBinFile::NIDec2015TrigBinFile(std::string filename) {
  Open(filename);
}


/*************************************************************************/
//                             Destructor
/*************************************************************************/
NIDec2015TrigBinFile::~NIDec2015TrigBinFile() {
  Close();
}

/*************************************************************************/
//                                Open   
/*************************************************************************/
bool NIDec2015TrigBinFile::Open(int filenum, int rionum){
	char tempstr[255];
	sprintf(tempstr,"Run_%d.trig",filenum);
	std::string filename = tempstr;
    return Open(filename);
}

/*************************************************************************/
//                             ReadHeader
/*************************************************************************/
bool NIDec2015TrigBinFile::ReadHeader() {
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
bool NIDec2015TrigBinFile::ReadNextEvent(BinEv_t& NI_event) {
  DecTrigBinEv_t* Trig_event = dynamic_cast<DecTrigBinEv_t*>(&NI_event);
  if (!Trig_event) {
    cout << "NIDec2015TrigBinFile::ReadNextEvent() requires TrigBinEv_t types" << endl;
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



#endif // __NI_DEC_TRIG_BIN_FILE_CPP__


