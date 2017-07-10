// Copyright 2017.  UT-Battelle, LLC.
// This file is part of UCNB_Analyzer.
// See LICENSE.md included in top directory of this distribution.

// File: ShapeScan.cpp
// Purpose: Analysis task: perform automatic calibration using UCNB Jan2016 Data.  Work in progress.

#ifndef SHAPE_SCAN_CPP__
#define SHAPE_SCAN_CPP__

#include <math.h>

#include "ShapeScan.hh"
#include "SiCalibrator.hh"

#include "TStyle.h"
#include "TFile.h"


double ceb(double*x, double* par);
double cex(double*x, double* par);
double snb(double*x, double* par);
double snx(double*x, double* par);
double bib(double*x, double* par);
double invx(double* x, double* par);
double enc2(double* x, double* par);


const bool dodraw = true;

/*************************************************************************/
//                           Constructors
/*************************************************************************/
ShapeScan::ShapeScan() {
}

ShapeScan::ShapeScan(int src, std::string pin, std::string pout) {
	Set(src, pin, pout);
}


/*************************************************************************/
//                             Destructor
/*************************************************************************/
ShapeScan::~ShapeScan() {
}


/*************************************************************************/
//                                Set
/*************************************************************************/
void ShapeScan::Set(int ss, std::string pin, std::string pout) {
	src = ss;
	pathin = pin;
	pathout = pout;
}


/*************************************************************************/
//                                 Go
/*************************************************************************/
void ShapeScan::Go() {
	if (src < 0 || src > 5 || src == 1) {
		cout << "Shaping Scan aborted. Source " << src << " not yet handled." << endl;
		return;
	}
	//Hard-coded parameters, maybe use config file eventually
	int thresh = 35; int decay = 200; int top = 300;
	const int num = 14;
	//int sscan[] = {50,60,70,100,150,200,250,300,350,400,450};
	int sscan[] = {35,50,60,70,100,150,200,250,300,350,400,450,500,600};
	//double sscan[] = {35,50,60,70,100,150,200,250,300,350,400,450,500,600};
	//3.62 at 300K, 3.72 at 80K
	double eVtoENC = 3.7;
	//-----Create output file
	std::string filename = pathin;
	filename.append("/");
	if(src==0)
		filename.append("ShapingScanBib.root");
	if(src==2)
		filename.append("ShapingScanSnb.root");
	if(src==3)
		filename.append("ShapingScanSnx.root");
	if(src==4)
		filename.append("ShapingScanCeb.root");
	if(src==5)
		filename.append("ShapingScanCex.root");
	TFile* myfile = new TFile(filename.c_str(),"RECREATE");
	SiCalibrator calib;
	if (pathin.compare("") != 0) { 
		calib.SetPath(pathin);
	}
	TF1* fenc = new TF1("fenc",enc2,10,1000,3);
	//-----Shaping scans
	double sigma[num];
	double ENC2[num];
	double scan[num];
	double fwhm[num];
	double chi2[num];
	TH1D* hh1 = new TH1D("h1","h1",48,0,48);
	TH1D* hh2 = new TH1D("h2","h2",48,0,48);
	TH1D* hh3 = new TH1D("h3","h3",48,0,48);
	TH1D* hminsig = new TH1D("hminsig","hminsig",48,0,48);
	int cnt;
	double Elo, Ehi;
	TCanvas* canv = new TCanvas();
	gStyle->SetOptFit(11);
	bool first = true;
	std::string pdfname;
	for (int ch=0; ch< 48; ch++) {
		cnt = 0;
		printf("Channel %d/%d\r",ch,48);fflush(stdout);
		double snpars[6] = {1,1,3,20,1,0};
		double cepars[6] = {1,1,3,25,2,0};
		double bibpars[5] = {1,1,3,0,0};
		double snbpars[5] = {1,1,3,0,0};
		double cebpars[6] = {1,1,3,10,0,0};
		for (int i=0;i<num;i++) {
			int shape = sscan[i]; //top = shape;
			calib.SetPars(thresh, decay, shape, top);
			if (!calib.Load()) {
				delete fenc;
				delete hh1;
				delete hh2;
				delete hh3;
				delete hminsig;
				delete canv;
				myfile->Close();
				return;
			}
			TH1D* hp;
			if (src%2 == 1)
				hp = calib.GetHist(src,ch);
			else
				hp = calib.GetHist(src+1,ch);
			int rebin = 2;
			hp->Rebin(rebin);
			if (hp!= 0) {
				if (src == 0) {
					Elo = 1000; 
					//Follow source window with "gaussian" movement
					Ehi = 2500 + 2000*TMath::Exp(-1.*(350-shape)*(350-shape)/2./150./150.);
				}
				if (src == 2) {
					Elo = 900; 
					Ehi = 2500;
				}
				if (src == 3) {
					Elo = 10; 
					Ehi = 500;
				}
				if (src == 4) {
					Elo = 350; 
					Ehi = 1000;
				}
				if (src == 5) {
					Elo = 100; 
					Ehi = 300;
				}
				char hname[25];
				sprintf(hname,"hch%dsh%d",ch,shape);
				hp->SetTitle(hname);
				hp->GetXaxis()->SetRangeUser(Elo,Ehi);
				if (hp->Integral(Elo,Ehi) > 0) {
					TF1* fitf;
					if (src==0) {
						fitf = new TF1("fitf",bib,Elo,Ehi,5);
						fitf->SetParameters(&bibpars[0]);
						fitf->SetParLimits(0,0.1,0.5);
						pdfname = "Snbscan.pdf";
					}
					if (src==2) {
						fitf = new TF1("fitf",snb,Elo,Ehi,5);
						fitf->SetParameters(&snbpars[0]);
						fitf->SetParLimits(0,0.1,0.5);
						pdfname = "Snbscan.pdf";
					}
					if (src==3) {
						fitf = new TF1("fitf",snx,Elo,Ehi,6);
						fitf->SetParameters(&snpars[0]);
						fitf->SetParLimits(0,0.1,0.3);
						fitf->SetParLimits(3,1,30);
						pdfname = "Snxscan.pdf";
					}
					if (src==4) {						
						fitf = new TF1("fitf",ceb,Elo,Ehi,6);
						fitf->SetParameters(&cebpars[0]);
						fitf->SetParLimits(0,0.15,0.5);
						pdfname = "Cebscan.pdf";
					}
					if (src==5) {						
						fitf = new TF1("fitf",cex,Elo,Ehi,6);
						fitf->SetParameters(&cepars[0]);
						fitf->SetParLimits(0,0.15,0.3);
						fitf->SetParLimits(3,10,30);
						pdfname = "Cexscan.pdf";
					}
					int maxbin = hp->GetMaximumBin();
					int max = maxbin*rebin;
					double calib;
					if (src==0)
						calib = 482./(double)max;
					if (src==2)
						calib = 364./(double)max;
					if (src==3)
						calib = 24./(double)max;
					if (src==4)
						calib = 127./(double)max;
					if (src==5)
						calib = 33.5/(double)max;
					fitf->SetParameter(0,calib);
					double scale = hp->GetBinContent(maxbin)/fitf->Eval(max);
					fitf->SetParameter(1,scale);
					fitf->SetParName(2,"sigma");
					fitf->SetParLimits(2,1,10);
					fitf->SetNpx(1000);
					if (src != 2) {
						fitf->SetParLimits(4,0,10);
						fitf->FixParameter(5,0);
					}
					int fitresult = hp->Fit(fitf,"QN","",Elo,Ehi);
					fitf->ReleaseParameter(5);
					fitresult = hp->Fit(fitf,"QN","",Elo,Ehi);
					double chi2ndf = fitf->GetChisquare()/fitf->GetNDF();
					if (chi2ndf > 2.5) {
						fitf->SetNpx(2000);
						if (src==3)	fitf->SetParameters(&snpars[0]);
						if (src==5)	fitf->SetParameters(&cepars[0]);
						fitresult = hp->Fit(fitf,"QN","",Elo,Ehi);
					}
					chi2ndf = fitf->GetChisquare()/fitf->GetNDF();
					sigma[cnt] = fitf->GetParameter(2);
					/*
					if (dodraw && chi2ndf > 2.5) {
						cout << "ch " << ch << " shape: " << shape << " : " << sigma[cnt];
						cout << ", x^2 " << chi2ndf << endl;
						cout << "calib " << 33.5/fitf->GetParameter(0) << endl;
						for (int i=0;i<6;i++)
							cout << "p" << i << "=" << fitf->GetParameter(i) << "; ";
						cout << endl;
					}
					*/
					if (dodraw) {
						fitresult = hp->Fit(fitf,"Q","",Elo,Ehi);
						//hp->Draw(); fitf->Draw("same"); 
						canv->Update();
						if (first) {
							pdfname.append("(");
							first = false;
						}
						canv->Print(pdfname.c_str(),"pdf");
						//char dummy;
						//cin >> dummy;
					}
					if (chi2ndf < 5 && (fitresult == 0 || fitresult == 4)) {
						fwhm[cnt] = sigma[cnt]*2.35;
						ENC2[cnt] = sigma[cnt]*sigma[cnt]*1.e3*1.e3/eVtoENC/eVtoENC;
						scan[cnt] = sscan[i]*4.e-3; //us
						chi2[cnt] = chi2ndf;
						fitf->GetParameters(&cepars[0]);
						cnt++;
					}
					delete fitf;
				}
				delete hp;
			}
		}
		if (cnt > 0) {
			myfile->cd();
			TGraph* g = new TGraph(cnt,scan,ENC2); //fwhm);
			g->SetMarkerStyle(20);
			char gname[25];
			sprintf(gname,"g%d",ch);
			g->SetName(gname);
			g->Fit(fenc,"QN","",10*4.e-3,1000*4.e-3);
			myfile->cd();
			g->Write();
			hh1->SetBinContent(ch+1,fenc->GetParameter(0));
			hh2->SetBinContent(ch+1,fenc->GetParameter(1));
			hh3->SetBinContent(ch+1,fenc->GetParameter(2));
			double msig = TMath::Sqrt(fenc->GetMinimum(10*4.e-3,1000*4.e-3))*eVtoENC/1.e3;
			if (!TMath::IsNaN(msig))
				hminsig->SetBinContent(ch+1,msig);
		}
	}
	pdfname.append(")");
	canv->Print(pdfname.c_str(),"pdf"); //close out
	delete canv;
	hh1->Write();
	hh2->Write();
	hh3->Write();
	hminsig->Write();
	cout << endl;
	myfile->Close();
}


//fit function for 139Ce betas
double ceb(double*x, double* par) {
	double xval = *x;
	double calib1 = par[0];
	double scale = par[1];
	double sigma = par[2];
	double A = par[3];
	double B = par[4];
	double calib0 = par[5];
	xval = xval*calib1+calib0;
	double loE = 100;
	double hiE = 200;
	double value = 0;
	Double_t s2 = TMath::Sqrt(2.);
	if (xval < loE || xval > hiE) {
		TF1::RejectPoint();
		return value;
	}
	int src = 5; //139Ce
	for (int i=0; i<CalibSource::sourcelist[src].betas.size(); i++) {
		double amp = CalibSource::sourcelist[src].betas[i].branch;
		double adc = CalibSource::sourcelist[src].betas[i].E;
		value += amp*TMath::Exp(-1.*(xval-adc)*(xval-adc)/2./sigma/sigma);
		Double_t stepbkgd = (A/2.)*TMath::Erfc((xval-adc)/s2/sigma) + B;
		value += stepbkgd;
	}
	return value*scale;
}
//fit function for 139Ce x-rays
double cex(double*x, double* par) {
	double xval = *x;
	double calib1 = par[0];
	double scale = par[1];
	double sigma = par[2];
	double exptail = par[3];
	double expamp = par[4];
	double calib0 = par[5];
	xval = xval*calib1+calib0;
	double loE = 25;
	double hiE = 50;
	double value = 0;
	if (xval < loE || xval > hiE) {
		TF1::RejectPoint();
		return value;
	}
	int src = 5; //139Ce
	for (int i=0; i<CalibSource::sourcelist[src].xrays.size(); i++) {
		double amp = CalibSource::sourcelist[src].xrays[i].branch;
		double adc = CalibSource::sourcelist[src].xrays[i].E;
		value += amp*TMath::Exp(-1.*(xval-adc)*(xval-adc)/2./sigma/sigma);
	}
	//to do: include 27 keV beta instead of approximating as exponential background?
	value += expamp*scale*TMath::Exp(-1.*xval/exptail);
	return value*scale;
}

//fit function for 113Sn betas
double snb(double*x, double* par) {
	double xval = *x;
	double calib1 = par[0];
	double scale = par[1];
	double sigma = par[2];
	double bkgd = par[3];
	double calib0 = par[4];
	xval = xval*calib1+calib0;
	double loE = 330;
	double hiE = 425;
	double value = 0;
	if (xval < loE || xval > hiE) {
		TF1::RejectPoint();
		return value;
	}
	int src = 3; //113Sn
	value = bkgd;
	for (int i=0; i<CalibSource::sourcelist[src].betas.size(); i++) {
		double amp = CalibSource::sourcelist[src].betas[i].branch;
		double adc = CalibSource::sourcelist[src].betas[i].E;
		value += amp*TMath::Exp(-1.*(xval-adc)*(xval-adc)/2./sigma/sigma);
	}
	return value*scale;
}

//fit function for 113Sn x-rays
double snx(double*x, double* par) {
	double xval = *x;
	double calib1 = par[0];
	double scale = par[1];
	double sigma = par[2];
	double exptail = par[3];
	double expamp = par[4];
	double calib0 = par[5];
	xval = xval*calib1+calib0;
	double loE = 15;
	double hiE = 50;
	double value = 0;
	if (xval < loE || xval > hiE) {
		TF1::RejectPoint();
		return value;
	}
	int src = 3; //113Sn
	for (int i=0; i<CalibSource::sourcelist[src].xrays.size(); i++) {
		double amp = CalibSource::sourcelist[src].xrays[i].branch;
		double adc = CalibSource::sourcelist[src].xrays[i].E;
		value += amp*TMath::Exp(-1.*(xval-adc)*(xval-adc)/2./sigma/sigma);
	}
	value += expamp*scale*TMath::Exp(-1.*xval/exptail);
	return value*scale;
}

//fit function for 207Bi betas
double bib(double*x, double* par) {
	double xval = *x;
	double calib1 = par[0];
	double scale = par[1];
	double sigma = par[2];
	double bkgd = par[3];
	double calib0 = par[4];
	xval = xval*calib1+calib0;
	double loE = 450;
	double hiE = 600;
	double value = 0;
	if (xval < loE || xval > hiE) {
		TF1::RejectPoint();
		return value;
	}
	int src = 1; //207Bi
	value = bkgd;
	for (int i=0; i<CalibSource::sourcelist[src].betas.size(); i++) {
		double amp = CalibSource::sourcelist[src].betas[i].branch;
		double adc = CalibSource::sourcelist[src].betas[i].E;
		value += amp*TMath::Exp(-1.*(xval-adc)*(xval-adc)/2./sigma/sigma);
	}
	return value*scale;
}

//fit function 
double invx(double* x, double* par) {
	double xval = *x;
	return par[0]/xval + par[1];
}

double enc2(double* x, double* par) {
	double xval = *x;
	return par[0]/xval + par[1] + par[2]*xval;
}



#endif //ifndef SHAPE_SCAN_CPP__

