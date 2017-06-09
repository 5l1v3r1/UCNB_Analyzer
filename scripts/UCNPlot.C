// Copyright 2016.  UT-Battelle, LLC.
// This file is part of UCNB_Analyzer.
// See LICENSE.md included in top directory of this distribution.

// Quick analysis of trig files from Jan and Feb run
// To replay files (I used XYZ = 500)
// ./Analyzer -f 0 185 -path "/Path/To/Files/" -format 1  -trig
// ./Analyzer -f 0 185 -path "/Path/To/Files/" -calibfile "/Path/To/calibration.dat" -coll XYZ
// To produce coincidence plots:
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


void Setdead();	
bool Open(int run);
void Loop(int run = -1);
void PrettyPlot();
void PlotProtonChannels(int r);
void CoincESpect(int run = -1);


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
	deadmask2[3] = (bit<<(41-sub)) + ((bit<<(38-sub))) + ((bit<<(34-sub)));
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

TDatime da[186];
void SetDateTimes() {
	da[0].Set(2016,1,30,4,56,0);
	da[1].Set(2016,1,30,6,57,0);
	da[2].Set(2016,1,30,7,26,0);
	da[3].Set(2016,1,30,8,08,0);
	da[4].Set(2016,1,30,8,17,0);
	da[5].Set(2016,1,30,9,01,0);
	da[6].Set(2016,1,30,9,50,0);
	da[7].Set(2016,1,30,10,20,0);
	//west dewar fill
	da[8].Set(2016,1,30,20,27,0);
	da[9].Set(2016,1,30,20,35,0);
	da[10].Set(2016,1,30,22,48,0);
	da[11].Set(2016,1,30,23,03,0);
	da[12].Set(2016,1,30,23,33,0);
	da[13].Set(2016,1,30,23,43,0);
	da[14].Set(2016,1,31,00,01,0);
	da[15].Set(2016,1,31,00,13,0);
	da[16].Set(2016,1,31,00,36,0);
	da[17].Set(2016,1,31,00,53,0);
	da[18].Set(2016,1,31,01,25,0);
	da[19].Set(2016,1,31,01,45,0);
	da[20].Set(2016,1,31,01,58,0);
	da[21].Set(2016,1,31,02,27,0);
	da[22].Set(2016,1,31,02,52,0);
	da[23].Set(2016,1,31,03,08,0);
	da[24].Set(2016,1,31,03,41,0);
	da[25].Set(2016,1,31,03,53,0);
	da[26].Set(2016,1,31,04,12,0);
	da[27].Set(2016,1,31,04,32,0);
	da[28].Set(2016,1,31,04,44,0);
	da[29].Set(2016,1,31,04,56,0);
	da[30].Set(2016,1,31,05,26,0);
	da[31].Set(2016,1,31,08,30,0);
	da[32].Set(2016,1,31,08,50,0);
	da[33].Set(2016,1,31,08,55,0);
	da[34].Set(2016,1,31,09,00,0);
	da[35].Set(2016,1,31,09,27,0);
	da[36].Set(2016,1,31,09,45,0);
	da[37].Set(2016,1,31,10,10,0);
	da[38].Set(2016,1,31,10,30,0);
	da[39].Set(2016,1,31,10,45,0);
	da[40].Set(2016,1,31,11,00,0);
	da[41].Set(2016,1,31,11,25,0);
	da[42].Set(2016,1,31,11,43,0);
	da[43].Set(2016,1,31,11,55,0);
	da[44].Set(2016,1,31,12,20,0);
	da[45].Set(2016,1,31,12,40,0);
	da[46].Set(2016,1,31,13,01,0);
	da[47].Set(2016,1,31,13,13,0);
	da[48].Set(2016,1,31,13,35,0);
	da[49].Set(2016,1,31,14,00,0);
	da[50].Set(2016,1,31,15,00,0);
	da[51].Set(2016,1,31,15,15,0);
	da[52].Set(2016,1,31,15,30,0);
	da[53].Set(2016,1,31,15,52,0);
	da[54].Set(2016,1,31,16,02,0);
	//west dewar fill
	da[55].Set(2016,1,31,16,18,0);
	da[56].Set(2016,1,31,19,35,0);
	da[57].Set(2016,1,31,20,08,0);
	da[58].Set(2016,1,31,20,19,0);
	da[59].Set(2016,1,31,20,41,0);
	da[60].Set(2016,1,31,21,05,0);
	da[61].Set(2016,1,31,21,18,0);
	da[62].Set(2016,1,31,21,31,0);
	da[63].Set(2016,1,31,21,43,0);
	da[64].Set(2016,1,31,22,09,0);
	da[65].Set(2016,1,31,22,34,0);
	da[66].Set(2016,1,31,23,04,0);
	da[67].Set(2016,1,31,23,30,0);
	da[68].Set(2016,2,01,00,20,0);
	da[69].Set(2016,2,01,00,47,0);
	da[70].Set(2016,2,01,01,02,0);
	da[71].Set(2016,2,01,01,25,0);
	da[72].Set(2016,2,01,01,36,0);
	da[73].Set(2016,2,01,01,48,0);
	da[74].Set(2016,2,01,02,11,0);
	da[75].Set(2016,2,01,02,25,0);
	da[76].Set(2016,2,01,02,46,0);
	da[77].Set(2016,2,01,02,56,0);
	da[78].Set(2016,2,01,03,07,0);
	da[79].Set(2016,2,01,03,30,0);
	da[80].Set(2016,2,01,03,45,0);
	da[81].Set(2016,2,01,04,14,0);
	da[82].Set(2016,2,01,04,22,0);
	da[83].Set(2016,2,01,04,44,0);
	da[84].Set(2016,2,01,05,12,0);
	da[85].Set(2016,2,01,05,29,0);
	da[86].Set(2016,2,01,06,03,0);
	da[87].Set(2016,2,01,06,25,0);
	da[88].Set(2016,2,01,06,47,0);
	da[89].Set(2016,2,01,07,04,0);
	da[90].Set(2016,2,01,07,29,0);
	//west dewar fill  week 2
	da[91].Set(2016,2,05,22,30,0);
	da[92].Set(2016,2,05,22,58,0);
	da[93].Set(2016,2,05,23,21,0);
	da[94].Set(2016,2,05,23,46,0);
	da[95].Set(2016,2,06,00,08,0);
	da[96].Set(2016,2,06,00,24,0);
	da[97].Set(2016,2,06,00,38,0);
	da[98].Set(2016,2,06,01,12,0);
	da[99].Set(2016,2,06,01,28,0);
	da[100].Set(2016,2,06,01,55,0);
	da[101].Set(2016,2,06,02,15,0);
	da[102].Set(2016,2,06,02,26,0);
	da[103].Set(2016,2,06,02,50,0);
	da[104].Set(2016,2,06,03,09,0);
	da[105].Set(2016,2,06,03,29,0);
	da[106].Set(2016,2,06,03,43,0);
	da[107].Set(2016,2,06,03,55,0);
	da[108].Set(2016,2,06,04,06,0);
	//long break, possible fill?
	da[109].Set(2016,2,06,16,30,0);
	da[110].Set(2016,2,06,16,56,0);
	da[111].Set(2016,2,06,17,18,0);
	da[112].Set(2016,2,06,17,28,0);
	da[113].Set(2016,2,06,21,11,0);
	da[114].Set(2016,2,06,21,35,0);
	da[115].Set(2016,2,06,21,48,0);
	da[116].Set(2016,2,06,22,04,0);
	da[117].Set(2016,2,06,22,19,0);
	da[118].Set(2016,2,06,22,40,0);
	da[119].Set(2016,2,06,22,56,0);
	da[120].Set(2016,2,06,23,22,0);
	da[121].Set(2016,2,06,23,40,0);
	da[122].Set(2016,2,06,23,59,0);
	da[123].Set(2016,2,07,00,13,0);
	da[124].Set(2016,2,07,00,25,0);
	da[125].Set(2016,2,07,00,46,0);
	da[126].Set(2016,2,07,01,11,0);
	da[127].Set(2016,2,07,02,01,0);
	da[128].Set(2016,2,07,02,12,0);
	da[129].Set(2016,2,07,02,24,0);
	da[130].Set(2016,2,07,02,51,0);
	da[131].Set(2016,2,07,03,06,0);
	da[132].Set(2016,2,07,03,35,0);
	da[133].Set(2016,2,07,03,46,0);
	da[134].Set(2016,2,07,03,57,0);
	da[135].Set(2016,2,07,04,20,0);
	da[136].Set(2016,2,07,04,38,0);
	da[137].Set(2016,2,07,05,02,0);
	da[138].Set(2016,2,07,05,15,0);
	da[139].Set(2016,2,07,06,12,0);
	da[140].Set(2016,2,07,06,16,0);
	da[141].Set(2016,2,07,06,22,0);
	da[142].Set(2016,2,07,06,47,0);
	da[143].Set(2016,2,07,07,00,0);
	da[144].Set(2016,2,07,07,22,0);
	da[145].Set(2016,2,07,07,32,0);
	da[146].Set(2016,2,07,07,44,0);
	da[147].Set(2016,2,07,07,50,0);
	da[148].Set(2016,2,07,08,15,0);
	da[149].Set(2016,2,07,08,35,0);
	da[150].Set(2016,2,07,08,58,0);
	da[151].Set(2016,2,07,09,09,0);
	da[152].Set(2016,2,07,09,20,0);
	da[153].Set(2016,2,07,09,44,0);
	da[154].Set(2016,2,07,10,10,0);
	da[155].Set(2016,2,07,10,31,0);
	da[156].Set(2016,2,07,10,44,0);
	da[157].Set(2016,2,07,10,58,0);
	da[158].Set(2016,2,07,11,20,0);
	da[159].Set(2016,2,07,11,38,0);
	da[160].Set(2016,2,07,11,59,0);
	da[161].Set(2016,2,07,12,10,0);
	da[162].Set(2016,2,07,12,24,0);
	da[163].Set(2016,2,07,12,47,0);
	da[164].Set(2016,2,07,13,24,0);
	da[165].Set(2016,2,07,13,46,0);
	da[166].Set(2016,2,07,13,58,0);
	da[167].Set(2016,2,07,14,11,0);
	da[168].Set(2016,2,07,14,36,0);
	da[169].Set(2016,2,07,14,53,0);
	da[170].Set(2016,2,07,15,15,0);
	da[171].Set(2016,2,07,15,20,0);
	//west dewar refill
	da[172].Set(2016,2,07,17,56,0);
	da[173].Set(2016,2,07,18,56,0);
	da[174].Set(2016,2,07,19,23,0);
	da[175].Set(2016,2,07,20,12,0);
	da[176].Set(2016,2,07,21,44,0);
	da[177].Set(2016,2,07,22,14,0);
	da[178].Set(2016,2,07,22,41,0);
	da[179].Set(2016,2,07,23,11,0);
	da[180].Set(2016,2,07,01,33,0);
	da[181].Set(2016,2,07,03,05,0);
	da[182].Set(2016,2,07,03,07,0);
	da[183].Set(2016,2,07,03,28,0);
	da[184].Set(2016,2,07,04,01,0);
	da[185].Set(2016,2,07,05,00,0);
}
				
bool Open(int run) {	
	if (run <= 90)
		ucnf.SetPath(janpath.Data());
	else
		ucnf.SetPath(febpath.Data());
	return ucnf.Open(run);
}

TH2D* HVonUCNon;
TH2D* HVoffUCNon;
TH2D* HVonUCNoff;
void Loop(int run = -1) {
	Setdead();
	SetDateTimes();
	
  struct event_t{
	Double_t Esum;
	Double_t Eprev;
	Double_t dt;
	int run;
	int HV;
	unsigned int time;
  }; 
  event_t myEvent;
	
	TFile* RootFile = new TFile("Coincs.root","RECREATE");
	TTree* RootTree = new TTree("t","t");	
	RootTree->Branch("Esum",&myEvent.Esum,"Esum/D");
	RootTree->Branch("Eprev",&myEvent.Eprev,"Eprev/D");
	RootTree->Branch("dt",&myEvent.dt,"dt/D");
	RootTree->Branch("run",&myEvent.run,"run/I");
	RootTree->Branch("HV",&myEvent.HV,"HV/I");
	RootTree->Branch("time",&myEvent.time,"time/i");
	gROOT->cd();

	TH2D* h = (TH2D*) gROOT->FindObject("HVUCN");  if (h != 0) delete h;
	h = (TH2D*) gROOT->FindObject("UCN");  if (h != 0) delete h;
	h = (TH2D*) gROOT->FindObject("HV");  if (h != 0) delete h;
	HVonUCNon = new TH2D("HVUCN","",300,-100,500,50,0,100);
	HVoffUCNon = new TH2D("UCN","",300,-100,500,50,0,100);
	HVonUCNoff = new TH2D("HV","",300,-100,500,50,0,100);
	int pcnt = 0;
	int bcnt = 0;
	int scnt = 0;
	double ptime = 0;
	double btime = 0;
	double stime = 0;
	int start = run;
	int stop = run+1;
	TDatime Toffset(2003,01,01,00,00,00);
	int offset = Toffset.Convert();
	if (run == -1) {start = 0; stop = 185;}
	for (int r=start;r<stop;r++) {
		myEvent.run = r;
		myEvent.time = da[r].Convert()-offset;
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
				if (((mask & deadmask[r]) == 0) && ((mask2 & deadmask2[r]) == 0) && ucnf.myEvent.tprev*4.e-3 < 500 && westside&& !badt && ucnf.myEvent.Eprev < 2000 && westprev) { 
				//Good event
					if (ucnf.myEvent.tprev*4.e-3 > 3 && ucnf.myEvent.tprev*4.e-3 < 200 && ucnf.myEvent.Esum > 5 && ucnf.myEvent.Esum < 25  && goodprev) {
						cnt++;
						Imaproton = true;
					}
					gROOT->cd();
					if (UCNon[r] && HVon[r]) {
						HVonUCNon->Fill(ucnf.myEvent.tprev*4.e-3,ucnf.myEvent.Esum);
						myEvent.Esum = ucnf.myEvent.Esum;
						myEvent.Eprev = ucnf.myEvent.Eprev;
						myEvent.dt = ucnf.myEvent.tprev*4.e-3;
						myEvent.HV = 1;
						RootFile->cd();
						RootTree->Fill();
						gROOT->cd();
					}
					else if (UCNon[r] && !HVon[r]) {
						HVoffUCNon->Fill(ucnf.myEvent.tprev*4.e-3,ucnf.myEvent.Esum);
						myEvent.Esum = ucnf.myEvent.Esum;
						myEvent.Eprev = ucnf.myEvent.Eprev;
						myEvent.dt = ucnf.myEvent.tprev*4.e-3;
						myEvent.HV = 0;
						RootFile->cd();
						RootTree->Fill();
						gROOT->cd();						
					}
					else if (!UCNon[r] && HVon[r])
						HVonUCNoff->Fill(ucnf.myEvent.tprev*4.e-3,ucnf.myEvent.Esum);
				}
				//Look backwards
				if (goodprev && westprev && ucnf.myEvent.tprev*4.e-3 < 100 && ((mask & deadmask[r]) == 0) && ((mask2 & deadmask2[r]) == 0) && !Imaproton && ucnf.myEvent.Eprev < 2000 && ucnf.myEvent.Esum < 2000) {
					gROOT->cd();
					if (UCNon[r] && HVon[r]) {
						HVonUCNon->Fill(ucnf.myEvent.tprev*-4.e-3,ucnf.myEvent.Eprev);
						myEvent.Esum = ucnf.myEvent.Eprev;
						myEvent.Eprev = ucnf.myEvent.Esum;
						myEvent.dt = ucnf.myEvent.tprev*-4.e-3;
						myEvent.HV = 1;
						RootFile->cd();
						RootTree->Fill();
						gROOT->cd();
					}
					else if (UCNon[r] && !HVon[r]) {
						HVoffUCNon->Fill(ucnf.myEvent.tprev*-4.e-3,ucnf.myEvent.Eprev);
						myEvent.Esum = ucnf.myEvent.Eprev;
						myEvent.Eprev = ucnf.myEvent.Esum;
						myEvent.dt = ucnf.myEvent.tprev*-4.e-3;
						myEvent.HV = 0;
						RootFile->cd();
						RootTree->Fill();
						gROOT->cd();
					}
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
	cout << "totals" << endl;
	if (ptime != 0) {
		cout << pcnt << "/" << ptime << " = " << (double)pcnt/ptime << endl;
		HVonUCNon->Scale(1./ptime);
	}
	if (btime != 0) {
		cout << bcnt << "/" << btime << " = " << (double)bcnt/btime << endl;
		HVoffUCNon->Scale(1./btime);
	}
	if (stime != 0) {
		cout << scnt << "/" << stime << " = " << (double)scnt/stime << endl;
		HVonUCNoff->Scale(1./stime);
	}
	
	RootFile->cd();
	RootTree->Write();
	RootFile->Close();
	
	gROOT->cd();
	PrettyPlot();
}

void PrettyPlot() {
	gStyle->SetOptStat(0);
//	gStyle->SetPalette(52);
	
	TCanvas* c = new TCanvas("canv","",600,1800);
	gPad->SetTicks(0);
	/*  Top pad  */
	c->Divide(1,3,0.001,0.001);
	TPad* pad = c->cd(1);
	gPad->SetLeftMargin(0.1);
	gPad->SetBottomMargin(0.15);
	gPad->SetTopMargin(0.05);
	gPad->SetRightMargin(0.2);
	gPad->SetLogz();
	TAxis* ax = HVonUCNon->GetXaxis();
	ax->SetTitle("p - e time diff [#mus]");
	ax->CenterTitle();
	ax->SetTitleSize(0.07);
	ax->SetLabelSize(0.07);
	ax->SetTitleFont(132);
	ax->SetLabelFont(132);
	ax->SetNdivisions(6,false);
	ax = HVonUCNon->GetYaxis();
	ax->SetTitle("Energy [keV]");
	ax->CenterTitle();
	ax->SetTitleOffset(0.7);
	ax->SetTitleSize(0.07);
	ax->SetLabelSize(0.07);
	ax->SetTitleFont(132);
	ax->SetLabelFont(132);
	ax->SetNdivisions(5,false);
	ax = HVonUCNon->GetZaxis();
	ax->SetTitle("Counts / bin / s");
	ax->CenterTitle();
	ax->SetTitleOffset(0.9);
	ax->SetTitleSize(0.07);
	ax->SetRangeUser(0,1.e-3);
	ax->SetLabelSize(0.07);
	ax->SetLabelFont(132);
	HVonUCNon->Draw("COLZ");
	/*  Middle pad  */
	pad =(TPad*)c->cd(2);
	gPad->SetLeftMargin(0.1);
	gPad->SetBottomMargin(0.15);
	gPad->SetTopMargin(0.05);
	gPad->SetRightMargin(0.2);
	gPad->SetLogz();
	ax = HVoffUCNon->GetXaxis();
	ax->SetTitle("p - e time diff [#mus]");
	ax->CenterTitle();
	ax->SetTitleSize(0.07);
	ax->SetTitleFont(132);
	ax->SetLabelFont(132);
	ax->SetLabelSize(0.07);
	ax->SetNdivisions(6,false);
	ax = HVoffUCNon->GetYaxis();
	ax->SetTitle("Energy [keV]");
	ax->CenterTitle();
	ax->SetTitleOffset(0.7);
	ax->SetTitleSize(0.07);
	ax->SetLabelSize(0.07);
	ax->SetTitleFont(132);
	ax->SetLabelFont(132);
	ax->SetNdivisions(5,false);
	ax = HVoffUCNon->GetZaxis();
	ax->SetTitle("Counts / bin / s");
	ax->CenterTitle();
	ax->SetTitleOffset(0.9);
	ax->SetTitleSize(0.07);
	ax->SetRangeUser(0,1.e-3);
	ax->SetLabelSize(0.07);
	ax->SetLabelFont(132);
	HVoffUCNon->Draw("COLZ");
	/*  Bottom pad  */
	pad =(TPad*)c->cd(3);
	gPad->SetLeftMargin(0.1);
	gPad->SetBottomMargin(0.15);
	gPad->SetTopMargin(0.05);
	gPad->SetRightMargin(0.2);
	gPad->SetLogz();
	ax = HVonUCNoff->GetXaxis();
	ax->SetTitle("p - e time diff [#mus]");
	ax->CenterTitle();
	ax->SetTitleSize(0.07);
	ax->SetLabelSize(0.07);
	ax->SetTitleFont(132);
	ax->SetLabelFont(132);
	ax->SetNdivisions(6,false);
	ax = HVonUCNoff->GetYaxis();
	ax->SetTitle("Energy [keV]");
	ax->CenterTitle();
	ax->SetTitleOffset(0.7);
	ax->SetTitleSize(0.07);
	ax->SetLabelSize(0.07);
	ax->SetTitleFont(132);
	ax->SetLabelFont(132);
	ax->SetNdivisions(5,false);
	ax = HVonUCNoff->GetZaxis();
	ax->SetTitle("Counts / bin / s");
	ax->CenterTitle();
	ax->SetTitleOffset(0.9);
	ax->SetTitleSize(0.07);
	ax->SetRangeUser(0,1.e-3);
	ax->SetLabelSize(0.07);
	ax->SetLabelFont(132);
	HVonUCNoff->Draw("COLZ");
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


void CoincESpect(int run = -1) {
	Setdead();
	TH1D* h = (TH1D*) gROOT->FindObject("elec");  if (h != 0) delete h;
	TH1D* elec = new TH1D("elec","",1000,0,1000);
	int start = run;
	int stop = run+1;
	if (run == -1) {start = 0; stop = 185;}
	for (int r=start;r<stop;r++) {
		if (!deadfile[r] && UCNon[r] != -1) {  if (Open(r)) {
			int numev = ucnf.GetNumEvents();
			cout << "Run " << r << endl;
			int ev = 0;
			bool westprev = false;
			bool goodprev = false;
			do {
				ucnf.GetEvent(ev);
				int mask = 0, mask2 = 0;
				bool westside = true;
				for (int ch=0;ch<maxch;ch++){
					if (ucnf.myEvent.E[ch] > 0) {
						int bit = 0x1;
						if (ch < 32)
							mask += bit<<ch;
						else
							mask2 += bit<<(ch-32);
						if (ch >= 24) westside = false;
					}
				}
				bool badt = false;
				for (int i=0;i<badtime[r].start.size();i++) {
					if (ucnf.myEvent.t > badtime[r].start[i] && ucnf.myEvent.t < badtime[r].stop[i])
						badt = true;
				}
				if (((mask & deadmask[r]) == 0) && ((mask2 & deadmask2[r]) == 0) && westside && westprev && goodprev && !badt && ucnf.myEvent.tprev*4.e-3 > 3 && ucnf.myEvent.tprev*4.e-3 < 200 && ucnf.myEvent.Eprev < 2000 && ucnf.myEvent.Esum > 5 && ucnf.myEvent.Esum < 25) { 
				//Good event
					gROOT->cd();
					elec->Fill(ucnf.myEvent.Eprev);
				}
				westprev = westside;
				goodprev = (!badt && ((mask & deadmask[r]) == 0) && ((mask2 & deadmask2[r]) == 0));
				ev++;
				if (ev < numev) ucnf.GetEvent(ev);
			}while (ev < numev && ucnf.myEvent.t < tmax[r]);
		}}
	}
	gROOT->cd();
	elec->SetStats(false);
	elec->Draw();

}
