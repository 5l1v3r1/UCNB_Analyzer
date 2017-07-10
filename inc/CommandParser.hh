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
#include <vector>
#include <memory>

#include "Task.hh"

using std::cout;
using std::endl;
using std::vector;
using std::shared_ptr;


class CommandParser
{
private:
	//tasks
	bool doraw;
	bool dotrig;
	bool dotrap;
	bool dofit;
	bool docoinc;
	bool doave;
	bool docal;
	bool doshapescan;
	//parameters
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
	std::string calibfile = "";
public:
	CommandParser();
	~CommandParser() {};
	bool Parse(int argc, char **argv);
	void GetTasks(vector<std::shared_ptr<Task>> &tasklist);
private:
	bool ErrorCheck(char* name);
};

#endif // COMMAND_PARSER_HH__
