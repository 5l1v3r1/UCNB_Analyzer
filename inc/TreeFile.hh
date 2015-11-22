// File: TreeFile.hh
// Name: Leah Broussard
// Date: 2015/5/8
// Purpose: Opens ROOT TTree files
//
// Revision History:
// 2015/5/8:   LJB  Create
// 2015/5/11:  LJB  Read/write ROOT files with 
// 2015/11/21: LJB  Open(filenum)
 
#ifndef TREE_FILE_HH__
#define TREE_FILE_HH__

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>

#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"

#include "LocalCFG.hh"
#include "BinFile.hh"

using std::cout;
using std::endl;
using std::vector;

class TreeFile
{
protected:
  TFile* RootFile;
  TTree* RootTree;
  bool createmode;
  std::string mypath;
  bool pathset;
public:
  TreeFile();
  ~TreeFile();
  void Close();
  bool Open(std::string path, std::string name);
  bool Open(std::string filename);
  virtual bool Open(int filenum) {};
  bool IsOpen();
#if !defined (__CINT__)
  bool Create(std::string path, std::string name);
  bool Create(std::string filename);
  virtual bool Create(int filenum) {};
#endif // !defined (__CINT__)
  void FillTree();
  void Write();
  Int_t GetNumEvents(){return RootTree->GetEntries();};
  void GetEvent(Int_t ev);
  void SetPath(std::string newpath) {mypath = newpath; pathset = true;}
};

#endif // TREE_FILE_JUNE_HH__
