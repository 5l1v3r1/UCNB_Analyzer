// Copyright 2016.  UT-Battelle, LLC.
// This file is part of UCNB_Analyzer.
// See LICENSE.md included in top directory of this distribution.

// Quick analysis of trig files from Jan and Feb run
// To replay files (I used XYZ = 500)
// ./Analyzer -f 0 185 -path "/Path/To/Files/" -format 1  -trig
// ./Analyzer -f 0 185 -path "/Path/To/Files/" -calibfile "/Path/To/calibration.dat" -coll XYZ
// To produce coincidence plot:
// In ROOT:   Loop()


EventTreeFile ucnf;
TString janpath = "Files/JanUCN";
TString febpath = "Files/FebUCN";

int maxch = MAXCH*MAXRIO;
int deadfile[185]; //files to ignore
int deadmask[185]; //channels to ignore per run
int deadmask2[185]; //next 32 channels hax
struct badtime_t {
	vector<double>start;
	vector<double>stop;
};
badtime_t badtime[185]; //ignore time slot per run
double tmax[185];
int HVon[185] = {0,1,1,1,1,  1,1,1,0,0,
				1,0,1,1,1,  1,1,0,1,1,
				1,1,0,1,1,  1,1,1,1,0,
				1,1,1,0,0,  1,1,1,1,0,
				1,1,1,1,0,  1,1,1,1,1,
				0,1,1,1,1,  1,1,1,0,1, //50-59
				1,1,1,1,0,  1,1,1,1,0,
				1,1,1,1,0,  1,1,1,1,0,
				1,1,1,1,0,  1,1,1,0,1,
				1,0,1,0,1,  1,1,1,0,1,
				1,1,1,0,1,  1,1,1,1,0, //100-109
				1,1,1,1,0,  1,1,1,1,1,
				0,1,1,1,1,  0,1,1,1,1,
				0,1,1,1,1,  0,1,1,1,-1,
				-1,1,0,1,1, 1,1,1,0,1,
				1,1,1,0,1,  1,1,1,0,1, //150-159
				1,1,1,0,1,  1,1,1,0,1,
				1,1,1,0,1,  1,0,0,0,1,
				1,-1,-1,-1,-1};
int UCNon[185]= {1,1,1,1,1,  1,1,1,1,1,
				1,1,1,1,-1, 0,1,1,1,-1,
				0,1,1,1,1,  -1,0,1,1,1,
				1,1,1,1,1,  1,-1,0,1,1,
				1,-1,0,1,1, 1,-1,0,1,1,
				1,1,1,-1,0, 1,1,1,1,1, //50-59
				1,-1,0,1,1, 1,-1,0,1,1,
				1,-1,0,1,1, 1,-1,0,1,1,
				1,0,-1,1,1, 1,-1,1,1,1,
				1,1,1,1,1,  -1,0,1,1,1,
				-1,0,1,1,1, -1,-1,0,1,1, //100-109
				1,-1,0,1,1, 1,1,-1,0,1,
				1,1,-1,0,1, 1,1,-1,0,1,
				1,1,-1,0,1, 1,1,-1,0,-1,
				-1,1,1,1,-1,0,1,1,1,1,
				-1,0,1,1,1, -1,0,1,1,1, //150-159
				-1,0,1,1,1, -1,0,1,1,1,
				-1,0,1,1,1, -1,-1,-1,-1,-1,
				1,-1,-1,-1,-1}; 
int AFPon[185]= {0,0,1,1,1,  1,1,1,1,1,
				0,0,0,0,-1, 1,1,1,1,-1,
				1,0,0,0,0,  -1,0,1,1,1,
				1,0,0,0,0,  0,-1,0,1,1,
				1,-1,0,0,0, 0,-1,0,0,1,
				0,1,1,-1,1, 1,0,0,0,0, //50-59
				0,-1,0,1,1, 1,-1,1,0,0,
				0,-1,0,1,1, 1,-1,1,0,0,
				0,0,-1,1,1, 1,1,0,0,0,
				0,0,0,0,0,  -1,0,1,1,1,
				1,1,0,0,0,  0,0,0,1,1, //100-109
				1,1,1,0,0,  0,0,-1,0,1,
				1,1,-1,1,0, 0,0,-1,0,1,
				1,1,-1,1,0, 0,0,-1,0,-1,
				-1,1,1,1,-1,1,0,0,0,0,
				-1,0,1,1,1,  -1,1,0,0,0, //150-159
				-1,0,1,1,1, -1,1,0,0,0,
				-1,0,1,1,1, -1,-1,-1,-1,-1,
				0,-1,-1,-1,-1};

void Setdead() {
	int bit = 0x1;
	int sub = 32;
	for (int i=0;i<185;i++) {
		deadmask[i]=0;
		deadfile[i]=0;
		tmax[i] = 1.e13;
		badtime[i].start.resize(0);
		badtime[i].stop.resize(0);
	}
	deadmask[0] = bit<<10;
	deadmask[1] = bit<<14;
	deadmask2[1] = (bit<<(42-sub)) + (bit<<(35-sub));
	badtime[1].start.push_back(0);
	badtime[1].stop.push_back(2.e11);
	tmax[1] = 1.e12;
	deadmask[2] = bit<<14;
	tmax[2] = 2.e11;
	deadmask[3] = bit<<14;
	deadmask2[3] = (bit<<(41-sub)) + ((bit<<38-sub)) + ((bit<<34-sub));
	tmax[3] = 3.38e11;
	deadmask[4] = bit<<14;
	deadmask2[4] = (bit<<(41-sub)) + (bit<<(39-sub));
	badtime[4].start.push_back(19.e9);
	badtime[4].stop.push_back(23.e9);
	deadmask[5] = bit<<14;
	deadmask2[5] = (bit<<(41-sub)) + (bit<<(38-sub));
	deadmask[6] = bit<<14;
	deadmask2[6] = (bit<<(42-sub)) + (bit<<(41-sub)) + (bit<<(38-sub)) + (bit<<(34-sub));
	deadmask[7] = bit<<14;
	deadmask2[8] = (bit<<(42-sub)) + (bit<<(41-sub)) + (bit<<(40-sub)) + (bit<<(38-sub));
	deadmask2[9] = (bit<<(42-sub)) + (bit<<(41-sub)) + (bit<<(39-sub)) + (bit<<(38-sub));
	deadmask[10] = bit<<10;
	deadmask[12] = (bit<<10) + (bit<<31);
	deadmask[23] = bit<<8;
	deadmask[25] = bit<<8;
	deadmask[27] = bit<<8;
	deadmask[29] = bit<<8;
	deadmask[30] = bit<<8;
	deadmask2[30] = (bit<<(40-sub));
	deadmask[31] = bit<<13;
	tmax[32] = 1.e11;
	deadmask[32] = bit<<13;
	deadmask[33] = bit<<13;
	deadmask[34] = (bit<<13) + (bit<<8);
	deadmask[35] = (bit<<10) + (bit<<8) + (bit<<1);
	deadmask[36] = (bit<<10) + (bit<<8);
	deadmask[37] = (bit<<10) + (bit<<8);
	deadmask[38] = bit<<10;
	deadmask[39] = bit<<10;
	deadmask[41] = bit<<8;
	deadmask[42] = bit<<8;
	deadmask[43] = bit<<10;
	deadmask[44] = (bit<<10) + (bit<<8);
	deadmask[45] = bit<<8;
	deadmask[47] = (bit<<10) + (bit<<8);
	deadmask[48] = bit<<8;
	deadmask[49] = bit<<8;
	deadmask[50] = (bit<<13) + (bit<<8);
	deadmask[51] = (bit<<13) + (bit<<8);
	deadmask[52] = bit<<8;
	deadmask[53] = (bit<<10) + (bit<<8);
	deadmask[54] = (bit<<10) + (bit<<8);
	deadmask[55] = (bit<<10) + (bit<<8);
	deadmask[55] = (bit<<10) + (bit<<8);
	tmax[56] = 2.75e11;
	deadmask[56] = bit<<13;
	deadmask2[56] = (bit<<(40-sub));
	deadmask2[57] = (bit<<(38-sub)) + (bit<<(39-sub));
	deadmask[58] = bit<<28;
	tmax[59] = 2.5e11;
	deadmask[60] = bit<<8;
	deadmask[62] = bit<<13;
	deadmask[65] = bit<<8;
	deadmask[66] = (bit<<8) + (bit<<1);
	deadfile[67] = 1;  //seems to be associated with loss of channels?
	deadmask[68] = bit<<15;
	deadmask[69] = bit<<15;
	//Feb runs
	deadmask[108] = bit<<14;
	deadmask[110] = (bit<<14) + (bit<<4);
	deadmask[111] = bit<<4;
	deadmask[112] = (bit<<14) + (bit<<4);
	deadmask[113] = (bit<<14) + (bit<<4);
	deadmask[115] = bit<<4;
	deadmask[116] = bit<<4;
	deadmask[117] = (bit<<18) + (bit<<15);
	deadmask[119] = bit<<4;
	deadmask[120] = bit<<4;
	deadmask[122] = bit<<4;
	deadmask[123] = bit<<28;
	deadmask[117] = (bit<<28) + (bit<<4);
	deadmask[126] = bit<<28;
	deadmask[138] = (bit<<28) + (bit<<18) + (bit<<15);
	deadfile[139] = 1;
	deadfile[140] = 1;
	deadmask[146] = (bit<<14) + (bit<<4);
	deadmask[147] = (bit<<14) + (bit<<4);
	badtime[147].start.push_back(230e9);
	badtime[147].stop.push_back(235e9);
	badtime[147].start.push_back(276e9);
	badtime[147].stop.push_back(278e9);
	deadmask[148] = (bit<<14) + (bit<<4);
	deadmask[149] = bit<<4;
	deadmask[150] = bit<<4;
	deadmask[151] = bit<<4;
	deadmask[157] = bit<<4;
	deadmask[158] = bit<<4;
	deadmask[169] = bit<<4;
	deadmask[170] = bit<<4;
	deadmask[171] = bit<<4;
	deadfile[172] = 1;
	deadfile[173] = 1;
	deadfile[174] = 1;
	deadfile[175] = 1;
	for (int i=180;i<=185;i++)
		deadfile[i] = 1;
}
				
bool Open(int run) {	
	if (run <= 90)
		ucnf.SetPath(janpath.Data());
	else
		ucnf.SetPath(febpath.Data());
	return ucnf.Open(run);
}

void Loop(int run = -1) {
	Setdead();
	TH2D* h = (TH2D*) gROOT->FindObject("HVUCN");  if (h != 0) delete h;
	h = (TH2D*) gROOT->FindObject("UCN");  if (h != 0) delete h;
	h = (TH2D*) gROOT->FindObject("HV");  if (h != 0) delete h;
	TH2D* HVonUCNon = new TH2D("HVUCN","",300,-100,500,50,0,100);
	TH2D* HVoffUCNon = new TH2D("UCN","",300,-100,500,50,0,100);
	TH2D* HVonUCNoff = new TH2D("HV","",300,-100,500,50,0,100);
	TH1D* elec = new TH1D("elec","",1000,0,1000);
	int pcnt = 0;
	int bcnt = 0;
	int scnt = 0;
	double ptime = 0;
	double btime = 0;
	double stime = 0;
	int start = run;
	int stop = run+1;
	if (run == -1) {start = 0; stop = 185;}
	for (int r=start;r<stop;r++) {
		int cnt = 0;
		if (!deadfile[r] && UCNon[r] != -1) {  if (Open(r)) {
			int numev = ucnf.GetNumEvents();
			cout << "Run " << r;
			int ev = 0;
			bool westprev = false;
			bool goodprev = false;
			cnt = 0;
			do {
				ucnf.GetEvent(ev);
				int mask = 0, mask2 = 0;
				bool westside = true;
				int thech = -1;
				for (int ch=0;ch<maxch;ch++){
					if (ucnf.myEvent.E[ch] > 0) {
						int bit = 0x1;
						if (ch < 32)
							mask += bit<<ch;
						else
							mask2 += bit<<(ch-32);
						if (ch >= 24) westside = false;
						thech = ch;
					}
				}
				bool badt = false;
				for (int i=0;i<badtime[r].start.size();i++) {
					if (ucnf.myEvent.t > badtime[r].start[i] && ucnf.myEvent.t < badtime[r].stop[i])
						badt = true;
				}
				bool Imaproton = false;
				if (((mask & deadmask[r]) == 0) && ((mask2 & deadmask2[r]) == 0) && ucnf.myEvent.tprev*4.e-3 < 500 && westside&& !badt && ucnf.myEvent.Eprev < 2000) { 
				//Good event
					if (ucnf.myEvent.tprev*4.e-3 > 3 && ucnf.myEvent.tprev*4.e-3 < 200 && ucnf.myEvent.Esum > 5 && ucnf.myEvent.Esum < 25  && goodprev) {
						cnt++;
						elec->Fill(ucnf.myEvent.Eprev);
						Imaproton = true;
					}
					gROOT->cd();
					if (UCNon[r] && HVon[r])
						HVonUCNon->Fill(ucnf.myEvent.tprev*4.e-3,ucnf.myEvent.Esum);
					else if (UCNon[r] && !HVon[r])
						HVoffUCNon->Fill(ucnf.myEvent.tprev*4.e-3,ucnf.myEvent.Esum);
					else if (!UCNon[r] && HVon[r])
						HVonUCNoff->Fill(ucnf.myEvent.tprev*4.e-3,ucnf.myEvent.Esum);
					if (UCNon[r] && HVon[r])
						elec->Fill(ucnf.myEvent.Eprev);
				}
				//Look backwards
				if (goodprev && westprev && ucnf.myEvent.tprev*4.e-3 < 100 && ((mask & deadmask[r]) == 0) && ((mask2 & deadmask2[r]) == 0) && !Imaproton && ucnf.myEvent.Eprev < 2000 && ucnf.myEvent.Esum < 2000) {
					gROOT->cd();
					if (UCNon[r] && HVon[r])
						HVonUCNon->Fill(ucnf.myEvent.tprev*-4.e-3,ucnf.myEvent.Eprev);
					else if (UCNon[r] && !HVon[r])
						HVoffUCNon->Fill(ucnf.myEvent.tprev*-4.e-3,ucnf.myEvent.Eprev);
					else if (!UCNon[r] && HVon[r])
						HVonUCNoff->Fill(ucnf.myEvent.tprev*-4.e-3,ucnf.myEvent.Eprev);
				}
				westprev = westside;
				goodprev = (!badt && ((mask & deadmask[r]) == 0) && ((mask2 & deadmask2[r]) == 0));
				ev++;
				if (ev < numev) ucnf.GetEvent(ev);
			}while (ev < numev && ucnf.myEvent.t < tmax[r]);
			if (ucnf.myEvent.t >= tmax[r])
				ucnf.GetEvent(ev-1);
			double fulltime = ucnf.myEvent.t*4.e-9;
			for (int i=0;i<badtime[r].start.size();i++) {
				fulltime -= (badtime[r].stop[i] - badtime[r].start[i])*4.e-9;
			}
			if (UCNon[r] && HVon[r]) {
				cout << ": " << cnt << " protons ";
				ptime += fulltime;
				pcnt += cnt;
			}
			else if (UCNon[r] && !HVon[r]) {
				cout << ": " << cnt << " betas? ";
				btime += fulltime;
				bcnt += cnt;
			}
			else if (!UCNon[r] && HVon[r]) {
				cout << ": " << cnt << " sparks? ";
				stime += fulltime;
				scnt += cnt;
			}
			if (fulltime != 0)
				cout << "\t" << (double)cnt / fulltime << " Hz\t\t" << fulltime << endl;
		}}
	}
	gROOT->cd();
	c = (TCanvas*) gROOT->FindObject("canv"); if (c!=0) delete c;
	c = new TCanvas("canv");
	c->Divide(2,2);
	c->cd(1);
	HVonUCNon->SetStats(false);
	HVonUCNon->GetZaxis()->SetRangeUser(0,200);
	HVonUCNon->Draw("COLZ");
	c->cd(2);
	HVoffUCNon->SetStats(false);
	HVoffUCNon->GetZaxis()->SetRangeUser(0,200);
	HVoffUCNon->Draw("COLZ");
	c->cd(3);
	HVonUCNoff->SetStats(false);
	HVonUCNoff->GetZaxis()->SetRangeUser(0,200);
	HVonUCNoff->Draw("COLZ");
	c->cd(4);
	elec->SetStats(false);
	elec->Draw();

	cout << "totals" << endl;
	if (ptime != 0)
		cout << pcnt << "/" << ptime << " = " << (double)pcnt/ptime << endl;
	if (btime != 0)
		cout << bcnt << "/" << btime << " = " << (double)bcnt/btime << endl;
	if (stime != 0)
		cout << scnt << "/" << stime << " = " << (double)scnt/stime << endl;
}

void PlotProtonChannels(int r) {
	Setdead();
	TH1D* hch = new TH1D("hch","hch",48,0,48);
	TH1D* Eprev = new TH1D("hEprev","",1000,0,1000);
	if (!deadfile[r] /*&& UCNon[r] && HVon[r]*/) {  if (Open(r)) {
		cout << "Run " << r << endl;
		int numev = ucnf.GetNumEvents();
		int ev = 0;
		do {
			ucnf.GetEvent(ev);
			ULong64_t mask = 0;
			for (int ch=0;ch<maxch;ch++){
				if (ucnf.myEvent.E[ch] > 0) {
					ULong64_t bit = 0x1;
					mask += bit<<ch;
				}
			}
			bool badt = false;
			for (int i=0;i<badtime[r].start.size();i++) {
				if (ucnf.myEvent.t > badtime[r].start[i] && ucnf.myEvent.t < badtime[r].stop[i])
					badt = true;
			}
			if ((mask & deadmask[r]) == 0 && ucnf.myEvent.tprev*4.e-3 < 200 && ucnf.myEvent.tprev*4.e-3 > 3 && ucnf.myEvent.Esum > 5 && ucnf.myEvent.Esum < 25 && !badt) { 
				gROOT->cd();
				Eprev->Fill(ucnf.myEvent.Eprev);
				for (int ch=0;ch<maxch;ch++){
					if (ucnf.myEvent.E[ch] > 0) {
						hch->Fill(ch);
					}
				}
			}
			ev++;
		}while (ev < numev && ucnf.myEvent.t < tmax[r]);
	}}
	gROOT->cd();
	hch->Draw();
	new TCanvas();
	Eprev->Draw();
}
