// Copyright 2016.  UT-Battelle, LLC.
// This file is part of UCNB_Analyzer.
// See LICENSE.md included in top directory of this distribution.

// Plot of shaping parameters per pixel

double PlotRevised() {
	
	c = new TCanvas("canv","",600,800);
	c->Divide(1,2);
	c->cd(1);
	
	const int NW = 16;
	double xW[NW] = {39,40,41,  50,52,53,
	63,64,65,66,67,  75,78,
	87,88,89};
	double h1W[NW] = {1.0,0.9,1.2,  1.1,0.9,1.0, /*39-53*/
	1.3,0.9,1.1,2.4,2.2,  1.3,1.8, /*54-78*/
	1.5,2.0,1.8}; /*79-89*/
	for (int i=0;i<NW;i++) h1W[i] = h1W[i]*1.e4;
	TGraph* g1W = new TGraph(NW,xW,h1W);
	g1W->SetMarkerStyle(20);
	g1W->SetMarkerSize(1);
	g1W->SetTitle("");
	double h2W[NW] = {8.2,9.2,6.9,  7.2,8.9,9.3, /*39-53*/
	8.4,6.7,9.2,12.4,10.5,  6.1,10.0, /*54-78*/
	9.3,8.2,8.8}; /*79-89*/
	for (int i=0;i<NW;i++) h2W[i] = h2W[i]*1.e4;
	TGraph* g2W = new TGraph(NW,xW,h2W);
	g2W->SetMarkerStyle(22);
	g2W->SetMarkerSize(1);
	g2W->SetTitle("");
	double h3W[NW] = {7.0,7.7,7.0,  6.8,6.9,7.0, /*39-53*/
	8.0,5.9,6.8,7.7,8.8,  6.4,6.1, /*54-78*/
	4.4,7.7,5.2}; /*79-89*/
	for (int i=0;i<NW;i++) h3W[i] = h3W[i]*1.e4;
	TGraph* g3W = new TGraph(NW,xW,h3W);
	g3W->SetMarkerStyle(4);
	g3W->SetMarkerSize(1);
	g3W->SetTitle("");
	
   
	gPad->SetLeftMargin(0.125);
	gPad->SetBottomMargin(0.15);
	gPad->SetTopMargin(0.08);
	gPad->SetRightMargin(0.05);
	
	TAxis* ax = g1W->GetXaxis();
	
	ax->SetTitle("West Pixel Label");
	ax->CenterTitle();
	ax->SetTitleOffset(1);
	ax->SetTitleSize(0.07);
	ax->SetLabelSize(0.06);
	ax->SetTitleFont(132);
	ax->SetLabelFont(132);
	ax = g1W->GetYaxis();
	ax->SetTitle("Coefficient value");
	ax->CenterTitle();
	ax->SetRangeUser(0,1.4e5);
	ax->SetTitleOffset(0.9);
	ax->SetTitleSize(0.07);
	ax->SetLabelSize(0.07);
	ax->SetTitleFont(132);
	ax->SetLabelFont(132);
	ax->SetNdivisions(5,true);
	
	g1W->Draw("AP");
	g2W->Draw("P");
	g3W->Draw("P");
   
   TLegend *leg = new TLegend(0.175,0.7,0.5,0.9,NULL,"brNDC");
   {
	   leg->SetNColumns(2);
   leg->SetBorderSize(1);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(1001);
   TLegendEntry *entry=leg->AddEntry("g1W","h1 [e^{2} #mu s]","P");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(20);
   entry->SetMarkerSize(1);
   entry->SetTextFont(132);
   TLegendEntry *entry=leg->AddEntry("g2W","h2 [e^{2}]","P");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(22);
   entry->SetMarkerSize(1);
   entry->SetTextFont(132);
   TLegendEntry *entry=leg->AddEntry("g3W","h3 [e^{2}/#mu s]","P");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(4);
   entry->SetMarkerSize(1);
   entry->SetTextFont(132);
   leg->Draw();
   }
	
	const int NE = 17;
	double xE[NE] = {39,40,41,  50,51,53,
	63,64,65,66,67,  75,77,78,
	87,88,89};
	double h1E[NE] = {0.9,1.0,1.1,  1.1,0.8,1.1,
	0.9,2.3,1.0,1.2,0.9,  2.0,1.8,4.3,
	2.6,1.7,9.2};
	for (int i=0;i<NE;i++) h1E[i] = h1E[i]*1.e4;
	TGraph* g1E = new TGraph(NE,xE,h1E);
	g1E->SetMarkerStyle(20);
	g1E->SetMarkerSize(1);
	g1E->SetTitle("");
	double h2E[NE] = {6.7,7.5,7.0,  7.6,6.4,6.2,
	7.8,8.7,5.3,7.4,8.5,  7.0,13.0,5.9,
	8.9,8.3,5.0};
	for (int i=0;i<NE;i++) h2E[i] = h2E[i]*1.e4;
	TGraph* g2E = new TGraph(NE,xE,h2E);
	g2E->SetMarkerStyle(22);
	g2E->SetMarkerSize(1);
	g2E->SetTitle("");
	double h3E[NE] = {6.0,5.8,6.3,  5.8,6.3,5.9,
	6.1,5.5,6.5,5.7,6.5,  6.1,4.0,10.2,
	3.9,4.0,13.0};
	for (int i=0;i<NE;i++) h3E[i] = h3E[i]*1.e4;
	TGraph* g3E = new TGraph(NE,xE,h3E);
	g3E->SetMarkerStyle(4);
	g3E->SetMarkerSize(1);
	g3E->SetTitle("");
	
	c->cd(2);
   
	gPad->SetLeftMargin(0.125);
	gPad->SetBottomMargin(0.15);
	gPad->SetTopMargin(0.08);
	gPad->SetRightMargin(0.05);
	
	ax = g1E->GetXaxis();
	
	ax->SetTitle("East Pixel Label");
	ax->CenterTitle();
	ax->SetTitleOffset(1);
	ax->SetTitleSize(0.07);
	ax->SetLabelSize(0.06);
	ax->SetTitleFont(132);
	ax->SetLabelFont(132);
	ax = g1E->GetYaxis();
	ax->SetTitle("Coefficient value");
	ax->CenterTitle();
	ax->SetRangeUser(0,1.4e5);
	ax->SetTitleOffset(0.9);
	ax->SetTitleSize(0.07);
	ax->SetLabelSize(0.07);
	ax->SetTitleFont(132);
	ax->SetLabelFont(132);
	ax->SetNdivisions(5,true);
	
	g1E->Draw("AP");
	g2E->Draw("P");
	g3E->Draw("P");
	

   leg = new TLegend(0.175,0.7,0.5,0.9,NULL,"brNDC");
   {
	   leg->SetNColumns(2);
   leg->SetBorderSize(1);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(1001);
   TLegendEntry *entry=leg->AddEntry("g1E","h1 [e^{2} #mu s]","P");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(20);
   entry->SetMarkerSize(1);
   entry->SetTextFont(132);
   TLegendEntry *entry=leg->AddEntry("g2E","h2 [e^{2}]","P");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(22);
   entry->SetMarkerSize(1);
   entry->SetTextFont(132);
   TLegendEntry *entry=leg->AddEntry("g3E","h3 [e^{2}/#mu s] ","P");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(4);
   entry->SetMarkerSize(1);
   entry->SetTextFont(132);
   TLegendEntry *entry=leg->AddEntry((TObject*)0, "", "");
   leg->Draw();
   }
	
}


double Plot() {
	
	c = new TCanvas("canv","",600,400);
	c->SetLogy();
	
	const Int_t nx = 38;
	const char *pix[38]={"39W","40W","41W","50W","51W","52W","53W","62W","63W","64W","* 65W","* 66W","75W","76W","77W","* 78W","* 87W","* 88W","* 89W","39E","40E","41E","50E","51E","52E","53E","62E","63E","64E","65E","66E","* 75E","76E","* 77E","78E","* 87E","* 88E","* 89E"};
	double h1val[nx] = {1.0,0.9,1.2,1.1,0.0,0.9,1.0,
		1.3,0.9,1.1,2.4,2.2,1.3,0,0,1.8,1.5,2.0,1.8,
		0.9,1.0,1.1,1.1,0.8,0,1.1,
		0.9,2.3,1.0,1.2,0.9,2.0,0,1.8,4.3,2.6,1.7,9.2};
	double h2val[nx] = {8.2,9.2,6.9,7.2,0.0,8.9,9.3,
		8.4,6.7,9.2,12.4,10.5,6.1,0,0,10.0,9.3,8.2,8.8,
		6.7,7.5,7.0,7.6,6.4,0,6.2,
		7.8,8.7,5.3,7.4,8.5,7.0,0,13.0,5.9,8.9,8.3,5.0};
	double h3val[nx] = {7.0,7.7,7.0,6.8,0.0,6.9,7.0,
		8.0,5.9,6.8,7.7,8.8,6.4,0,0,6.1,4.4,7.7,5.2,
		6.0,5.8,6.3,5.8,6.3,0,5.9,
		6.1,5.5,6.5,5.7,6.5,6.1,0,4.0,10.2,3.9,4.0,13.0};
	
   TH1F *h = new TH1F("h","",4,0,4);
   h->SetStats(0);
   h->SetFillColor(kBlack);
   h->SetLineColor(kBlack);
   h->SetFillStyle(3002);
   h->SetBit(TH1::kCanRebin);
   TH1F *h2 = new TH1F("h2","",4,0,4);
   h2->SetStats(0);
   h2->SetFillColor(kBlack);
   h2->SetLineColor(kBlack);
   h2->SetLineWidth(2);
   h2->SetFillStyle(0);
   h2->SetBit(TH1::kCanRebin);
   TH1F *h3 = new TH1F("h3","",4,0,4);
   h3->SetStats(0);
   h3->SetFillColor(kBlack);
   h3->SetLineColor(kBlack);
   h3->SetFillStyle(3005);
   h3->SetBit(TH1::kCanRebin);
   for (Int_t i=0;i<nx;i++) {
      h->Fill(pix[i],h1val[i]*1.e4);
      h2->Fill(pix[i],h2val[i]*1.e4);
      h3->Fill(pix[i],h3val[i]*1.e4);
   }
   h->LabelsDeflate();
	ax = h->GetXaxis();
	ax->LabelsOption("V");
	ax->SetTitle("Pixel");
	ax->CenterTitle();
	ax->SetTitleOffset(1.3);
	ax->SetTitleSize(0.07);
	ax->SetLabelSize(0.06);
	ax = h->GetYaxis();
	ax->SetTitle("Coefficient value");
	ax->CenterTitle();
	ax->SetRangeUser(5.e3,3.e5);
	ax->SetTitleOffset(0.9);
	ax->SetTitleSize(0.07);
	ax->SetLabelSize(0.07);
	ax->SetTitleFont(132);
	ax->SetLabelFont(132);
	ax->SetNdivisions(5,true);
	ax->SetTickLength(0);
   
	gPad->SetLeftMargin(0.125);
	gPad->SetBottomMargin(0.2);
	gPad->SetTopMargin(0.05);
	gPad->SetRightMargin(0.05);
   h->Draw();
   h2->Draw("Same");
   h3->Draw("same");
   
   TLegend *leg = new TLegend(0.15,0.8,0.425,0.925,NULL,"brNDC");
   {
	   leg->SetNColumns(3);
   leg->SetBorderSize(1);
   leg->SetLineColor(0);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(1001);
   TLegendEntry *entry=leg->AddEntry("h","h1","f");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(20);
   entry->SetMarkerSize(1);
   entry->SetTextFont(132);
   TLegendEntry *entry=leg->AddEntry("h2","h2","f");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(4);
   entry->SetMarkerSize(1);
   entry->SetTextFont(132);
   TLegendEntry *entry=leg->AddEntry("h3","h3","f");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(4);
   entry->SetMarkerSize(1);
   entry->SetTextFont(132);
   leg->Draw();
   }
}

void PlotI_LRevised() {
	
	c = new TCanvas("canv","",600,800);
	c->Divide(1,2);
	c->cd(1);
	
	const int NW = 16;
	double xW[NW] = {39,40,41,  50,52,53,
	63,64,65,66,67,  75,78,
	87,88,89};
	double h1W[NW] = {6.7,7.4,6.8,  6.5,6.6,6.7, /*39-53*/
	7.7,5.6,6.4,7.4,8.5,  6.1,5.9, /*54-78*/
	4.2,7.4,5.0}; /*79-89*/
	TGraph* g1W = new TGraph(NW,xW,h1W);
	g1W->SetMarkerStyle(20);
	g1W->SetMarkerSize(1);
	g1W->SetTitle("");
   
	gPad->SetLeftMargin(0.125);
	gPad->SetBottomMargin(0.15);
	gPad->SetTopMargin(0.08);
	gPad->SetRightMargin(0.05);
	
	TAxis* ax = g1W->GetXaxis();
	
	ax->SetTitle("West Pixel Label");
	ax->CenterTitle();
	ax->SetTitleOffset(1);
	ax->SetTitleSize(0.07);
	ax->SetLabelSize(0.06);
	ax->SetTitleFont(132);
	ax->SetLabelFont(132);
	ax = g1W->GetYaxis();
	ax->SetTitle("Pixel Leakage Current [nA]");
	ax->CenterTitle();
	ax->SetRangeUser(0,13);
	ax->SetTitleOffset(0.9);
	ax->SetTitleSize(0.07);
	ax->SetLabelSize(0.07);
	ax->SetTitleFont(132);
	ax->SetLabelFont(132);
	ax->SetNdivisions(5,true);
	
	g1W->Draw("AP");
		
	const int NE = 17;
	double xE[NE] = {39,40,41,  50,51,53,
	63,64,65,66,67,  75,77,78,
	87,88,89};
	double h1E[NE] = {5.7,5.6,5.6,  5.5,6.0,5.6,
	5.9,5.3,6.2,5.5,6.2,  5.9,3.8,9.7,
	3.8,3.8,12.5};
	TGraph* g1E = new TGraph(NE,xE,h1E);
	g1E->SetMarkerStyle(20);
	g1E->SetMarkerSize(1);
	g1E->SetTitle("");
	
	c->cd(2);
   
	gPad->SetLeftMargin(0.125);
	gPad->SetBottomMargin(0.15);
	gPad->SetTopMargin(0.08);
	gPad->SetRightMargin(0.05);
	
	ax = g1E->GetXaxis();
	
	ax->SetTitle("East Pixel Label");
	ax->CenterTitle();
	ax->SetTitleOffset(1);
	ax->SetTitleSize(0.07);
	ax->SetLabelSize(0.06);
	ax->SetTitleFont(132);
	ax->SetLabelFont(132);
	ax = g1E->GetYaxis();
	ax->SetTitle("Pixel Leakage Current [nA]");
	ax->CenterTitle();
	ax->SetRangeUser(0,13);
	ax->SetTitleOffset(0.9);
	ax->SetTitleSize(0.07);
	ax->SetLabelSize(0.07);
	ax->SetTitleFont(132);
	ax->SetLabelFont(132);
	ax->SetNdivisions(5,true);
	
	g1E->Draw("AP");
}


void PlotI_L() {
	
	c = new TCanvas("canv","",600,400);
	
	const Int_t nx = 38;
	const char *pix[38]={"39W","40W","41W","50W","51W","52W","53W","62W","63W","64W","* 65W","* 66W","75W","76W","77W","* 78W","* 87W","* 88W","* 89W","39E","40E","41E","50E","51E","52E","53E","62E","63E","64E","65E","66E","* 75E","76E","* 77E","78E","* 87E","* 88E","* 89E"};
	double IL[nx] =    {6.7,7.4,6.8,6.5,0.0,6.6,6.7,
		7.7,5.6,6.4,7.4,8.5,6.1,0,0,5.9,4.2,7.4,5.0,
		5.7,5.6,5.6,5.5,6.0,0,5.6,
		5.9,5.3,6.2,5.5,6.2,5.9,0,3.8,9.7,3.8,3.8,12.5};
	
   TH1F *h = new TH1F("hI","",4,0,4);
   h->SetStats(0);
   h->SetLineColor(kBlack);
   h->SetBit(TH1::kCanRebin);
   for (Int_t i=0;i<nx;i++) {
      h->Fill(pix[i],IL[i]);
   }
   h->LabelsDeflate();
	ax = h->GetXaxis();
	ax->LabelsOption("V");
	ax->SetTitle("Pixel");
	ax->CenterTitle();
	ax->SetTitleOffset(1.3);
	ax->SetTitleSize(0.07);
	ax->SetLabelSize(0.06);
	ax = h->GetYaxis();
	ax->SetTitle("Pixel leakage current[nA]");
	ax->CenterTitle();
	ax->SetTitleOffset(0.7);
	ax->SetTitleSize(0.07);
	ax->SetLabelSize(0.07);
	ax->SetTitleFont(132);
	ax->SetLabelFont(132);
//	ax->SetNdivisions(5,true);
	//ax->SetTickLength(0);
   
	gPad->SetLeftMargin(0.105);
	gPad->SetBottomMargin(0.2);
	gPad->SetTopMargin(0.05);
	gPad->SetRightMargin(0.05);
   h->Draw();
}

PlotMinFWHMRevised() {
	
	c = new TCanvas("canv","",600,800);
	c->Divide(1,2);
	c->cd(1);
	
	const int NW = 16;
	double xW[NW] = {39,40,41,  50,52,53,
	63,64,65,66,67,  75,78,
	87,88,89};
	double ILW[NW] = {3.2,3.3,3.1,  3.1,3.2,3.3, /*39-53*/
	3.3,2.9,3.3,4.0,3.8,  3.0,3.6, /*54-78*/
	3.3,3.5,3.4}; /*79-89*/
	TGraph* g1W = new TGraph(NW,xW,ILW);
	g1W->SetMarkerStyle(20);
	g1W->SetMarkerSize(1);
	g1W->SetTitle("");
		
	gPad->SetLeftMargin(0.125);
	gPad->SetBottomMargin(0.15);
	gPad->SetTopMargin(0.08);
	gPad->SetRightMargin(0.05);
	
	TAxis* ax = g1W->GetXaxis();
	
	ax->SetTitle("West Pixel Label");
	ax->CenterTitle();
	ax->SetTitleOffset(1);
	ax->SetTitleSize(0.07);
	ax->SetLabelSize(0.06);
	ax->SetTitleFont(132);
	ax->SetLabelFont(132);
	ax = g1W->GetYaxis();
	ax->SetTitle("Min. FWHM [keV]");
	ax->CenterTitle();
	ax->SetRangeUser(2.5,4.75);
	ax->SetTitleOffset(0.9);
	ax->SetTitleSize(0.07);
	ax->SetLabelSize(0.07);
	ax->SetTitleFont(132);
	ax->SetLabelFont(132);
	ax->SetNdivisions(5,true);
	
	g1W->Draw("AP");
	
	const int NE = 17;
	double xE[NE] = {39,40,41,  50,51,53,
	63,64,65,66,67,  75,77,78,
	87,88,89};
	double ILE[NE] = {2.9,3.0,3.0,  3.1,2.9,2.9,
	3.1,3.3,2.8,3.1,3.2,  3.3,3.7,3.8,
	3.4,3.2,4.5};
	TGraph* g3E = new TGraph(NE,xE,ILE);
	g3E->SetMarkerStyle(20);
	g3E->SetMarkerSize(1);
	g3E->SetTitle("");
	
	c->cd(2);
   
	gPad->SetLeftMargin(0.125);
	gPad->SetBottomMargin(0.15);
	gPad->SetTopMargin(0.08);
	gPad->SetRightMargin(0.05);
	
	ax = g3E->GetXaxis();
	
	ax->SetTitle("East Pixel Label");
	ax->CenterTitle();
	ax->SetTitleOffset(1);
	ax->SetTitleSize(0.07);
	ax->SetLabelSize(0.06);
	ax->SetTitleFont(132);
	ax->SetLabelFont(132);
	ax = g3E->GetYaxis();
	ax->SetTitle("Min. FWHM [keV]");
	ax->CenterTitle();
	ax->SetRangeUser(2.5,4.75);
	ax->SetTitleOffset(0.9);
	ax->SetTitleSize(0.07);
	ax->SetLabelSize(0.07);
	ax->SetTitleFont(132);
	ax->SetLabelFont(132);
	ax->SetNdivisions(5,true);
	
	g3E->Draw("AP");
	
}


PlotMinFWHM() {
	
	c = new TCanvas("canv","",600,400);
	
	const Int_t nx = 38;
	const char *pix[38]={"39W","40W","41W","50W","51W","52W","53W","62W","63W","64W","* 65W","* 66W","75W","76W","77W","* 78W","* 87W","* 88W","* 89W","39E","40E","41E","50E","51E","52E","53E","62E","63E","64E","65E","66E","* 75E","76E","* 77E","78E","* 87E","* 88E","* 89E"};
	double IL[nx] =    {3.2,3.3,3.1,3.1,0.0,3.2,3.3,
		3.3,2.9,3.3,4.0,3.8,3.0,0,0,3.6,3.3,3.5,3.4,
		2.9,3.0,3.0,3.1,2.9,0,2.9,
		3.1,3.3,2.8,3.1,3.2,3.3,0,3.7,3.8,3.4,3.2,4.5};
	
   TH1F *h = new TH1F("hI","",4,0,4);
   h->SetStats(0);
   h->SetLineColor(kBlack);
   h->SetBit(TH1::kCanRebin);
   for (Int_t i=0;i<nx;i++) {
      h->Fill(pix[i],IL[i]);
   }
   h->LabelsDeflate();
	ax = h->GetXaxis();
	ax->LabelsOption("V");
	ax->SetTitle("Pixel");
	ax->CenterTitle();
	ax->SetTitleOffset(1.3);
	ax->SetTitleSize(0.07);
	ax->SetLabelSize(0.06);
	ax = h->GetYaxis();
	ax->SetTitle("Min. FWHM [keV]");
	ax->CenterTitle();
	ax->SetRangeUser(2.5,4.75);
	ax->SetTitleOffset(0.7);
	ax->SetTitleSize(0.07);
	ax->SetLabelSize(0.07);
	ax->SetTitleFont(132);
	ax->SetLabelFont(132);
	ax->SetNdivisions(5,true);
	//ax->SetTickLength(0);
   
	gPad->SetLeftMargin(0.105);
	gPad->SetBottomMargin(0.2);
	gPad->SetTopMargin(0.05);
	gPad->SetRightMargin(0.05);
   h->Draw();
}