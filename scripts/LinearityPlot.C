// Copyright 2016.  UT-Battelle, LLC.
// This file is part of UCNB_Analyzer.
// See LICENSE.md included in top directory of this distribution.

// Plots result of linearity scan (produced with separate code)

void MakePlots() {
	
	TGaxis::SetMaxDigits(2);
	TCanvas* c1= (TCanvas*)gROOT->FindObject("canv");
	if (c1!=0) delete c1;
	c1 = new TCanvas("canv","",600,400);
	
//draw main pad
   TPad *pad = new TPad("pad","",0,0,1,1);
   pad->SetFillColor(kWhite);
   pad->Draw();
   pad->cd();
	gPad->SetLeftMargin(0.1);
	gPad->SetBottomMargin(0.15);
	gPad->SetTopMargin(0.1);
	gPad->SetRightMargin(0.11);
	
	//create first graph
   TGraphErrors *gre = new TGraphErrors(24);
   gre->SetMarkerStyle(20);P1
   {
   gre->SetPoint(0,4,3706.915);
   gre->SetPointError(0,0,46.17142);
   gre->SetPoint(1,4,3712.369);
   gre->SetPointError(1,0,50.57353);
   gre->SetPoint(2,1.2,1088.629);
   gre->SetPointError(2,0,12.92783);
   gre->SetPoint(3,1.2,1087.51);
   gre->SetPointError(3,0,12.26679);
   gre->SetPoint(4,6,5252.756);
   gre->SetPointError(4,0,253.9185);
   gre->SetPoint(5,6,5167.007);
   gre->SetPointError(5,0,253.7601);
   gre->SetPoint(6,6,5216.589);
   gre->SetPointError(6,0,252.4664);
   gre->SetPoint(7,6,5223.809);
   gre->SetPointError(7,0,251.5261);
   gre->SetPoint(8,3.4,3141.362);
   gre->SetPointError(8,0,11.53817);
   gre->SetPoint(9,3.4,3142.826);
   gre->SetPointError(9,0,11.33212);
   gre->SetPoint(10,2.7,2486.115);
   gre->SetPointError(10,0,7.609153);
   gre->SetPoint(11,2.7,2487.099);
   gre->SetPointError(11,0,7.669671);
   gre->SetPoint(12,5.1,4912.449);
   gre->SetPointError(12,0,106.9241);
   gre->SetPoint(13,5.1,4914.423);
   gre->SetPointError(13,0,106.8374);
   gre->SetPoint(14,1.4,1274.382);
   gre->SetPointError(14,0,10.58772);
   gre->SetPoint(15,1.4,1274.312);
   gre->SetPointError(15,0,10.69361);
   gre->SetPoint(16,4.2,3934.805);
   gre->SetPointError(16,0,87.00496);
   gre->SetPoint(17,4.2,3936.243);
   gre->SetPointError(17,0,87.31609);
   gre->SetPoint(18,2.1,1935.128);
   gre->SetPointError(18,0,10.17374);
   gre->SetPoint(19,2.1,1935.522);
   gre->SetPointError(19,0,10.19932);
   gre->SetPoint(20,1.8,1652.906);
   gre->SetPointError(20,0,9.631963);
   gre->SetPoint(21,1.8,1653.213);
   gre->SetPointError(21,0,9.677217);
   gre->SetPoint(22,2.35,2167.249);
   gre->SetPointError(22,0,8.66594);
   gre->SetPoint(23,2.35,2167.8);
   gre->SetPointError(23,0,8.785928);
   }
   TH1F *Graph_Graph1 = (TH1F*)gROOT->FindObject("Graph_Graph1");
   if (Graph_Graph1 != 0) delete Graph_Graph1;
   Graph_Graph1 = new TH1F("Graph_Graph1","",100,0,5.5);
   Graph_Graph1->SetMinimum(0);
   Graph_Graph1->SetMaximum(6000);
   Graph_Graph1->SetStats(0);
	ax = Graph_Graph1->GetXaxis();
	ax->SetTitle("Input Amplitude (mV)");
	ax->CenterTitle();
	ax->SetTitleOffset(1.05);
	ax->SetTitleSize(0.07);
	ax->SetLabelSize(0.07);
	ax->SetTitleFont(132);
	ax->SetLabelFont(132);
	ax->SetNdivisions(8,true);
	ax = Graph_Graph1->GetYaxis();
	ax->SetTitle("Output Amplitude (a.u.)");
	ax->CenterTitle();
	ax->SetTitleOffset(0.55);
	ax->SetTitleSize(0.07);
	ax->SetLabelSize(0.07);
	ax->SetTitleFont(132);
	ax->SetLabelFont(132);
	ax->SetNdivisions(8,true);
   gre->SetHistogram(Graph_Graph1);
   
   //create fit to line
   TF1 *fll = new TF1("fll","pol1",1.1,3.75);
   fll->SetFillColor(19);
   fll->SetFillStyle(0);
   fll->SetLineColor(kBlack);
   fll->SetLineWidth(2);
   fll->SetChisquare(1.71922);
   fll->SetNDF(12);
   fll->SetParameter(0,-27.8005);
   fll->SetParError(0,9.504955);
   fll->SetParLimits(0,0,0);
   fll->SetParameter(1,932.578);
   fll->SetParError(1,4.126812);
   fll->SetParLimits(1,0,0);
   gre->GetListOfFunctions()->Add(fll);
   gre->GetXaxis()->SetLimits(0,5.5);
   gre->Draw("ap");
   //create residuals plot
   
   gre = new TGraphErrors(24);
   gre->SetName("gres");
   gre->SetTitle("");
   gre->SetMarkerStyle(28);
   gre->SetMarkerColor(kGray+2);
   {
   gre->SetPoint(0,3.996746,0.001228814);
   gre->SetPointError(0,0,0);
   gre->SetPoint(1,3.996746,0.002669492);
   gre->SetPointError(1,0,0);
   gre->SetPoint(2,1.193059,-0.002415254);
   gre->SetPointError(2,0,0);
   gre->SetPoint(3,1.193059,-0.003432203);
   gre->SetPointError(3,0,0);
   gre->SetPoint(4,5.989154,-0.05652542);
   gre->SetPointError(4,0,0);
   gre->SetPoint(5,5.989154,-0.07194915);
   gre->SetPointError(5,0,0);
   gre->SetPoint(6,5.989154,-0.06305085);
   gre->SetPointError(6,0,0);
   gre->SetPoint(7,5.989154,-0.06173729);
   gre->SetPointError(7,0,0);
   gre->SetPoint(8,3.388286,-0.0005084746);
   gre->SetPointError(8,0,0);
   gre->SetPoint(9,3.388286,-4.237288e-05);
   gre->SetPointError(9,0,0);
   gre->SetPoint(10,2.696312,-0.001610169);
   gre->SetPointError(10,0,0);
   gre->SetPoint(11,2.696312,-0.001228814);
   gre->SetPointError(11,0,0);
   gre->SetPoint(12,5.09436,0.0388983);
   gre->SetPointError(12,0,0);
   gre->SetPoint(13,5.09436,0.03932203);
   gre->SetPointError(13,0,0);
   gre->SetPoint(14,1.395879,-0.002669492);
   gre->SetPointError(14,0,0);
   gre->SetPoint(15,1.395879,-0.002711864);
   gre->SetPointError(15,0,0);
   gre->SetPoint(16,4.199566,0.01177966);
   gre->SetPointError(16,0,0);
   gre->SetPoint(17,4.199566,0.01216102);
   gre->SetPointError(17,0,0);
   gre->SetPoint(18,2.099783,0.002372881);
   gre->SetPointError(18,0,0);
   gre->SetPoint(19,2.099783,0.002584746);
   gre->SetPointError(19,0,0);
   gre->SetPoint(20,1.789588,0.001271186);
   gre->SetPointError(20,0,0);
   gre->SetPoint(21,1.789588,0.001440678);
   gre->SetPointError(21,0,0);
   gre->SetPoint(22,2.338395,0.001652542);
   gre->SetPointError(22,0,0);
   gre->SetPoint(23,2.338395,0.00190678);
   gre->SetPointError(23,0,0);
   }


   //create a transparent pad drawn on top of the main pad
   c1->cd(2);
   TPad *overlay = new TPad("overlay","",0,0,1,1);
   
   overlay->SetFillStyle(4000);
   overlay->SetFillColor(0);
   overlay->SetFrameFillStyle(4000);
   overlay->Draw();
   overlay->cd();
   
	gPad->SetLeftMargin(0.1);
	gPad->SetBottomMargin(0.15);
	gPad->SetTopMargin(0.1);
	gPad->SetRightMargin(0.11);
   Double_t xmin = 0;
   Double_t ymin = -0.005;
   Double_t xmax = 5.5;
   Double_t ymax = 0.005;
   TH1F *hframe = overlay->DrawFrame(xmin,ymin,xmax,ymax);
   hframe->GetXaxis()->SetTickLength(0);
   hframe->GetYaxis()->SetTickLength(0);
   hframe->GetXaxis()->SetLabelOffset(99);
   hframe->GetYaxis()->SetLabelOffset(99);
   gre->Draw("p");

   //Draw an axis on the right
   TGaxis *axis = new TGaxis(xmax,ymin,xmax, ymax,ymin,ymax,5,"+L");
//   axis->SetNoExponent(kFALSE);
   axis->SetLineColor(kGray+2);
   axis->SetLabelColor(kGray+2);
//   axis->SetTickSize(0);
   axis->CenterTitle();
   axis->SetLabelSize(0.07);
   axis->SetLabelFont(132);
   axis->SetTitleSize(0.07);
   axis->SetTitleFont(132);
   axis->SetTitleOffset(0.7);
   axis->SetTitle("Fractional residuals");
   axis->SetTitleColor(kGray+2);
   axis->Draw();
   
   TLegend *leg = new TLegend(0.5,0.2,0.85,0.35,NULL,"brNDC");
   {
   leg->SetBorderSize(1);
   leg->SetLineColor(0);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(1001);
   TLegendEntry *entry=leg->AddEntry("gmeas","Measured","P");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(20);
   entry->SetMarkerSize(1);
   entry->SetTextFont(132);
   TLegendEntry *entry=leg->AddEntry("gres","Fractional residuals","P");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(28);
   entry->SetMarkerSize(1);
   entry->SetTextFont(132);
   entry->SetTextColor(kGray+2);
   leg->Draw();
   }
}