// Copyright 2017.  UT-Battelle, LLC.
// This file is part of UCNB_Analyzer.
// This program is distributed under the terms of the GNU General Public License, version 2.0.  See LICENSE.md included in top directory of this distribution.

// File: NIMay2017BinFile.hh
// Purpose: Opens binary files in NI May 2017 format

#ifndef NI_MAY17_BIN_FILE_CPP__
#define NI_MAY17_BIN_FILE_CPP__

#include "NIMay2017BinFile.hh"

/*************************************************************************/
//                           Constructors
/*************************************************************************/
NIMay2017BinFile::NIMay2017BinFile() {
}

NIMay2017BinFile::NIMay2017BinFile(std::string path, std::string name) {
  Open(path,name);
}

NIMay2017BinFile::NIMay2017BinFile(std::string filename) {
  Open(filename);
}


/*************************************************************************/
//                             Destructor
/*************************************************************************/
NIMay2017BinFile::~NIMay2017BinFile() {
  Close();
}

/*************************************************************************/
//                                Open   
/*************************************************************************/
bool NIMay2017BinFile::Open(int filenum, int rionum){
	char tempstr[255];
	sprintf(tempstr,"Run_%d_0.bin",filenum); //THIS FORMAT IS SO STUPIDDDDDDDD
	std::string filename = tempstr;
    return Open(filename);
}

/*************************************************************************/
//                             ReadHeader
/*************************************************************************/
bool NIMay2017BinFile::ReadHeader() {
  if (!IsOpen()) 
    return false;
  char dummy = '0';
  int i = 0;
  fFileStream.seekg(8,fFileStream.beg);
  /*
  while (dummy != 'M' && GetPosition()<25) {
	fFileStream.read(&dummy,1);
  }
  if (GetPosition()==25)
	Reset();
	*/
	
  readheader = true;
}

/*************************************************************************/
//                             ReadEvent
/*************************************************************************/
bool NIMay2017BinFile::ReadNextEvent(BinEv_t& NI_event) {
  MayBinEv_t* May_event = dynamic_cast<MayBinEv_t*>(&NI_event);
  if (!May_event) {
    cout << "NIMay2017BinFile::ReadNextEvent() requires MayBinEv_t types" << endl;
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
  fFileStream.read(reinterpret_cast<char *>(&(May_event->Req)),8);
  fFileStream.read(reinterpret_cast<char *>(&(May_event->Curr)),8);
//  fFileStream.read(reinterpret_cast<char *>(&(May_event->timestamp)),8);
  int wavelen;
  fFileStream.read(reinterpret_cast<char *>(&wavelen),4);
  if (wavelen > MAXWAVE) {
    cout << endl;
    cout << GetPosition() << endl;
    cout << May_event->result << endl;
    cout << May_event->eventID << endl;
    cout << May_event->board << endl;
    cout << May_event->channel << endl;
    cout << May_event->Req << endl;
    cout << May_event->Curr << endl;
    cout << May_event->timestamp << endl;
    cout << "Error in wavelength " << wavelen << endl;
    return false;
  }
  May_event->wave.resize(wavelen);
  if (!fFileStream.read(reinterpret_cast<char *>(&(May_event->wave[0])),wavelen*2))
    return false;
  return true;
}



#endif // __NI_MAY17_BIN_FILE_CPP__


