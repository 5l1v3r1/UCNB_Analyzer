// Copyright 2016.  UT-Battelle, LLC.
// This file is part of UCNB_Analyzer.
// See LICENSE.md included in top directory of this distribution.

// Example script for use in ROOT with typical analysis routines that haven't made their way into the main program. Warning: may contain lazy and inefficient programming; viewer discretion is advised.

EventTreeFile etf;
TString mypath = "Files/JanUCN";

int maxch = MAXCH*MAXRIO;
int dt = 250;

//Quick check that Eprev works
void CheckE(int run) {
	etf.SetPath(mypath.Data());
	if (!etf.Open(run)) return;
	TH1D* h = MakeHist("hdiff",1000);
	double elast = -1;
	for (int i=0;i<etf.GetNumEvents();i++) {
		etf.GetEvent(i);
		if (elast != -1) h->Fill(elast-etf.myEvent.Eprev);
		double elast = 0;
		for (int ch=0;ch<maxch;ch++) {
			if (etf.myEvent.E[ch] > 0) elast += etf.myEvent.E[ch];
		}
	}
	h->Draw();
}

void PlotE(int i) {
	etf.GetEvent(i);
	TH1D* h = MakeHist("hsum",maxch);
	double esum = 0;
	for (int ch=0;ch<maxch;ch++) {
		h->Fill(ch,etf.myEvent.E[ch]);
	}
	h->Draw();
}

//Plot num participating channels
void PlotNumCh(int run) {
	etf.SetPath(mypath.Data());
	etf.Open(run); 
	int maxch = MAXCH*MAXRIO;
	TH1D* h = MakeHist("hnch",10);
	for (int i=0;i<etf.GetNumEvents();i++) {
		etf.GetEvent(i);
		int nch = 0;
		for (int ch=0;ch<maxch;ch++) {
			if (etf.myEvent.E[ch] > 0) nch++;
		}
		h->Fill(nch);
	}
	h->Draw();
}

//Plot which channels
void PlotCh(int run) {
	etf.SetPath(mypath.Data());
	etf.Open(run); 
	int maxch = MAXCH*MAXRIO;
	TH1D* h = MakeHist("hch",maxch);
	for (int i=0;i<etf.GetNumEvents();i++) {
		etf.GetEvent(i);
		for (int ch=0;ch<maxch;ch++) {
			if (etf.myEvent.E[ch] > 0)
				h->Fill(ch);
		}
	}
	h->Draw();
}

//Plot which channels
void PlotChvTime(int run,double time) {
	if (run <= 90)
		etf.SetPath(mypath.Data());
	else
		etf.SetPath("Files/FebUCN");
	etf.Open(run); 
	int maxch = MAXCH*MAXRIO;
	TH2D* hh = (TH2D*)gROOT->FindObject("hh");
	if (hh != 0) delete hh;
	hh = new TH2D("hh","hh",45,0,45,300,0,time);
	for (int i=0;i<etf.GetNumEvents();i++) {
		etf.GetEvent(i);
		for (int ch=0;ch<maxch;ch++) {
			if (etf.myEvent.E[ch] > 0)
				hh->Fill(ch,etf.myEvent.t);
		}
	}
	hh->Draw("COLZ");
}


//Plot channels with multiple hits
void PlotMultCh(int run) {
	etf.SetPath(mypath.Data());
	etf.Open(run); 
	int maxch = MAXCH*MAXRIO;
	TH1D* h = MakeHist("hnch",maxch);
	for (int i=0;i<etf.GetNumEvents();i++) {
		etf.GetEvent(i);
		int nch = 0;
		for (int ch=0;ch<maxch;ch++) {
			if (etf.myEvent.E[ch] > 0) nch++;
		}
		if (nch > 1)
			for (int ch=0;ch<maxch;ch++) {
				if (etf.myEvent.E[ch] > 0)
					h->Fill(ch);
			}
	}
	h->Draw();
}

void PlotCh2D(int run) {
	etf.SetPath(mypath.Data());
	etf.Open(run); 
	int maxch = MAXCH*MAXRIO;
	TH2D* h = MakeHist("hnch",maxch,maxch);
	for (int i=0;i<etf.GetNumEvents();i++) {
		etf.GetEvent(i);
		int nch = 0;
		int ch1 = -1, ch2 = -1;
		for (int ch=0;ch<maxch;ch++) {
			if (etf.myEvent.E[ch] > 0) {
				nch++;
				if (ch1 == -1)
					ch1 = ch;
				else if (ch2 == -1)
					ch2 = ch;
			}
		}
		if (nch > 1)
			for (int ch=0;ch<maxch;ch++) {
				if (etf.myEvent.E[ch] > 0) {
					h->Fill(ch1,ch2);
				}
			}
	}
	h->Draw("COLZ");	
}

//Plot mask
void PlotMask(int run) {
	etf.SetPath(mypath.Data());
	etf.Open(run); 
	int maxch = MAXCH*MAXRIO;
	TH1D* h = MakeHist("hmask",1000);
	for (int i=0;i<etf.GetNumEvents();i++) {
		etf.GetEvent(i);
		int mask = 0;
		for (int ch=0;ch<maxch;ch++) {
			if (etf.myEvent.E[ch] > 0)
				mask += 0x1 << ch;
		}
		h->Fill(mask);
	}
	h->Draw();
}

TH1D* MakeHist(const char* name, int len) {
	TH1D* h = (TH1D*) gROOT->FindObject(name);
	if (h != 0) delete h;
	return new TH1D(name,name,len,0,len);
}

TH2D* MakeHist(const char* name, int len1, int len2) {
	TH2D* h = (TH2D*) gROOT->FindObject(name);
	if (h != 0) delete h;
	return new TH2D(name,name,len1,0,len1,len2,0,len2);
}