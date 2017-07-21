// Copyright 2017.  UT-Battelle, LLC.
// This file is part of UCNB_Analyzer.
// See LICENSE.md included in top directory of this distribution.

// File: Ca45CommandParser.hh
// Purpose: Parse parameters from Analyzer
 
#ifndef CA45_COMMAND_PARSER_HH__
#define CA45_COMMAND_PARSER_HH__

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


class Ca45CommandParser
{
private:
	//tasks
	bool doreplay;
	bool dotrig;
	bool dotrap;
	bool dofit;
	bool docoinc;
	bool doave;
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
	std::string path = "";
	std::string calibfile = "";
public:
	Ca45CommandParser();
	~Ca45CommandParser() {};
	bool Parse(int argc, char **argv);
	void GetTasks(vector<std::shared_ptr<Task>> &tasklist);
private:
	bool ErrorCheck(char* name);
};

#endif // CA45_COMMAND_PARSER_HH__
