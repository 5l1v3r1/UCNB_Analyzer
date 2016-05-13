// Copyright 2016.  Los Alamos National Security, LLC.
// This file is part of UCNB_Analyzer.
// This program is distributed under the terms of the GNU General Public License, version 2.0.  See LICENSE.md included in top directory of this distribution.

// File: CalibSource.cpp
// Purpose: List of calibration sources

#ifndef CALIBSOURCE_CPP__
#define CALIBSOURCE_CPP__

#include "CalibSource.hh"

vector<CalibSource::CalibSource_t> DefineSources();
const vector<CalibSource::CalibSource_t> CalibSource::sourcelist = DefineSources();

double pe(double E);

/*************************************************************************/
//                             DefineSources
/*************************************************************************/
vector<CalibSource::CalibSource_t> DefineSources() {
	//if (maxtype != 0) return;
	vector<CalibSource::CalibSource_t> slist;
	int cnt = 0;
	CalibSource::CalibSource_t Bi207;
	Bi207.name = "Bi207";
	Bi207.type = cnt++;
	CalibSource::SourceLine_t line;
	line.E = 7.97; line.branch = 54.4; Bi207.betas.push_back(line);
	line.E = 56.7; line.branch = 2.9; Bi207.betas.push_back(line);
	line.E = 481.6935; line.branch = 1.537; Bi207.betas.push_back(line);
	line.E = 553.8372; line.branch = 0.442; Bi207.betas.push_back(line);
	line.E = 565.8473; line.branch = 0.111; Bi207.betas.push_back(line);
	line.E = 975.651; line.branch = 7.08; Bi207.betas.push_back(line);
	line.E = 1047.795; line.branch = 1.84; Bi207.betas.push_back(line);
	line.E = 1059.805; line.branch = 0.44; Bi207.betas.push_back(line);
	slist.push_back(Bi207);
	CalibSource::CalibSource_t Bi207x;
	Bi207x.name = "Bi207x";
	Bi207x.type = cnt++;
	Bi207x.betas = Bi207.betas;
	line.E = 10.6; line.branch = 33.2*pe(line.E); Bi207x.xrays.push_back(line);
	line.E = 72.805; line.branch = 21.4*pe(line.E); Bi207x.xrays.push_back(line);
	line.E = 74.969; line.branch = 35.7*pe(line.E); Bi207x.xrays.push_back(line);
	line.E = 84.45; line.branch = 4.31*pe(line.E); Bi207x.xrays.push_back(line);
	line.E = 84.938; line.branch = 8.27*pe(line.E); Bi207x.xrays.push_back(line);
	line.E = 87.3; line.branch = 3.02*pe(line.E); Bi207x.xrays.push_back(line);
	slist.push_back(Bi207x);
	//Fill in extras
	CalibSource::CalibSource_t Sn113;
	Sn113.name = "Sn113";
	Sn113.type = cnt++;
	line.E = 20.1; line.branch = 17.0; Sn113.betas.push_back(line);
	line.E = 363.758; line.branch = 28.8; Sn113.betas.push_back(line);
	line.E = 387.461; line.branch = 5.60; Sn113.betas.push_back(line);
	line.E = 390.872; line.branch = 1.137; Sn113.betas.push_back(line);
	slist.push_back(Sn113);
	CalibSource::CalibSource_t Sn113x;
	Sn113x.name = "Sn113x";
	Sn113x.type = cnt++;
	Sn113x.betas = Sn113.betas;
	line.E = 24.002; line.branch = 28.0*pe(line.E); Sn113x.xrays.push_back(line);
	line.E = 24.21; line.branch = 51.8*pe(line.E); Sn113x.xrays.push_back(line);
	line.E = 27.238; line.branch = 4.66*pe(line.E); Sn113x.xrays.push_back(line);
	line.E = 27.276; line.branch = 9.0*pe(line.E); Sn113x.xrays.push_back(line);
	line.E = 27.863; line.branch = 2.39*pe(line.E); Sn113x.xrays.push_back(line);
	slist.push_back(Sn113x);
	CalibSource::CalibSource_t Ce139;
	Ce139.name = "Ce139";
	Ce139.type = cnt++;
	line.E = 27.4; line.branch = 8.3; Ce139.betas.push_back(line);
	line.E = 126.9329; line.branch = 17.15; Ce139.betas.push_back(line);
	line.E = 159.5912; line.branch = 2.298; Ce139.betas.push_back(line);
	line.E = 164.4962; line.branch = 0.4751; Ce139.betas.push_back(line);
	slist.push_back(Ce139);
	CalibSource::CalibSource_t Ce139x;
	Ce139x.name = "Ce139x";
	Ce139x.type = cnt++;
	Ce139x.betas = Ce139.betas;
	line.E = 33.034; line.branch = 22.5*pe(line.E); Ce139x.xrays.push_back(line);
	line.E = 33.442; line.branch = 41.0*pe(line.E); Ce139x.xrays.push_back(line);
	line.E = 37.72; line.branch = 3.95*pe(line.E); Ce139x.xrays.push_back(line);
	line.E = 37.801; line.branch = 7.62*pe(line.E); Ce139x.xrays.push_back(line);
	line.E = 38.726; line.branch = 2.46*pe(line.E); Ce139x.xrays.push_back(line);
	slist.push_back(Ce139x);
//	maxtype = cnt;
	return slist;
//	CalData.resize(maxtype);
}

double pe(double E) {
	//www.nndc.bnl.gov
	double sidensity = 2.329; //g/cm^3
	double sithickness = 0.2; //cm
	//from fit to photoelectric cs from NIST XCOM
	return sidensity*sithickness*(1.313 - (120.095/E) + (3465.4/E/E));
}


#endif // CALIBSOURCE_CPP__