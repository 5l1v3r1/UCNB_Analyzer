// Copyright 2016.  Los Alamos National Security, LLC.
// This file is part of UCNB_Analyzer.
// This program is distributed under the terms of the GNU General Public License, version 2.0.  See LICENSE.md included in top directory of this distribution.

// File: SiCalibrator.cpp
// Purpose: Calibrates silicon detectors using known sources
 

#ifndef SI_CALIBRATOR_CPP__
#define SI_CALIBRATOR_CPP__

#include "SiCalibrator.hh"

/*************************************************************************/
//                            Constructor
/*************************************************************************/
SiCalibrator::SiCalibrator() {
}

/*************************************************************************/
//                              Destructor
/*************************************************************************/
SiCalibrator::~SiCalibrator() {
}

/*************************************************************************/
//                             DefineSources
/*************************************************************************/
void SiCalibrator::DefineSources() {
	//www.nndc.bnl.gov
	CalibSource_t Bi207 = {.name = "Bi207", .type = 1};
	SourceLine_t line = {.E = 56.7, .Edet = 0, .branch = 2.9};
	Bi207.betas.push_back(line);
	line.E = 481.6935; line.branch = 1.537; Bi207.betas.push_back(line);
	line.E = 553.8372; line.branch = 0.442; Bi207.betas.push_back(line);
	line.E = 565.8473; line.branch = 0.111; Bi207.betas.push_back(line);
	sourcelist.push_back(Bi207);
	CalibSource_t Bi207x = Bi207; Bi207x.type = -1 * Bi207.type;
	line.E = 10.6; line.branch = 33.2; Bi207x.xrays.push_back(line);
	line.E = 72.805; line.branch = 21.4; Bi207x.xrays.push_back(line);
	line.E = 74.969; line.branch = 35.7; Bi207x.xrays.push_back(line);
	sourcelist.push_back(Bi207x);
	CalibSource_t Sn113 = {.name = "Sn113", .type = 2};
	line.E = 20.1; line.branch = 17.0; Sn113.betas.push_back(line);
	line.E = 363.758; line.branch = 28.8; Sn113.betas.push_back(line);
	line.E = 387.461; line.branch = 5.60; Sn113.betas.push_back(line);
	line.E = 390.872; line.branch = 1.137; Sn113.betas.push_back(line);
	sourcelist.push_back(Sn113);
	CalibSource_t Sn113x = Sn113; Sn113x.type = -1 * Sn113.type;
	line.E = 24.002; line.branch = 28.0; Sn113x.xrays.push_back(line);
	line.E = 24.21; line.branch = 51.8; Sn113x.xrays.push_back(line);
	line.E = 27.238; line.branch = 4.66; Sn113x.xrays.push_back(line);
	sourcelist.push_back(Sn113x);
	CalibSource_t Ce139 = {.name = "Ce139", .type = 3};
	line.E = 27.4; line.branch = 8.3; Ce139.betas.push_back(line);
	line.E = 126.9329; line.branch = 17.15; Ce139.betas.push_back(line);
	line.E = 159.5912; line.branch = 2.298; Ce139.betas.push_back(line);
	line.E = 164.4962; line.branch = 0.4751; Ce139.betas.push_back(line);
	sourcelist.push_back(Ce139);
	CalibSource_t Ce139x = Ce139; Ce139x.type = -1 * Ce139.type;
	line.E = 33.034; line.branch = 22.5; Ce139x.betas.push_back(line);
	line.E = 33.442; line.branch = 41.0; Ce139x.betas.push_back(line);
	sourcelist.push_back(Ce139x);
}


#endif // SI_CALIBRATOR_CPP__

