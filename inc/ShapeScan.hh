// Copyright 2017.  UT-Battelle, LLC.
// This file is part of UCNB_Analyzer.
// See LICENSE.md included in top directory of this distribution.

// File: ShapeScan.hh
// Purpose: Analysis task: perform automatic calibration using UCNB Jan2016 Data.  Work in progress.
 
#ifndef SHAPE_SCAN_HH__
#define SHAPE_SCAN_HH__

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>

#include "Task.hh"

using std::cout;
using std::endl;
using std::vector;

class ShapeScan : public Task
{
private:
	int src;
	std::string pathin;
	std::string pathout;
public:
	ShapeScan();
	ShapeScan(int src, std::string pin, std::string pout);
	~ShapeScan();
	void Set(int src, std::string pin, std::string pout);
	void Go();
};

#endif //ifndef SHAPE_SCAN_HH__
