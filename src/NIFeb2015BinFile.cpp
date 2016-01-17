// Copyright 2016.  Los Alamos National Security, LLC.
// This file is part of UCNB_Analyzer.
// This program is distributed under the terms of the GNU General Public License, version 2.0.  See LICENSE.md included in top directory of this distribution.

// File: NIFeb2015BinFile.cpp
// Purpose: Opens binary files in NI February 2015 format

#ifndef NI_FEB15_BIN_FILE_CPP__
#define NI_FEB15_BIN_FILE_CPP__

#include "NIFeb2015BinFile.hh"

/*************************************************************************/
//                           Constructors
/*************************************************************************/
NIFeb2015BinFile::NIFeb2015BinFile() {
  readheader = false;
}

NIFeb2015BinFile::NIFeb2015BinFile(std::string path, std::string name) {
  readheader = false;
  Open(path,name);
}

NIFeb2015BinFile::NIFeb2015BinFile(std::string filename) {
  readheader = false;
  Open(filename);
}

/*************************************************************************/
//                             Destructor
/*************************************************************************/
NIFeb2015BinFile::~NIFeb2015BinFile() {
  Close();
}


/*************************************************************************/
//                                Open   
/*************************************************************************/
bool NIFeb2015BinFile::Open(int filenum, int rionum){
	char tempstr[255];
	sprintf(tempstr,"Run%04d_RIO%d.bin",filenum,rionum);
	std::string filename = tempstr;
	return Open(filename);
}

/*************************************************************************/
//                             ReadHeader
/*************************************************************************/
bool NIFeb2015BinFile::ReadHeader() {
  if (!IsOpen()) 
    return false;
  fFileStream.read(reinterpret_cast<char *>(&date_len),4);
  fFileStream.read(reinterpret_cast<char *>(date),date_len);
  date[date_len] = '\0';
  fFileStream.read(reinterpret_cast<char *>(&comment_len),4);
  fFileStream.read(reinterpret_cast<char *>(comment),comment_len);
  comment[comment_len] = '\0';

  readheader = true;
}

/*************************************************************************/
//                             ReadEvent
/*************************************************************************/
bool NIFeb2015BinFile::ReadNextEvent(BinEv_t& NI_event) {
  FebBinEv_t* Feb_event = dynamic_cast<FebBinEv_t*>(&NI_event);
  if (!Feb_event) {
    cout << "NIFeb2015BinFile::ReadNextEvent() requires FebBinEv_t types" << endl;
    return false;
  }

  if (!IsOpen()) 
    return false;
  if (!readheader)
    ReadHeader();
  if (!CheckLength())
    return false;
  fFileStream.read(reinterpret_cast<char *>(&(Feb_event->timestamp)),8);
  int numch;
  fFileStream.read(reinterpret_cast<char *>(&numch),4);  
  Feb_event->ch.resize(numch);
  for (int ch=0;ch<numch;ch++)
    fFileStream.read(reinterpret_cast<char *>(&(Feb_event->ch[ch].Edaq)),2);
  int wavelen;
  fFileStream.read(reinterpret_cast<char *>(&wavelen),4);
  wavelen = wavelen/8.;
  if (wavelen > MAXWAVE) {
    cout << endl;
    cout << GetPosition() << endl;
    for (int i=0;i<Feb_event->ch.size();i++)
      cout << Feb_event->ch[i].Edaq << " ";
    cout << endl;
    cout << Feb_event->timestamp << endl;
    cout << "Error in wavelength " << wavelen << endl;
    return false;
  }
  for (int ch=0;ch<numch;ch++) {
    Feb_event->ch[ch].wave.resize(wavelen);
    fFileStream.read(reinterpret_cast<char *>(&(Feb_event->ch[ch].wave[0])),wavelen*2);    
  }
  return true;

}



#endif // __NI_FEB15_BIN_FILE_CPP__


