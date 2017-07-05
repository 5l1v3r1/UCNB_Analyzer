// Copyright 2017.  UT-Battelle, LLC.
// This file is part of UCNB_Analyzer.
// See LICENSE.md included in top directory of this distribution.

// File: CommandParser.cpp
// Purpose: Parse parameters from Analyzer
 

#ifndef COMMAND_PARSER_CPP__
#define COMMAND_PARSER_CPP__

#include "CommandParser.hh"
#include "TSystem.h"

// dataformat:  0 = feb15, 1 = june15, 2 = may16, 3 = may17
const int maxformat = 4; // 4 formats so far...
void Usage(std::string program);

/*************************************************************************/
//                              Constructor
/*************************************************************************/
CommandParser::CommandParser() {
	doraw = false, dotrig = false, dotrap = false, dofit = false, docoinc = false, doave = false, docal = false, doshapescan = false;
	fileok = false;
	filenum1, filenum2, fitthresh=-1, trapthresh=-1, decay=-1, shaping=-1, top=-1, smpcoinc=-1, scansrc=-1;
	dataformat = 3;
	path = "";
	std::string calpath = "";
}

/*************************************************************************/
//                              Parse
/*************************************************************************/
bool CommandParser::Parse(int argc, char **argv) {
	int i=1;
	while (i+1 <= argc) {
    if ((strcmp(argv[i],"-p")==0)||(strcmp(argv[i],"-path")==0)) {
		i++;
		if (i+1 > argc) {
			cout << "Missing argument for -p" << endl;
			Usage(argv[0]);
			return false;
		}
		if (argv[i][0] == '-') {
			cout << "Missing valid argument for -p" << endl;
			Usage(argv[0]);
			return false;
		}
		path += argv[i];
		i++;
		if (gSystem->AccessPathName(path.c_str())) {
			cout << "bad path: " << path << " not found " << endl;
			return false;
		}
	}
    else if (strcmp(argv[i],"-calpath")==0) {
		i++;
		if (i+1 > argc) {
			cout << "Missing argument for -cailbfile" << endl;
			Usage(argv[0]);
			return false;
		}
		if (argv[i][0] == '-') {
			cout << "Missing valid argument for -calpath" << endl;
			Usage(argv[0]);
			return false;
		}
		calpath += argv[i];
		i++;
		if (gSystem->AccessPathName(calpath.c_str())) {
			cout << "bad file: " << path << " not found " << endl;
			return false;
		}
	}
    else if (strcmp(argv[i],"-f")==0) {
      i++;
      if (i+1 > argc) {
	cout << "Missing argument for -f" << endl;
	Usage(argv[0]);
	return false;
      }
      if (!(argv[i][0] >= '0' && argv[i][0] <= '9')) {
	cout << "Missing valid argument for -f" << endl;
	Usage(argv[0]);
	return false;
      }
      filenum1 = atoi(argv[i]);
      filenum2 = filenum1;
      i++;
      if (i+1 <= argc) {
	if (argv[i][0] >= '0' && argv[i][0] <= '9') {
	  filenum2 = atoi(argv[i]);
	  i++;
	}
      }
      fileok = true;
    }
    else if (strcmp(argv[i],"-raw")==0) {
      doraw = true;
      i++;
    }
    else if (strcmp(argv[i],"-format")==0) {
      i++;
      if (i+1 > argc) {
	cout << "Missing argument for -format" << endl;
	Usage(argv[0]);
	return false;
      }
      dataformat = atoi(argv[i]);
      if (!(argv[i][0] >= '0' && argv[i][0] <= '9')) {
	cout << "Missing valid argument for -format" << endl;
	Usage(argv[0]);
	return false;
      }
      if (!(dataformat >= 0 && dataformat < maxformat)) {
	cout << "Missing valid argument for -format" << endl;
	Usage(argv[0]);
	return false;
      }
      i++;
    }
    else if (strcmp(argv[i],"-trig")==0) {
      dotrig = true;
      i++;
    }
    else if (strcmp(argv[i],"-fit")==0) {
      dofit = true;
      i++;
      if (i+1 > argc) {
	cout << "Missing argument for -fit" << endl;
	Usage(argv[0]);
	return false;
      }
      if (!(argv[i][0] >= '0' && argv[i][0] <= '9')) {
	cout << "Missing valid argument for -fit" << endl;
	Usage(argv[0]);
	return false;
      }
      fitthresh = atoi(argv[i]);
      i++;
    }
    else if (strcmp(argv[i],"-trap")==0) {
      dotrap = true;
      i++;
      if (i+1 > argc) {
	cout << "Missing argument for -trap" << endl;
	Usage(argv[0]);
	return false;
      }
      if (!(argv[i][0] >= '0' && argv[i][0] <= '9')) {
	cout << "Missing valid argument for -trap" << endl;
	Usage(argv[0]);
	return false;
      }
      trapthresh = atoi(argv[i]);
      i++;
    }
    else if (strcmp(argv[i],"-decay")==0) {
      i++;
      if (i+1 > argc) {
	cout << "Missing argument for -decay" << endl;
	Usage(argv[0]);
	return false;
      }
      if (!(argv[i][0] >= '0' && argv[i][0] <= '9')) {
	cout << "Missing valid argument for -decay" << endl;
	Usage(argv[0]);
	return false;
      }
      decay = atoi(argv[i]);
      i++;
    }
    else if (strcmp(argv[i],"-shaping")==0) {
      i++;
      if (i+1 > argc) {
	cout << "Missing argument for -shaping" << endl;
	Usage(argv[0]);
	return false;
      }
      if (!(argv[i][0] >= '0' && argv[i][0] <= '9')) {
	cout << "Missing valid argument for -shaping" << endl;
	Usage(argv[0]);
	return false;
      }
      shaping = atoi(argv[i]);
      i++;
    }
    else if (strcmp(argv[i],"-top")==0) {
      i++;
      if (i+1 > argc) {
	cout << "Missing argument for -top" << endl;
	Usage(argv[0]);
	return false;
      }
      if (!(argv[i][0] >= '0' && argv[i][0] <= '9')) {
	cout << "Missing valid argument for -top" << endl;
	Usage(argv[0]);
	return false;
      }
      top = atoi(argv[i]);
      i++;
    }
    else if (strcmp(argv[i],"-coll")==0) {
      docoinc = true;
      i++;
      if (i+1 > argc) {
	cout << "Missing argument for -coll" << endl;
	Usage(argv[0]);
	return false;
      }
      if (!(argv[i][0] >= '0' && argv[i][0] <= '9')) {
	cout << "Missing valid argument for -coll" << endl;
	Usage(argv[0]);
	return false;
      }
      smpcoinc = atoi(argv[i]);
      i++;      
    }
    else if (strcmp(argv[i],"-ave")==0) {
      doave = true;
      i++;
    }
    else if (strcmp(argv[i],"-cal")==0) {
      docal = true;
      i++;
      if (i+1 > argc) {
	cout << "Missing argument for -cal" << endl;
	Usage(argv[0]);
	return false;
      }
      if (!(argv[i][0] >= '0' && argv[i][0] <= '9')) {
	cout << "Missing valid argument for -cal" << endl;
	Usage(argv[0]);
	return false;
      }
      trapthresh = atoi(argv[i]);
      i++;
    }
    else if (strcmp(argv[i],"-shapescan")==0) {
      doshapescan = true;
      i++;
      if (i+1 > argc) {
	cout << "Missing argument for -shapescan" << endl;
	Usage(argv[0]);
	return false;
      }
      if (!(argv[i][0] >= '0' && argv[i][0] <= '9')) {
	cout << "Missing valid argument for -shapescan" << endl;
	Usage(argv[0]);
	return false;
      }
      scansrc = atoi(argv[i]);
      i++;
    }
    else {
      cout << "Unrecognized parameter " << argv[i] << endl;
      Usage(argv[0]);
      return false;
    }
  }
  
  
  if (!docal && !doshapescan && (!fileok || (!doraw && !dotrig && !dotrap && !dofit && !docoinc &&!doave))) {
    if (!fileok)
      cout << "No file indicated" << endl;
    if (!doraw && !dotrig && !dotrap && !dofit && !docoinc && !doave)
      cout << "What do you want to do? " << endl;
    Usage(argv[0]);
    return false;
  }
  
  if ((dotrap || docal) && (decay == -1 || shaping == -1 || top == -1)) {
	  cout << "Specify trap parameters decay/shaping/top" << endl;
    Usage(argv[0]);
    return false;
  }
  
  if (doave && !dotrap) {
	cout << "Use average with trap filter" << endl;
	Usage(argv[0]);
    return false;
  }
	
	
		if (!docal && !doshapescan && (!fileok || (!doraw && !dotrig && !dotrap && !dofit && !docoinc &&!doave))) {
    if (!fileok)
      cout << "No file indicated" << endl;
    if (!doraw && !dotrig && !dotrap && !dofit && !docoinc && !doave)
      cout << "What do you want to do? " << endl;
    Usage(argv[0]);
    return false;
  }
  if ((dotrap || docal) && (decay == -1 || shaping == -1 || top == -1)) {
	  cout << "Specify trap parameters decay/shaping/top" << endl;
    Usage(argv[0]);
    return false;
  }
  if (doave && !dotrap) {
	cout << "Use average with trap filter" << endl;
	Usage(argv[0]);
    return false;
  }
	return true;
}

void Usage(std::string program) {
  cout << "Usage:   " << program  << " -f #1 [#2] -p path" << endl;
  cout << "-raw to convert .bin files to .root" << endl;
  cout << "      -format <format (0=feb2015,1=june2015,2=may2016,3=may2017)> to set data format (default 3)" << endl;
  cout << "-trig to convert .trig files to .root" << endl;
  cout << "-trap <threshold> to filter waveforms for events using linear trapezoid" << endl;
  cout << "      -decay <smp> to set linear trap decay constant" << endl;
  cout << "      -shaping <smp> to set linear trap shaping time" << endl;
  cout << "      -top <smp> to set linear trap flat top length" << endl;
  cout << "-ave (with -trap) to build average waveform" << endl;
  cout << "-fit <threshold> to filter waveforms for events using pulse fitting" << endl;
  cout << "-coll <time in smp (250smp = 1us)> to collect single-event coincidences" << endl;
  cout << "-cal <threshold> to perform calibration" << endl;
  cout << "-shapescan <src> o perform shaping scan on 207Bi (src=1), 113Sn (src=2-3), or 139Ce (src=4-5) betas or x-rays" << endl;
}


#endif // COMMAND_PARSER_CPP__

