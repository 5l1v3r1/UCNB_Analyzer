// Copyright 2017.  UT-Battelle, LLC.
// This file is part of UCNB_Analyzer.
// See LICENSE.md included in top directory of this distribution.

// File: Ca45CommandParser.cpp
// Purpose: Parse parameters from Analyzer
 

#ifndef CA45_COMMAND_PARSER_CPP__
#define CA45_COMMAND_PARSER_CPP__

#include "Ca45CommandParser.hh"
//List of tasks
#include "ReplayFile.hh"
#include "ReplayCaBinFile.hh"
#include "ApplySingleTrap.hh"
#include "FitRCCR.hh"
#include "BuildTemplateWaveform.hh"

#include "TSystem.h"
#include "dirent.h"

void Usage(std::string program);

/*************************************************************************/
//                              Constructor
/*************************************************************************/
Ca45CommandParser::Ca45CommandParser() {
	//tasks
	doreplay = false, dotrig = false, dotrap = false, dofit = false, docoinc = false, doave = false;
	//parameters
	fileok = false;
	filenum1, filenum2, fitthresh=-1, trapthresh=-1, decay=-1, shaping=-1, top=-1, smpcoinc=-1;
	path = "";
	calibfile = "";
}


/*************************************************************************/
//                              Parse
/*************************************************************************/
bool Ca45CommandParser::Parse(int argc, char **argv) {
	//Clean all this up at some point
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
		if (opendir(path.c_str())==NULL) {
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
	else if (strcmp(argv[i],"-replay")==0) {
		doreplay = true;
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
    else if ((strcmp(argv[i],"-shaping")==0)||(strcmp(argv[i],"-shape")==0)) {
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
bool Ca45CommandParser::ErrorCheck(char* name) {
	if (!fileok || (!doreplay && !dotrig && !dotrap && !dofit && !docoinc &&!doave)) {
		if (!fileok)
			cout << "No file indicated" << endl;
		if (!doreplay && !dotrig && !dotrap && !dofit && !docoinc && !doave)
			cout << "What do you want to do? " << endl;
		Usage(name);
		return false;
	}
  
	if (dotrap && (decay == -1 || shaping == -1 || top == -1)) {
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
void Ca45CommandParser::GetTasks(vector<std::shared_ptr<Task>> &tasklist) {
	tasklist.clear();
	if (fileok) {
		shared_ptr<ReplayFile> replaytask(new ReplayFile(filenum1,filenum2));
		/*
		if (dotrig) {
			shared_ptr<ReplayTrigFile> mytask(new ReplayTrigFile(dataformat,path,path));
			replaytask->AddTask(mytask);
		}
		*/
		if (doreplay) {
			shared_ptr<ReplayCaBinFile> mytask(new ReplayCaBinFile(path,path));
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
		/*
		if (docoinc) {
			shared_ptr<FindCoincidence> mytask(new FindCoincidence(smpcoinc,calibfile,path,path));
			replaytask->AddTask(mytask);
		}
		*/
		if (doave && dotrap) {
			shared_ptr<BuildTemplateWaveform> mytask(new BuildTemplateWaveform(decay,shaping,top,path,path));
			replaytask->AddTask(mytask);
		}
		tasklist.push_back(replaytask);
	}
}


void Usage(std::string program) {
  cout << endl << "Usage:   " << program  << " -f #1 [#2 if range] -p path" << endl;
  cout << "-replay to replay .bin files into corrected waveforms" << endl;
  cout << "-trig to convert .trig files to .root" << endl;
  cout << "-trap <threshold> to filter waveforms for events using linear trapezoid" << endl;
  cout << "      -decay <smp> to set linear trap decay constant" << endl;
  cout << "      -shaping <smp> to set linear trap shaping time" << endl;
  cout << "      -top <smp> to set linear trap flat top length" << endl;
  cout << "-ave (with -trap) to build average waveform" << endl;
  cout << "-fit <threshold> to filter waveforms for events using pulse fitting" << endl;
  cout << "-coinc <time in smp (250smp = 1us)> to collect single-event coincidences" << endl;
}


#endif // CA45_COMMAND_PARSER_CPP__

