// Copyright 2016.  UT-Battelle, LLC.
// This file is part of UCNB_Analyzer.
// See LICENSE.md included in top directory of this distribution.

// Plot of shaping parameters per pixel



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
	ax->SetTitle("Pixel leakage current (nA)");
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
	ax->SetTitle("Min. FWHM (keV)");
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