// Copyright 2017.  UT-Battelle, LLC.
// This file is part of UCNB_Analyzer.
// See LICENSE.md included in top directory of this distribution.

// File: CommandParser.hh
// Purpose: Parse parameters from Analyzer
 
#ifndef COMMAND_PARSER_HH__
#define COMMAND_PARSER_HH__

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <cstring>

using std::cout;
using std::endl;


class CommandParser
{
private:
	bool doraw;
	bool dotrig;
	bool dotrap;
	bool dofit;
	bool docoinc;
	bool doave;
	bool docal;
	bool doshapescan;
	bool fileok;
	int filenum1;
	int filenum2;
	int fitthresh=-1;
	int trapthresh=-1;
	int decay=-1;
	int shaping=-1;
	int top=-1;
	int smpcoinc=-1;
	int scansrc=-1;
	int dataformat = 3;
	std::string path = "";
	std::string calpath = "";
public:
	CommandParser();
	~CommandParser() {};
	bool Parse(int argc, char **argv);
	bool FileOK() {return fileok;};
	bool DoRaw() {return doraw;};
	bool DoTrig() {return dotrig;};
	bool DoTrap() {return dotrap;};
	bool DoFit() {return dofit;};
	bool DoCoinc() {return docoinc;};
	bool DoAve() {return doave;};
	bool DoCal() {return docal;};
	bool DoShapeScan() {return doshapescan;};
	int File1() {return filenum1;};
	int File2() {return filenum2;};
	int FitThresh() {return fitthresh;};
	int TrapThresh() {return trapthresh;};
	int Decay() {return decay;};
	int Shaping() {return shaping;};
	int Top() {return top;};
	int SmpCoinc() {return smpcoinc;};
	int ScanSrc() {return scansrc;};
	int DataFormat() {return dataformat;};
	std::string Path() {return path;};
	std::string CalPath() {return calpath;};
};

#endif // COMMAND_PARSER_HH__
