// Copyright 2017.  UT-Battelle, LLC.
// This file is part of UCNB_Analyzer.
// See LICENSE.md included in top directory of this distribution.

// File: UCNBCommandParser.cpp
// Purpose: Parse parameters from Analyzer
 

#ifndef UCNB_COMMAND_PARSER_CPP__
#define UCNB_COMMAND_PARSER_CPP__

#include "UCNBCommandParser.hh"
//List of tasks
#include "ReplayFile.hh"
#include "ReplayBinFile.hh"
#include "ReplayTrigFile.hh"
#include "ApplySingleTrap.hh"
#include "FitRCCR.hh"
#include "FindCoincidence.hh"
#include "BuildTemplateWaveform.hh"
#include "Calibrate.hh"
#include "ShapeScan.hh"

#include "TSystem.h"

// dataformat:  0 = feb15, 1 = june15, 2 = may16, 3 = may17
const int maxformat = 4; // 4 formats so far...
void Usage(std::string program);

/*************************************************************************/
//                              Constructor
/*************************************************************************/
UCNBCommandParser::UCNBCommandParser() {
	//tasks
	doraw = false, dotrig = false, dotrap = false, dofit = false, docoinc = false, doave = false, docal = false, doshapescan = false;
	//parameters
	fileok = false;
	filenum1, filenum2, fitthresh=-1, trapthresh=-1, decay=-1, shaping=-1, top=-1, smpcoinc=-1, scansrc=-1;
	dataformat = 3;
	path = "";
	calibfile = "";
}


/*************************************************************************/
//                              Parse
/*************************************************************************/
bool UCNBCommandParser::Parse(int argc, char **argv) {
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
    else if (strcmp(argv[i],"-calibfile")==0) {
		i++;
		if (i+1 > argc) {
			cout << "Missing argument for -calibfile" << endl;
			Usage(argv[0]);
			return false;
		}
		if (argv[i][0] == '-') {
			cout << "Missing valid argument for -calibfile" << endl;
			Usage(argv[0]);
			return false;
		}
		calibfile += argv[i];
		i++;
		if (gSystem->AccessPathName(calibfile.c_str())) {
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
    else if (strcmp(argv[i],"-coinc")==0) {
      docoinc = true;
      i++;
      if (i+1 > argc) {
	cout << "Missing argument for -coinc" << endl;
	Usage(argv[0]);
	return false;
      }
      if (!(argv[i][0] >= '0' && argv[i][0] <= '9')) {
	cout << "Missing valid argument for -coinc" << endl;
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
  
	 return ErrorCheck(argv[0]);
}
  
/*************************************************************************/
//                             ErrorCheck
/*************************************************************************/
bool UCNBCommandParser::ErrorCheck(char* name) {
	if (!docal && !doshapescan && (!fileok || (!doraw && !dotrig && !dotrap && !dofit && !docoinc &&!doave))) {
		if (!fileok)
			cout << "No file indicated" << endl;
		if (!doraw && !dotrig && !dotrap && !dofit && !docoinc && !doave)
			cout << "What do you want to do? " << endl;
		Usage(name);
		return false;
	}
  
	if ((dotrap || docal) && (decay == -1 || shaping == -1 || top == -1)) {
		cout << "Specify trap parameters decay/shaping/top" << endl;
		Usage(name);
		return false;
	}
  
	if (doave && !dotrap) {
		cout << "Use average with trap filter" << endl;
		Usage(name);
		return false;
	}
	
	if (!docal && !doshapescan && (!fileok || (!doraw && !dotrig && !dotrap && !dofit && !docoinc &&!doave))) {
		if (!fileok)
			cout << "No file indicated" << endl;
		if (!doraw && !dotrig && !dotrap && !dofit && !docoinc && !doave)
			cout << "What do you want to do? " << endl;
		Usage(name);
		return false;
	}
	
	if ((dotrap || docal) && (decay == -1 || shaping == -1 || top == -1)) {
		cout << "Specify trap parameters decay/shaping/top" << endl;
		Usage(name);
		return false;
	}
	
	if (doave && !dotrap) {
		cout << "Use average with trap filter" << endl;
		Usage(name);
		return false;
	}
	
	return true;
}
  
  

/*************************************************************************/
//                             GetTaskList
/*************************************************************************/
void UCNBCommandParser::GetTasks(vector<std::shared_ptr<Task>> &tasklist) {
	tasklist.clear();
	if (fileok) {
		shared_ptr<ReplayFile> replaytask(new ReplayFile(filenum1,filenum2));
		if (dotrig) {
			shared_ptr<ReplayTrigFile> mytask(new ReplayTrigFile(dataformat,path,path));
			replaytask->AddTask(mytask);
		}
		if (doraw) {
			shared_ptr<ReplayBinFile> mytask(new ReplayBinFile(dataformat,path,path));
			replaytask->AddTask(mytask);
		}
		if (dotrap && !doave) {
			shared_ptr<ApplySingleTrap> mytask(new ApplySingleTrap(trapthresh,decay,shaping,top,path,path));
			replaytask->AddTask(mytask);
		}
		if (dofit) {
			shared_ptr<FitRCCR> mytask(new FitRCCR(fitthresh,path,path));
			replaytask->AddTask(mytask);
		}
		if (docoinc) {
			shared_ptr<FindCoincidence> mytask(new FindCoincidence(smpcoinc,calibfile,path,path));
			replaytask->AddTask(mytask);
		}
		if (doave && dotrap) {
			shared_ptr<BuildTemplateWaveform> mytask(new BuildTemplateWaveform(decay,shaping,top,path,path));
			replaytask->AddTask(mytask);
		}
		tasklist.push_back(replaytask);
	}
	if (docal) { // to be tested
		shared_ptr<Calibrate> mytask(new Calibrate(trapthresh,decay,shaping,top,path,path));
		tasklist.push_back(mytask);
	}
	if (doshapescan) { // to be tested
		shared_ptr<ShapeScan> mytask(new ShapeScan(scansrc,path,path));
		tasklist.push_back(mytask);
	}
}


void Usage(std::string program) {
  cout << endl << "Usage:   " << program  << " -f #1 [#2 if range] -p path" << endl;
  cout << "-raw to convert .bin files to .root" << endl;
  cout << "      -format <format (0=feb2015,1=june2015,2=may2016,3=may2017)> to set data format (default 3)" << endl;
  cout << "-trig to convert .trig files to .root" << endl;
  cout << "-trap <threshold> to filter waveforms for events using linear trapezoid" << endl;
  cout << "      -decay <smp> to set linear trap decay constant" << endl;
  cout << "      -shaping <smp> to set linear trap shaping time" << endl;
  cout << "      -top <smp> to set linear trap flat top length" << endl;
  cout << "-ave (with -trap) to build average waveform" << endl;
  cout << "-fit <threshold> to filter waveforms for events using pulse fitting" << endl;
  cout << "-coinc <time in smp (250smp = 1us)> to collect single-event coincidences" << endl;
  cout << endl << "UNDER DEVELOPMENT:  only works with UCNB data set" << endl << endl;
  cout << "-cal <threshold> to perform auto-calibration" << endl;
  cout << "-shapescan <src> to perform shaping scan on 207Bi (src=1), 113Sn (src=2-3), or 139Ce (src=4-5) betas and x-rays" << endl << endl;
}


#endif // UCNB_COMMAND_PARSER_CPP__

