// Copyright 2016.  UT-Battelle, LLC.
// This file is part of UCNB_Analyzer.
// See LICENSE.md included in top directory of this distribution.

// File: NIMay2016TrigBinFile.hh
// Purpose: Opens binary files in NI trig format

#ifndef NI_MAY_TRIG_BIN_FILE_CPP__
#define NI_MAY_TRIG_BIN_FILE_CPP__

#include "NIMay2016TrigBinFile.hh"

/*************************************************************************/
//                           Constructors
/*************************************************************************/
NIMay2016TrigBinFile::NIMay2016TrigBinFile() {
}

NIMay2016TrigBinFile::NIMay2016TrigBinFile(std::string path, std::string name) {
  Open(path,name);
}

NIMay2016TrigBinFile::NIMay2016TrigBinFile(std::string filename) {
  Open(filename);
}


/*************************************************************************/
//                             Destructor
/*************************************************************************/
NIMay2016TrigBinFile::~NIMay2016TrigBinFile() {
  Close();
}

/*************************************************************************/
//                                Open   
/*************************************************************************/
bool NIMay2016TrigBinFile::Open(int filenum, int rionum){
	char tempstr[255];
	sprintf(tempstr,"Run_%d.trig",filenum);
	std::string filename = tempstr;
    return Open(filename);
}

/*************************************************************************/
//                             ReadHeader
/*************************************************************************/
bool NIMay2016TrigBinFile::ReadHeader() {
  if (!IsOpen()) 
    return false;
  fFileStream.read(reinterpret_cast<char *>(&(starttimestamp)),8);
  readheader = true;
}

/*************************************************************************/
//                             ReadEvent
/*************************************************************************/
bool NIMay2016TrigBinFile::ReadNextEvent(BinEv_t& NI_event) {
  MayTrigBinEv_t* Trig_event = dynamic_cast<MayTrigBinEv_t*>(&NI_event);
  if (!Trig_event) {
    cout << "NIMay2016TrigBinFile::ReadNextEvent() requires TrigBinEv_t types" << endl;
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



#endif // __NI_MAY_TRIG_BIN_FILE_CPP__


