// Copyright 2017.  UT-Battelle, LLC.
// This file is part of UCNB_Analyzer.
// See LICENSE.md included in top directory of this distribution.

// File: BuildTemplateWaveform.hh
// Purpose: Analysis task: create pulse template from average of waveforms
 
#ifndef BUILD_TEMPLATE_WAVEFORM_HH__
#define BUILD_TEMPLATE_WAVEFORM_HH__

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>

#include "Task.hh"

using std::cout;
using std::endl;
using std::vector;

class BuildTemplateWaveform : public Task
{
private:
	int decay;
	int shaping;
	int top;
	std::string pathin;
	std::string pathout;
public:
	BuildTemplateWaveform();
	BuildTemplateWaveform(int decay, int shaping, int top, std::string pin, std::string pout);
	~BuildTemplateWaveform();
	void Set(int decay, int shaping, int top, std::string pin, std::string pout);
	void Go(int filenum);
};

#endif //ifndef BUILD_TEMPLATE_WAVEFORM_HH__
