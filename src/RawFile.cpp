// File: RawFile.cpp
// Name: Leah Broussard
// Date: 2015/5/6
// Purpose: Handles output files created by NI DAQ
//
// Revision History:
// 2015/5/6:  LJB  Find and open file given run #
// 2015/5/7:  LJB  Open multiple RIO files, requires consecutive RIO 
//                 files starting with RIO0
// 2015/5/11: LJB  Reads events into a NI_event structure

#ifndef RAW_FILE_CPP__
#define RAW_FILE_CPP__

#include "RawFile.hh"

/*************************************************************************/
//                           Constructors
/*************************************************************************/
RawFile::RawFile() {
  numchan = 0;
}

RawFile::RawFile(std::string path, std::string name) {
  Open(path,name);
}

RawFile::RawFile(std::string filename) {
  Open(filename);
}

RawFile::RawFile(int filenum) {
  Open(filenum);
}

/*************************************************************************/
//                             Destructor
/*************************************************************************/
RawFile::~RawFile() {
  Close();
}

/*************************************************************************/
//                                Open
/*************************************************************************/

bool RawFile::Open(std::string path, std::string name) {
  if (RIO[0].fFileStream.is_open())
    Close();
  std::string filename = path;
  filename.append("/");
  filename.append(name);
  RIO[0].fFileStream.open(filename.c_str(),std::ios::binary);
  if (RIO[0].fFileStream.is_open()) {
    fFilePath = path;
    fFileName = name;
    //open rest
    std::string filename2 = filename;
    std::size_t fpos = filename2.find("RIO") + 3;
    for (int rio=1;rio<NRIO;rio++) {
      std::ostringstream riostrm;
      riostrm << rio;
      filename2.replace(fpos,1,riostrm.str());
      RIO[rio].fFileStream.open(filename2.c_str(),std::ios::binary);
    }
    Scan();
  }
  else {
    fFilePath = "";
    fFileName = "";
  }
  return RIO[0].fFileStream.is_open();
}

bool RawFile::Open(std::string filename) {
  std::size_t pos = filename.rfind("/");
  if (pos!=std::string::npos) {
    std::string path = filename.substr(0,pos+1);
    std::string name = filename.substr(pos+1,filename.length());
    return Open(path, name);
  }
  else { //no path in filename
    //1. Check local directory
    //2. Check Files/ directory
    //3. Check INPUT_PATH defined in UCNConfig.hh
    //4. Check environmental variables (getenv("RAW_DATA_DIR"))
    std::string name = filename; 
    const int ntrypath = 7;
    std::string trypath[] = {".","file","File","files","Files",
			      INPUT_PATH/*,getenv("RAW_DATA_DIR")*/};
    int tp = 0;
    bool success = false;
    do {
      success = Open(trypath[tp],name);
    }while (++tp<ntrypath && !success);
    return success;
  }
}

bool RawFile::Open(int filenum) {
  char tempstr[255];
  sprintf(tempstr,"Run%04d_RIO0.bin",filenum);
  std::string filename = tempstr;
  return Open(filename);
}

/*************************************************************************/
//                               Close
/*************************************************************************/
void RawFile::Close() {
  for (int rio=0; rio<NRIO; rio++) {
    if (RIO[rio].fFileStream.is_open()) {
      fFileName = "";
      fFilePath = "";
      RIO[rio].fFileStream.close();
    }
  }
}

/*************************************************************************/
//                                Scan
/*************************************************************************/
bool RawFile::Scan() {
  numchan = 0;
  for (int rio=0; rio<NRIO; rio++) {
    if (RIO[rio].fFileStream.is_open()) {
      RIO[rio].fFileStream.seekg(0,RIO[rio].fFileStream.end);
      RIO[rio].filelength = RIO[rio].fFileStream.tellg();
      RIO[rio].fFileStream.seekg(0,RIO[0].fFileStream.beg);

      //Read config file?
      
      //Read Header
      RIO[rio].fFileStream.read(reinterpret_cast<char *>(&RIO[rio].date_len),sizeof(Int_t));
      RIO[rio].fFileStream.read(reinterpret_cast<char *>(RIO[rio].date),RIO[rio].date_len);
      RIO[rio].date[RIO[rio].date_len] = '\0';
      RIO[rio].fFileStream.read(reinterpret_cast<char *>(&RIO[rio].comment_len),sizeof(Int_t));
      RIO[rio].fFileStream.read(reinterpret_cast<char *>(RIO[rio].comment),RIO[rio].comment_len);
      RIO[rio].comment[RIO[rio].comment_len] = '\0';

      streampos pos = RIO[rio].fFileStream.tellg();
      //Read First Event
      Long64_t timestamp;
      Short_t Edaq;
      RIO[rio].fFileStream.read(reinterpret_cast<char *>(&timestamp),sizeof(Long64_t));
      RIO[rio].fFileStream.read(reinterpret_cast<char *>(&RIO[rio].numch),sizeof(Int_t));
      numchan += RIO[rio].numch;
      for (int e=0;e<RIO[rio].numch;e++)
	RIO[rio].fFileStream.read(reinterpret_cast<char *>(&RIO[rio].ch[e].Edaq),sizeof(Short_t));
      RIO[rio].fFileStream.read(reinterpret_cast<char *>(&RIO[rio].wavelen),sizeof(Int_t));
      RIO[rio].wavelen = RIO[rio].wavelen/8.;
      for (int e=0;e<RIO[rio].numch;e++)
        RIO[rio].ch[e].wave.resize(RIO[rio].wavelen);
      for (int e=0;e<RIO[rio].numch;e++)
	RIO[rio].fFileStream.read(reinterpret_cast<char *>(&RIO[rio].ch[e].wave[0]),RIO[rio].wavelen*sizeof(Short_t));

      RIO[rio].fFileStream.seekg(pos);
    }
  }

  //Scan the file
  /*
  while (pos < filelength) {
    fFileList[FAT].fFileStream.seekg(pos);
    packet_list.push_back(pos);
    output_header header;
    if (static_cast<size_t>(pos) + sizeof(header) > static_cast<size_t>(filelength))
      continue;
    if (!ReadHeader(header))
      continue;
    //cout << "Scanning: ch " << header.fadc_number << " time " << header.tv_usec << endl;
    pos = fFileList[FAT].fFileStream.tellg();
    pos += header.data_size;
    //if (packet_list.size() > 30) //LJB quick test
      //return true;
  }
  fFileList[FAT].fFileStream.seekg(0,fFileList[FAT].fFileStream.beg);
  */
  return true;
}
/*************************************************************************/
//                              ReadEvent
/*************************************************************************/
/*
bool RawFile::ReadEvent(int packetno, output_header& header, std::vector<Data_Block_t> &datablck) {
  fFileList[FAT].fFileStream.seekg(packet_list[packetno]);
  ReadEvent(header,datablck);
}
*/
bool RawFile::ReadEvent(ev_t &NI_event) {
  Long64_t ts[NRIO];
  Int_t wavelen, numch;
  for (int rio=0;rio<NRIO;rio++) {
    streampos pos = RIO[rio].fFileStream.tellg();
    if (pos >= RIO[rio].filelength) {
      return false;
    }
    RIO[rio].fFileStream.read(reinterpret_cast<char *>(&ts[rio]),sizeof(Long64_t));
    RIO[rio].fFileStream.read(reinterpret_cast<char *>(&numch),sizeof(Int_t));
    if (numch != RIO[rio].numch) {
      cout << "numch change for board " << rio << endl;
      return false;
    }
    NI_event.numch += numch;
    for (int ch=0;ch<RIO[rio].numch;ch++) {
      int thech = rio*RIO[rio].numch + ch;
      NI_event.ch[thech].rio = rio;
      NI_event.ch[thech].ch = ch;
      RIO[rio].fFileStream.read(reinterpret_cast<char *>(&NI_event.ch[thech].Edaq),sizeof(Short_t));
    }
    RIO[rio].fFileStream.read(reinterpret_cast<char *>(&wavelen),sizeof(Int_t));
    wavelen = wavelen/8.;
    if (wavelen != RIO[rio].wavelen) {
      cout << "wavelen change for board " << rio << endl;
      cout << wavelen << " != " << RIO[rio].wavelen << endl;
      return false;
    }
    for (int ch=0;ch<RIO[rio].numch;ch++) {
      int thech = rio*RIO[rio].numch + ch;
      NI_event.ch[thech].wavelen = wavelen;
      if (wavelen > MAXWAVE) {
	cout << "error, wavelen > MAXWAVE" << endl;
	cout << wavelen << " > " << MAXWAVE << endl;
	return false;
      }
      //NI_event.ch[thech].wave.resize(wavelen);
      RIO[rio].fFileStream.read(reinterpret_cast<char *>(&NI_event.ch[thech].wave[0]),wavelen*sizeof(Short_t));
    }
  }
  //For now, assume timestamps will match
  for (int rio=0;rio<NRIO-1;rio++) {
    if (rio < NRIO-1) {
      if (ts[rio] != ts[rio+1]) {
	cout << "Timestamp mismatch, " << ts[rio] << " != " << ts[rio+1] << endl;
	return false;
      }
    }
  }
  NI_event.timestamp = ts[0];

  return true;
}



#endif // __RAW_FILE_CPP__

