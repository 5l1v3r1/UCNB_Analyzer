// Copyright 2016.  UT Battelle, LLC.
// This file is part of UCNB_Analyzer.
// This program is distributed under the terms of the GNU General Public License, version 2.0.  See LICENSE.md included in top directory of this distribution.

// File: NIMay2016BinFile.hh
// Purpose: Opens binary files in NI May 2016 format

#ifndef NI_MAY16_BIN_FILE_CPP__
#define NI_MAY16_BIN_FILE_CPP__

#include "NIMay2016BinFile.hh"

/*************************************************************************/
//                           Constructors
/*************************************************************************/
NIMay2016BinFile::NIMay2016BinFile() {
}

NIMay2016BinFile::NIMay2016BinFile(std::string path, std::string name) {
  Open(path,name);
}

NIMay2016BinFile::NIMay2016BinFile(std::string filename) {
  Open(filename);
}


/*************************************************************************/
//                             Destructor
/*************************************************************************/
NIMay2016BinFile::~NIMay2016BinFile() {
  Close();
}

/*************************************************************************/
//                                Open   
/*************************************************************************/
bool NIMay2016BinFile::Open(int filenum, int rionum){
	char tempstr[255];
	sprintf(tempstr,"Run_%d_0.bin",filenum);
	std::string filename = tempstr;
    return Open(filename);
}

/*************************************************************************/
//                             ReadHeader
/*************************************************************************/
bool NIMay2016BinFile::ReadHeader() {
  if (!IsOpen()) 
    return false;
  fFileStream.read(reinterpret_cast<char *>(&(starttimestamp)),8);
  readheader = true;
}

/*************************************************************************/
//                             ReadEvent
/*************************************************************************/
bool NIMay2016BinFile::ReadNextEvent(BinEv_t& NI_event) {
  MayBinEv_t* May_event = dynamic_cast<MayBinEv_t*>(&NI_event);
  if (!May_event) {
    cout << "NIMay2016BinFile::ReadNextEvent() requires MayBinEv_t types" << endl;
    return false;
  }

  if (!IsOpen())
    return false;
  if (!readheader)
    ReadHeader();
  if (!CheckLength())
    return false;

  May_event->result = 0;
  fFileStream.read(reinterpret_cast<char *>(&(May_event->result)),1);
  fFileStream.read(reinterpret_cast<char *>(&(May_event->eventID)),4);
  fFileStream.read(reinterpret_cast<char *>(&(May_event->board)),4);
  fFileStream.read(reinterpret_cast<char *>(&(May_event->channel)),4);
  fFileStream.read(reinterpret_cast<char *>(&(May_event->timestamp)),8);
  int wavelen;
  fFileStream.read(reinterpret_cast<char *>(&wavelen),4);
  if (wavelen > MAXWAVE) {
    cout << endl;
    cout << GetPosition() << endl;
    cout << May_event->result << endl;
    cout << May_event->eventID << endl;
    cout << May_event->board << endl;
    cout << May_event->channel << endl;
    cout << May_event->timestamp << endl;
    cout << "Error in wavelength " << wavelen << endl;
    return false;
  }
  May_event->wave.resize(wavelen);
  if (!fFileStream.read(reinterpret_cast<char *>(&(May_event->wave[0])),wavelen*2))
    return false;
  return true;
}



#endif // __NI_MAY16_BIN_FILE_CPP__


