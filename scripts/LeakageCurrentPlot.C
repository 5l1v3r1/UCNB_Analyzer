// Copyright 2016.  UT-Battelle, LLC.
// This file is part of UCNB_Analyzer.
// See LICENSE.md included in top directory of this distribution.

// Plot of leakage current data taken by Sky Sjue

double LeakageCurrentPlot() {
	
	ifstream leakfile("scripts/t-v-i.dat");
	double x[29], y[29];
	int cnt;
	for(int i=0; i<29; i++) {
		leakfile>>x[i]>>y[i];
    }
	c = new TCanvas("canv","",600,400);
	c->SetLogy();
	/*Pretty plot*/
	gPad->SetLeftMargin(0.15);
	gPad->SetBottomMargin(0.15);
	gPad->SetTopMargin(0.05);
	gPad->SetRightMargin(0.05);
	TGraph* g = new TGraph(29,x,y);
	g->SetMarkerStyle(20);
	g->SetTitle("");
	ax = g->GetXaxis();
	ax->SetTitle("Detector temperature [K]");
	ax->CenterTitle();
	ax->SetTitleOffset(1.1);
	ax->SetTitleSize(0.07);
	ax->SetLabelSize(0.07);
	ax->SetTitleFont(132);
	ax->SetLabelFont(132);
	ax->SetNdivisions(8,true);
	ax = g->GetYaxis();
	ax->SetRangeUser(1.e-3,1e2);
	ax->SetTitle("Leakage current [nA]");
	ax->CenterTitle();
	ax->SetTitleOffset(0.95);
	ax->SetTitleSize(0.07);
	ax->SetLabelSize(0.07);
	ax->SetTitleFont(132);
	ax->SetLabelFont(132);
	ax->SetNdivisions(5,true);
	
	g->Draw("AP");
	
	
}