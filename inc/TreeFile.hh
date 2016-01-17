// Copyright 2016.  Los Alamos National Security, LLC.
// This file is part of UCNB_Analyzer.
// This program is distributed under the terms of the GNU General Public License, version 2.0.  See LICENSE.md included in top directory of this distribution.

// File: TreeFile.hh
// Purpose: Opens ROOT TTree files
 
#ifndef TREE_FILE_HH__
#define TREE_FILE_HH__

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>

#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"

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
  char namestr[25];
public:
  TreeFile();
  ~TreeFile();
  void Close();
  bool Open(std::string path, std::string name);
  bool Open(std::string filename);
  bool Open(int filenum);
  bool IsOpen();
#if !defined (__CINT__)
  bool Create(std::string path, std::string name);
  bool Create(std::string filename);
  bool Create(int filenum);
#endif // !defined (__CINT__)
  void FillTree();
  void Write();
  Int_t GetNumEvents(){return RootTree->GetEntries();};
  void GetEvent(Int_t ev);
  void SetPath(std::string newpath) {mypath = newpath; pathset = true;}
protected:
  virtual void SetNameStr() {};
  virtual void SetBranches() {};
  virtual void MakeBranches() {}; 
  
};

#endif // TREE_FILE_JUNE_HH__
