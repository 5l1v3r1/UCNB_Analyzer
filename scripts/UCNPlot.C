// Copyright 2016.  UT-Battelle, LLC.
// This file is part of UCNB_Analyzer.
// See LICENSE.md included in top directory of this distribution.

// Quick analysis of trig files from Jan and Feb run

EventTreeFile ucnf;
TrigTreeFile ttf;
TString janpath = "Files/JanUCN";
TString febpath = "Files/FebUCN";

int maxch = MAXCH*MAXRIO;
int dt = 250;
int deadfile[185]; //files to ignore
ULong64_t deadmask[185]; //channels to ignore per run
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
	for (int i=0;i<185;i++) {
		deadmask[i]=0;
		deadfile[i]=0;
		tmax[i] = 1.e13;
	}
	//Not enough bits, break down
	ULong64_t bit = 0x1;
	deadmask[0] = bit<<10;
	deadmask[1] = (bit<<42) + (bit<<35);
	tmax[2] = 2.e11;
	tmax[3] = 3.38e11;
	deadmask[3] = (bit<<41) + (bit<<38) + (bit<<34) + (bit<<14);
	deadmask[4] = (bit<<41) + (bit<<39) + (bit<<14);
	deadmask[5] = bit<<41 + bit<<38 + bit<<14;
	deadmask[6] = bit<<42 + bit<<41 + bit<<38 + bit<<34 + bit<<14;
	deadmask[7] = bit<<14;
	deadmask[8] = bit<<42 + bit<<41 + bit<<40 + bit<<38;
	deadmask[9] = bit<<42 + bit<<41 + bit<<39 + bit<<38;
	deadmask[10] = bit<<10;
	deadmask[12] = bit<<10 + bit<<31;
	deadmask[23] = bit<<8;
	deadmask[25] = bit<<8;
	deadmask[27] = bit<<8;
	deadmask[29] = bit<<8;
	deadmask[30] = bit<<40 + bit<<8;
	deadmask[31] = bit<<13;
	tmax[32] = 1.e11;
	deadmask[32] = bit<<13;
	deadmask[33] = bit<<13;
	deadmask[34] = bit<<13 + bit<<8;
	deadmask[35] = bit<<10 + bit<<8 + bit<<1;
	deadmask[36] = bit<<10 + bit<<8;
	deadmask[37] = bit<<10 + bit<<8;
	deadmask[37] = bit<<10;
	deadmask[38] = bit<<10;
	deadmask[39] = bit<<10;
	deadmask[41] = bit<<8;
	deadmask[42] = bit<<8;
	deadmask[43] = bit<<10;
	deadmask[44] = bit<<10 + bit<<8;
	deadmask[45] = bit<<8;
	deadmask[47] = bit<<10 + bit<<8;
	deadmask[48] = bit<<8;
	deadmask[49] = bit<<8;
	deadmask[50] = bit<<13 + bit<<8;
	deadmask[51] = bit<<13 + bit<<8;
	deadmask[52] = bit<<8;
	deadmask[53] = bit<<10 + bit<<8;
	deadmask[54] = bit<<10 + bit<<8;
	deadmask[55] = bit<<10 + bit<<8;
	tmax[56] = 2.75e11;
	deadmask[56] = bit<<40 + bit<<13;
	deadmask[57] = bit<<38 + bit<<39;
	deadmask[58] = bit<<28;
	tmax[59] = 2.5e11;
	deadmask[60] = bit<<8;
	deadmask[62] = bit<<13;
	deadmask[65] = bit<<8;
	deadmask[66] = bit<<8 + bit<<1;
	deadfile[67] = 1;  //seems to be associated with loss of channels?
	deadmask[68] = bit<<15;
	deadmask[69] = bit<<15;
	//Feb runs
	deadmask[110] = bit<<4;
	deadmask[111] = bit<<4;
	deadmask[112] = bit<<4;
	deadmask[113] = bit<<4;
	deadmask[115] = bit<<4;
	deadmask[116] = bit<<4;
	deadmask[117] = bit<<18 + bit<<15;
	deadmask[119] = bit<<4;
	deadmask[120] = bit<<4;
	deadmask[122] = bit<<4;
	deadmask[123] = bit<<28;
	deadmask[117] = bit<<28 + bit<<4;
	deadmask[126] = bit<<28;
	deadmask[138] = bit<<28 + bit<<18 + bit<<15;
	deadfile[139] = 1;
	deadfile[140] = 1;
	deadmask[146] = bit<<14 + bit<<4;
	deadmask[147] = bit<<14 + bit<<4;
	deadmask[148] = bit<<14 + bit<<4;
	deadmask[149] = bit<<4;
	deadmask[150] = bit<<4;
	deadmask[151] = bit<<4;
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
bool OpenTrig(int run) {	
	if (run <= 90)
		ttf.SetPath(janpath.Data());
	else
		ttf.SetPath(febpath.Data());
	return ttf.Open(run);
}

void TrigLoop(int mych) {
	Setdead();
	TH2D* Hsum = new TH2D("sum","",100,0,500,50,-500,1500);
	TH2D* HVonUCNon = new TH2D("HVUCN","",100,0,500,50,-500,1500);
	TH2D* HVoffUCNon = new TH2D("UCN","",100,0,500,50,-500,1500);
	TH2D* HVonUCNoff = new TH2D("HV","",100,0,500,50,-500,1500);
	for (int r=0;r<185;r++) {
		if (!deadfile[r] && UCNon[r] != -1) {  if (OpenTrig(r)) {
			cout << "Run " << r << endl;
			int numev = ttf.GetNumEvents();
			int StartEv = 0;
			do {
				ttf.GetEvent(StartEv);
				double startt = ttf.Trig_event.t;
				ULong64_t mask = 0;
				ULong64_t bit = 0x1;
				//mask += bit<<ttf.Trig_event.ch;
				double startE = ttf.Trig_event.E;
				if (mask & deadmask[r] == 0) {
					int ev = StartEv + 1;
					if (ev < numev)
						ttf.GetEvent(ev);
					double tdiff = (ttf.Trig_event.t - startt)*4.e-3;
					while (tdiff < 1000 && ev < numev ) { //100 mus
					//never called????
						double esum = ttf.Trig_event.E;
						cout << esum << endl;
						mask = 0;
						//mask += bit<<ttf.Trig_event.ch;
						if (esum < 1500 && (mask & deadmask[r] == 0)) {
							Hsum->Fill(tdiff,esum);
							if (UCNon[r] && HVon[r])
								HVonUCNon->Fill(tdiff,esum);
							else if (UCNon[r] && !HVon[r])
								HVoffUCNon->Fill(tdiff,esum);
							else if (!UCNon[r] && HVon[r])
								HVonUCNoff->Fill(tdiff,esum);
						}
						ev++;
						if (ev < numev)
							ttf.GetEvent(ev);
						tdiff = (ttf.Trig_event.t - startt)*4.e-3;
					}
				}
				StartEv++; // for now double-count
			}while (StartEv < numev && ttf.Trig_event.t < tmax[r]);
		}}
	}
	Hsum->Draw("COLZ");
	TCanvas* c = new TCanvas();
	c->Divide (1,3);
	c->cd(1);
	HVonUCNon->Draw("COLZ");
	c->cd(2);
	HVoffUCNon->Draw("COLZ");
	c->cd(3);
	HVonUCNoff->Draw("COLZ");
	
}

void Loop() {
	Setdead();
	TH2D* Hsum = new TH2D("sum","",100,0,500,50,0,50);
	TH2D* HVonUCNon = new TH2D("HVUCN","",100,0,500,50,0,50);
	TH2D* HVoffUCNon = new TH2D("UCN","",100,0,500,50,0,50);
	TH2D* HVonUCNoff = new TH2D("HV","",100,0,500,50,0,50);
	int pcnt = 0;
	int bcnt = 0;
	int scnt = 0;
	double ptime = 0;
	double btime = 0;
	double stime = 0;
//	for (int r=0;r<185;r++) {
//	for (int r=0;r<=90;r++) {
	for (int r=101;r<104;r++) {
		int cnt = 0;
		if (!deadfile[r] && UCNon[r] != -1) {  if (Open(r)) {
			cout << "Run " << r;
			int numev = ucnf.GetNumEvents();
			int ev = 0;
			double Eprevprev = -1; //how to do it*****
			double tprevprev = -1;
			cnt = 0;
			do {
				ucnf.GetEvent(ev);
				ULong64_t mask = 0;
				bool westside = true;
				int thech = -1;
				for (int ch=0;ch<maxch;ch++){
					if (ucnf.myEvent.E[ch] > 0) {
						ULong64_t bit = 0x1;
						mask += bit<<ch;
						if (ch >= 24) westside = false;
						thech = ch;
					}
				}
				if (((mask & deadmask[r]) == 0) && ucnf.myEvent.tprev*4.e-3 < 500 && westside) { 
				//Good event
					cnt++;
					Hsum->Fill(ucnf.myEvent.tprev*4.e-3,ucnf.myEvent.Esum);
					if (UCNon[r] && HVon[r])
						HVonUCNon->Fill(ucnf.myEvent.tprev*4.e-3,ucnf.myEvent.Esum);
					else if (UCNon[r] && !HVon[r])
						HVoffUCNon->Fill(ucnf.myEvent.tprev*4.e-3,ucnf.myEvent.Esum);
					else if (!UCNon[r] && HVon[r])
						HVonUCNoff->Fill(ucnf.myEvent.tprev*4.e-3,ucnf.myEvent.Esum);
				}
				Eprevprev = ucnf.myEvent.Eprev;
				tprevprev = ucnf.myEvent.tprev;
				ev++;
			}while (ev < numev && ucnf.myEvent.t < tmax[r]);
			}
			double fulltime = ucnf.myEvent.t*4.e-9;
			if (fulltime < 1.e6) {
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
			}
		}
	}
	Hsum->Draw("COLZ");
	TCanvas* c = new TCanvas();
	c->Divide (1,3);
	c->cd(1);
	HVonUCNon->Draw("COLZ");
	c->cd(2);
	HVoffUCNon->Draw("COLZ");
	c->cd(3);
	HVonUCNoff->Draw("COLZ");

	cout << "totals" << endl;
	if (ptime != 0)
		cout << pcnt << "/" << ptime << " = " << (double)pcnt/ptime << endl;
	if (btime != 0)
		cout << bcnt << "/" << btime << " = " << (double)bcnt/btime << endl;
	if (stime != 0)
		cout << scnt << "/" << stime << " = " << (double)scnt/stime << endl;
}

void PlotProtonChannels() {
	Setdead();
	TH1D* hch = new TH1D("hch","hch",48,0,48);
	TH1D* Eprev = new TH1D("hEprev","",1000,0,1000);
	for (int r=0;r<185;r++) {
		if (!deadfile[r] && UCNon[r] && HVon[r]) {  if (Open(r)) {
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
				if (mask & deadmask[r] == 0 && ucnf.myEvent.tprev*4.e-3 < 500 && ucnf.myEvent.tprev*4.e-3 > 10 && ucnf.myEvent.Esum > 5 && ucnf.myEvent.Esum < 250) { 
					Eprev->Fill(ucnf.myEvent.Eprev);
					for (int ch=0;ch<maxch;ch++){
						if (ucnf.myEvent.E[ch] > 0) {
							hch->Fill(ch);
						}
					}
				}
				ev++;
			}while (ev < numev && ucnf.myEvent.t < tmax[r]);
		} }
	}
	hch->Draw();
	new TCanvas();
	Eprev->Draw();
}

void PlotAllE(int r) {
	if (!Open(r)) return;
	TH1D* h = new TH1D("hE","",500,-100,400);
	for (int ev = 0; ev < ucnf.GetNumEvents(); ev++) {
		ucnf.GetEvent(ev);
		double esum = 0;
		for (int ch=0;ch<maxch;ch++) {
			esum += ucnf.myEvent.E[ch];
		}
		if (esum < 0) cout << ev << endl;
		h->Fill(esum);
	}
	h->Draw();
}

void ScanAhead() {
	Setdead();
	TH2D* Hp = new TH2D("hp","",50,0,1000,50,0,500);
	for (int r=0;r<1;r++) {
//	for (int r=0;r<185;r++) {
		if (!deadfile[r] && UCNon[r] != -1) {  if (Open(r)) {
			cout << "Run " << r << endl;
			int numev = ucnf.GetNumEvents();
			int StartEv = 0;
			do {
				ucnf.GetEvent(StartEv);
				double startt = ucnf.myEvent.t;
				ULong64_t mask = 0;
				double startE = 0;
				for (int ch=0;ch<maxch;ch++){
					if (ucnf.myEvent.E[ch] > 0) {
						ULong64_t bit = 0x1;
						mask += bit<<ch;
						startE += ucnf.myEvent.E[ch];
					}
				}
				int ev = StartEv+1;
				ucnf.GetEvent(ev);
				double tdiff = (ucnf.myEvent.t - startt)*4.e-3;
				while (tdiff < 1000 && ev < numev) { //100 mus
					//cout << StartEv << " and " << ev << endl;
					//cout << "tdiff is " << tdiff << endl;
					if (tdiff < 1000) {
						double esum = 0;
						for (int ch=0;ch<maxch;ch++) {
							if (ucnf.myEvent.E[ch] > 0) {
								esum += ucnf.myEvent.E[ch];
							}
						}
						if (esum > 0 && esum < 500)
							Hp->Fill(tdiff, esum);
					}
					ev++;
					ucnf.GetEvent(ev);
					tdiff = (ucnf.myEvent.t - startt)*4.e-3;
				}
				StartEv = ev;
			}while (StartEv < numev && ucnf.myEvent.t < tmax[r]);
		}
		}
	}
	Hp->Draw("COLZ");
}