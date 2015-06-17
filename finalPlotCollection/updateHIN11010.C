#include <TChain.h>
#include <TGraphAsymmErrors.h>
#include <TCanvas.h>
#include <iostream>
#include <TMath.h>
#include <TROOT.h>
#include <TClonesArray.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TCut.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TLegend.h>
#include <TF1.h>
#include <TProfile.h>
#include <TNtuple.h>
#include <TLine.h>
#include <TGraphErrors.h>
#include <TPolyLine.h>
#include "commonUtility.h"
#include "TGraphHelpers.h"
#include "drawPlotsGammaJetPapers.h"

const int khimc=0;
const int khidata=1;
const int kppdata=2;
const int kppdata13=3;
const int kppmc=10; // z2 tune

void DivideTG(TGraphAsymmErrors* g1=0,TGraphAsymmErrors* g2=0) {
  for (int i=0;i<g1->GetN();i++) {
    double theX1,val1,theX2,val2;
    if ( theX1 != theX2 ) 
      { std::cout <<" error in DivideTG!!!!!" << std::endl;};
    g1->GetPoint(i,theX1,val1);
    g2->GetPoint(i,theX2,val2);
    g1->SetPoint(i,theX1,val1/val2);
  }
}


void updateHIN11010(int etPho = 60, int etJet = 30, bool scaleByR=true, bool drawMC=true, int prodDate=20131021) {
  bool mcOnly=false;

  int percentBin[5] = {0,10,30,50,100};
  TH1D* hxgj[5][10];   //[data kind] [ centrality] 
  TH1D* hxgjpp[20];
  TFile *fSum1 = new TFile(Form("histOutputFiles60GeVInclusive/HisOutput_Photonv7_v29_akPu3PF_InclPtRatio_gamma%djet%ddphiSig2749_Isol0_Norm1.root",etPho,etJet));
  for ( int icent=0; icent<=3 ; icent++) { 
    hxgj[khimc][icent] =  (TH1D*)fSum1->Get(Form("dataSrc%d_reco1_cent%dSubtractedExtrapExtrapNorm",khimc,icent));
    hxgj[khidata][icent] =  (TH1D*)fSum1->Get(Form("dataSrc%d_reco1_cent%dSubtractedExtrapExtrapNorm",khidata,icent));
  }
  hxgjpp[kppdata] = (TH1D*)fSum1->Get("dataSrc2_reco1_cent0SubtractedExtrapExtrapNorm");

  TFile *fSum2 = new TFile(Form("histOutputFiles60GeVInclusive/HisOutput_Photonv7_v29_akPu3PF_InclPtRatio_gamma%djet%ddphiSig2749_Isol0_Norm2.root",etPho,etJet));
  TGraphAsymmErrors* mxhimc = (TGraphAsymmErrors*)fSum2->Get("dataSrc0_reco1_x_Summary_0");
  TGraphAsymmErrors* mxhidata = (TGraphAsymmErrors*)fSum2->Get("dataSrc1_reco1_x_Summary_0");
  //  TGraphAsymmErrors* mxppdata = (TGraphAsymmErrors*)fSum2->Get("dataSrc2_reco1_x_Summary_0");
  TGraphAsymmErrors* mxppdata;
  TGraphAsymmErrors* mxppmc = (TGraphAsymmErrors*)fSum2->Get("dataSrc10_reco1_x_Summary_0");

  TGraphAsymmErrors* rxhimc = (TGraphAsymmErrors*)fSum2->Get("dataSrc0_reco1_R_Summary_0");
  TGraphAsymmErrors* rxhidata = (TGraphAsymmErrors*)fSum2->Get("dataSrc1_reco1_R_Summary_0");
  TGraphAsymmErrors* rxppdata = (TGraphAsymmErrors*)fSum2->Get("dataSrc2_reco1_R_Summary_0");
  TGraphAsymmErrors* rxppmc = (TGraphAsymmErrors*)fSum2->Get("dataSrc10_reco1_R_Summary_0");

  TFile *fSum3 = new TFile(Form("histOutputFiles60GeVInclusive/HisOutput_Photonv7_v29_akPu3PF_InclDeltaPhi_gamma%djet%ddphiSig628_subJ1SS1_Isol0_Norm1.root",etPho,etJet));

  TFile *fSum4 = new TFile("histOutputFiles60GeVInclusive/HisSummary_Photonv7_v29_akPu3PF_InclDeltaPhi_gamma60jet30dphiSig628_subJ1SS1_Isol0_Norm1.root");
  TGraphAsymmErrors* dphihimc = (TGraphAsymmErrors*)fSum4->Get("dataSrc0_reco1_dphi_Summary");
  TGraphAsymmErrors* dphihidata = (TGraphAsymmErrors*)fSum4->Get("dataSrc1_reco1_dphi_Summary");
  TGraphAsymmErrors* dphippdata = (TGraphAsymmErrors*)fSum4->Get("dataSrc2_reco1_dphi_Summary");
  TGraphAsymmErrors* dphippmc = (TGraphAsymmErrors*)fSum4->Get("dataSrc10_reco1_dphi_Summary");

  // new dphihidata points
  //


  double sysDphi[4] = {0.032,0.03,0.045,0.077};
  //0.109940, 0.046998, 0.034206,0.142664};
  double sysDphipp[1] = {sysDphi[3]};
  
  double sysR[4] = {0.045,0.039,0.041,0.037};
  //0.017232,0.012847,0.011691,0.012724};
  double sysRpp[1] = {sysR[3]};
  
  double sysMx[4] = { 0.053,0.048,0.051,0.032};
  double sysMxpp[1] = {0.009};
  double corrSysMx =  0.028;

  TH1D* hdphi[5][5];   //[data kind] [ centrality]                                                                                     
  TH1D* hdphipp[20];
  for ( int icent=0; icent<=3 ; icent++) {
    hdphi[khimc][icent] =  (TH1D*)fSum3->Get(Form("dataSrc%d_reco1_cent%dSubtractedExtrapExtrapNorm",khimc,icent));
    hdphi[khidata][icent] =  (TH1D*)fSum3->Get(Form("dataSrc%d_reco1_cent%dSubtractedExtrapExtrapNorm",khidata,icent));
  }
  hdphipp[kppdata] = (TH1D*)fSum3->Get("dataSrc2_reco1_cent0SubtractedExtrapExtrapNorm");
  
  // 2013 pp data!!!!!!
  TFile* pp13 = new TFile(Form("ffFilesPP60GeVInclusive/photonTrackCorr_ppDATA_output_photonPtThr60_to_9999_jetPtThr30_%d.root", prodDate));
  // TFile* pp13 = new TFile("ffFilesPP60GeVInclusive/oldSmearing.root");

  hdphi[kppdata13][1] =  (TH1D*)pp13->Get("jetDphi_icent10010_final");
  hxgj[kppdata13][1] = (TH1D*)pp13->Get("xjg_icent10010_final");
  hdphi[kppdata13][2] =  (TH1D*)pp13->Get("jetDphi_icent11030_final");
  hxgj[kppdata13][2] = (TH1D*)pp13->Get("xjg_icent11030_final");
  hdphi[kppdata13][3] =  (TH1D*)pp13->Get("jetDphi_icent13050_final");
  hxgj[kppdata13][3] = (TH1D*)pp13->Get("xjg_icent13050_final");
  hdphi[kppdata13][4] =  (TH1D*)pp13->Get("jetDphi_icent15099_final");
  hxgj[kppdata13][4] = (TH1D*)pp13->Get("xjg_icent15099_final");
  hdphi[kppdata13][5] =  (TH1D*)pp13->Get("jetDphi_icent7_final");
  hxgj[kppdata13][5] = (TH1D*)pp13->Get("xjg_icent7_final");
  

  for ( int icent  = 1 ; icent<=5 ; icent++) {
    hdphi[kppdata13][icent]->Scale(1./hdphi[kppdata13][icent]->Integral());
    for ( int i = 1 ; i<=5 ; i++) {
      hdphi[kppdata13][icent]->SetBinContent(i,-1e4);
    }
    //    hxgj[kppdata13][icent]->Rebin(10);  // Now the bins are already rebined from photonTrackCorr_ppDATA_output_photonPtThr60_to_9999_jetPtThr30_20131021.root
    hxgj[kppdata13][icent]->Scale(1./hxgj[kppdata13][icent]->Integral("width"));
  }
  
  TH1D* hPtPP2013[10];  
  hPtPP2013[1] = (TH1D*)pp13->Get("jetPt_icent10010_final");
  hPtPP2013[2] = (TH1D*)pp13->Get("jetPt_icent11030_final");
  hPtPP2013[3] = (TH1D*)pp13->Get("jetPt_icent13050_final");
  hPtPP2013[4] = (TH1D*)pp13->Get("jetPt_icent15099_final");
  hPtPP2013[5] = (TH1D*)pp13->Get("jetPt_icent7_final");
  
  TH1D* hRpp2013[10]; 
  hRpp2013[1] = new TH1D("hrpp2013_icent1","",1, 359.1-10, 359.1+10);
  hRpp2013[2] = new TH1D("hrpp2013_icent2","",1, 235.6-10, 235.6+10);
  hRpp2013[3] = new TH1D("hrpp2013_icent3","",1, 116.4-10, 116.4+10);
  hRpp2013[4] = new TH1D("hrpp2013_icent4","",1, 43.6-10,  43.6 +10);
  hRpp2013[5] = new TH1D("hrpp2013_icent5","",1, -8,  18 );

  for ( int icent=1 ;  icent<=5; icent++) { 
    double temprPP13err;
    double temprPP13 = hPtPP2013[icent]->IntegralAndError(1,hPtPP2013[icent]->GetNbinsX(),temprPP13err,"width");
    hRpp2013[icent]->SetBinContent(1,temprPP13);
    hRpp2013[icent]->SetBinError(1,temprPP13err);
    handsomeTH1(hRpp2013[icent],1);
    hRpp2013[icent]->SetMarkerStyle(21);
  }
  
  TH1D* hMXpp2013[10];
  hMXpp2013[1] = new TH1D("hmxpp2013_icent1","",1, 359.1-10, 359.1+10);
  hMXpp2013[2] = new TH1D("hmxpp2013_icent2","",1, 235.6-10, 235.6+10);
  hMXpp2013[3] = new TH1D("hmxpp2013_icent3","",1, 116.4-10, 116.4+10);
  hMXpp2013[4] = new TH1D("hmxpp2013_icent4","",1, 43.6-10,  43.6 +10);
  hMXpp2013[5] = new TH1D("hmxpp2013_icent5","",1, -8, 18);
  for ( int icent=1 ;  icent<=5; icent++) {
    hMXpp2013[icent]->SetBinContent(1,hxgj[kppdata13][icent]->GetMean());
    hMXpp2013[icent]->SetBinError(1,hxgj[kppdata13][icent]->GetMeanError());
    handsomeTH1(hMXpp2013[icent],1);
    hMXpp2013[icent]->SetMarkerStyle(21);
  }


  TFile* fPPsys = new TFile("ffFilesPP60GeVInclusive/relativeSys_merged_pp60GeV.root");

  TH1D* hdphiWidth  = (TH1D*)fPPsys->Get("dphiWidth_uncertainty_merged");
  
  TH1D* hDphiPPUnc = new TH1D("hdphippunc","",1,0,1);
  hDphiPPUnc->SetBinContent(1, hdphiWidth->GetBinContent(1) );

  TH1D* ppSysX[4];
  ppSysX[0] = (TH1D*)fPPsys->Get("dNdXjg_uncertainty_merged");
  ppSysX[1] = (TH1D*)ppSysX[0]->Clone("ppSysx1");
  ppSysX[2] = (TH1D*)ppSysX[0]->Clone("ppSysx2");
  ppSysX[3] = (TH1D*)ppSysX[0]->Clone("ppSysx3");

  TH1D* meanXpp13Sys = (TH1D*)fPPsys->Get("meanXjg_uncertainty_merged");

  float ppSysMx60 = meanXpp13Sys->GetBinContent(1);    // UPDATED on Oct 22nd
  TH1D* ppSysMx = new TH1D("ppSysMx","",1,0,1);
  ppSysMx->SetBinContent(1,ppSysMx60);

  TH1D* meanRpp13Sys = (TH1D*)fPPsys->Get("meanRjg_uncertainty_merged");
  float ppSysR60 = meanRpp13Sys->GetBinContent(1); // UPDATED on Oct 22nd
  TH1D* ppSysR = new TH1D("ppSysR","",1,0,1);
  ppSysR->SetBinContent(1,ppSysR60);

  
  
  // xjg distributions
  TCanvas *c1 = new TCanvas("c1","",1100,330);
  makeMultiPanelCanvas(c1,4,1,0.0,0.0,0.24,0.15,0.075);
  c1->cd(0);
  drawCMSppPbPb(0.1,0.95);
  //c1->Divide(4,1,0.0,0.0);
  for ( int icent=0; icent<=3 ; icent++) {
    c1->cd( 4 - icent);
    //hxgj[khimc][icent]->SetAxisRange(-.2,2.5,"Y");
    hxgj[khimc][icent]->SetAxisRange(0,2.5,"Y");
    hxgj[khimc][icent]->SetNdivisions(505);
    //    hxgj[khimc][icent]->SetTitle(";x_{J#gamma} = p^{Jet}_{T}/p^{#gamma}_{T}; #frac{1}{N_{J#gamma}} #frac{dN_{J#gamma}}{dx_{J#gamma}}");
    hxgj[khimc][icent]->SetTitle(";x_{J#gamma}; #frac{1}{N_{J#gamma}} #frac{dN_{J#gamma}}{dx_{J#gamma}}");
    handsomeTH1(hxgj[khimc][icent]);
    fixedFontHist(hxgj[khimc][icent],1,1.35);
    mcStyle2(hxgj[khimc][icent]);
    handsomeTH1(hxgj[khidata][icent],2);
    
    hxgj[khimc][icent]->GetYaxis()->SetTitleOffset(1.5);
    TH1D * htemp41 = (TH1D*)hxgj[khimc][icent]->Clone(Form("htemp41_%d",icent));
    for ( int i=0 ; i<=20 ; i++) {
      htemp41->SetBinContent(i,0);
      htemp41->SetBinError(i,0);
    }
    if (scaleByR) {
      htemp41->SetAxisRange(0,2,"Y");
      htemp41->SetYTitle("#frac{1}{N_{#gamma}} #frac{dN_{J#gamma}}{dx_{J#gamma}}");
    }
    htemp41->DrawCopy("hist");

    if (scaleByR)    {
      hxgj[kppdata13][icent+1]->Scale(hRpp2013[icent+1]->GetBinContent(1));
      std::cout <<"  Scaled by pp R :" << hRpp2013[icent+1]->GetBinContent(1) << std::endl;
      
      double x,y;
      rxhidata->GetPoint(icent, x,y);
      hxgj[khidata][icent]->Scale(y);
      std::cout << " scaled by PbPb R: " << y << std::endl;

      rxhimc->GetPoint(icent, x,y);
      hxgj[khimc][icent]->Scale(y);
      std::cout << " scaled by PbPb R: " << y << std::endl;
    }
    if(drawMC) hxgj[khimc][icent]->DrawCopy("hist same");

    if ( !mcOnly )    drawXSys(icent,hxgj[khidata][icent]);
    //   if ( icent==3){ }
    
    handsomeTH1(hxgj[kppdata13][icent+1]);
    hxgj[kppdata13][icent+1]->SetMarkerStyle(21);

    drawSys(hxgj[kppdata13][icent+1],ppSysX[icent],kGreen,3001);

    hxgj[kppdata13][icent+1]->Draw("same ");
    
    if ( !mcOnly )   hxgj[khidata][icent]->Draw("same");
    //onSun(0,0,2,0);
 
    // if ( icent == 2) {
    //   TLegend *leg0  = new TLegend(0.2796373,0.7545885,0.9742202,0.9937661,NULL,"brNDC");
    //     easyLeg(leg0,"");
    //   //      if ( !mcOnly )       leg0->AddEntry(hxgj[khidata][icent],"PbPb Data","p");
    //   leg0->AddEntry(hxgj[khimc][icent],"PYTHIA + HYDJET","f");
    //   leg0->AddEntry(hxgjpp[kppdata],"","");

    //   leg0->Draw();
    // }
    
    if ( icent == 3) {
      TLegend *leg0 = new TLegend(0.2916647,0.7045885,0.9862476,0.9869226,NULL,"brNDC");
      easyLeg(leg0);
      leg0->AddEntry(hxgj[khidata][icent],"PbPb Data","p");
      if ( !mcOnly )       leg0->AddEntry(hxgj[kppdata13][icent+1],"Smeared pp reference","p");
      //    leg0->AddEntry(hxgj[khidata][icent],"","");
      if(drawMC) leg0->AddEntry(hxgj[khimc][icent],"PbPb PYTHIA + HYDJET","f");
      leg0->Draw();
      //drawText("#sqrt{s_{NN}}=2.76 TeV ",0.65,0.74,0,15);
    }
        
    if ( icent == 2) {
      drawText(Form("p^{#gamma}_{T} > %d GeV/c     |#eta^{#gamma}| < 1.44",etPho),0.2,0.85,0,15);
      drawText(Form("p^{Jet}_{T} > %d GeV/c    |#eta^{Jet}| < 1.6",etJet),0.2,0.77,0,15);
      drawText("#Delta#phi_{J#gamma} > #frac{7}{8}#pi",0.2,0.69,0,15);
    }
    if ( icent == 0 ) {
      //      drawText("CMS",0.8,0.9,1);
      //      drawText("pp       #int L dt = 231 nb^{-1}",0.4,0.68,1,15);
    }
    
    if ( icent == 3) 
      drawText(Form("%d%% - %d%%",percentBin[icent],percentBin[icent+1]),0.72,0.5,0,15);
    else 
      drawText(Form("%d%% - %d%%",percentBin[icent],percentBin[icent+1]),0.67,0.5,0,15);
    
    /*    if ( icent == 3)
      drawText("(a)",0.275,0.8,1);
    if ( icent == 2)
      drawText("(b)",0.05,0.8,1);  
    if ( icent == 1)
      drawText("(c)",0.05,0.8,1);   
    if ( icent == 0)
      drawText("(d)",0.05,0.8,1);
    */
    gPad->RedrawAxis();
  }

  c1->SaveAs("plotPPPbPb/inclusivePt_ppPbPb_xjg_distribution.pdf");
  //c1->SaveAs("plotPPPbPb/inclusivePt_ppPbPb_xjg_distribution.png");
  //c1->SaveAs("plotPPPbPb/inclusivePt_ppPbPb_xjg_distribution.gif");
  //c1->SaveAs("plotPPPbPb/inclusivePt_ppPbPb_xjg_distribution.C");

  
  
  // TCanvas *c1all = new TCanvas("c1all","",500,500);

  // for ( int icent=0;icent<4;icent++){
  //   hxgj[khimc][icent]->SetFillStyle(0);
  //   hxgj[khimc][icent]->SetMarkerSize(1.5);
  // }
  // hxgj[khimc][0]->SetMarkerStyle(22);
  // hxgj[khimc][1]->SetMarkerStyle(26);
  // hxgj[khimc][2]->SetMarkerStyle(23);
  // hxgj[khimc][3]->SetMarkerStyle(32);
  // hxgj[khimc][0]->DrawCopy("");
  // hxgj[khimc][1]->DrawCopy(" same");
  // hxgj[khimc][2]->DrawCopy(" same");
  // hxgj[khimc][3]->DrawCopy(" same");
  // jumSun(0,0,2,0);

  // drawText("PYTHIA+HYDJET",0.2,0.80,0,25);
  // TLegend *legc1all = new TLegend(0.6149194,0.6716102,0.9435484,0.9555085,NULL,"brNDC");
  // easyLeg(legc1all,"");
  // legc1all->AddEntry(hxgj[khimc][3],"50-100%","p");
  // legc1all->AddEntry(hxgj[khimc][2],"30-50%","p");
  // legc1all->AddEntry(hxgj[khimc][1],"10-30%","p");
  // legc1all->AddEntry(hxgj[khimc][0],"0-10%","p");
  // legc1all->Draw();
  
  // dphi distributions
  TCanvas *c1ppDphi = new TCanvas("c1ppDphi","",500,500);
  TString fitFunc = "(TMath::Pi()/20.0)*exp(-(TMath::Pi()-x)/[0])/([0]*(1-exp(-TMath::Pi()/[0])))";
  float fitxmin=3.1415926*2./3;
  TF1 *fdphiPP[10];
  TH1D* hDphiPP2013[10];
  hDphiPP2013[1] = new TH1D("hDphiPP2013_icent1","",1, 359.1-10, 359.1+10);
  hDphiPP2013[2] = new TH1D("hDphiPP2013_icent2","",1, 235.6-10, 235.6+10);
  hDphiPP2013[3] = new TH1D("hDphiPP2013_icent3","",1, 116.4-10, 116.4+10);
  hDphiPP2013[4] = new TH1D("hDphiPP2013_icent4","",1, 43.6-10,  43.6 +10);
  hDphiPP2013[5] = new TH1D("hDphiPP2013_icent5","",1, -8, 18);
  
  for ( int icent=1; icent<=5 ; icent++) {
    fdphiPP[icent] = new TF1(Form("fdphiPP_icent%d",icent),fitFunc.Data(),2.0*TMath::Pi()/3.0,TMath::Pi());
    fdphiPP[icent]->SetParName(0,"width");
    fdphiPP[icent]->SetParameter("width",0.3);
    hdphi[kppdata13][icent]->Fit(Form("fdphiPP_icent%d",icent),"0","",fitxmin,3.1415926);
    fdphiPP[icent]->SetLineWidth(1);
    fdphiPP[icent]->SetLineStyle(2);
    fdphiPP[icent]->SetLineColor(1);
    hdphi[kppdata13][icent]->SetAxisRange(1.00001e-3,1,"Y");
    hdphi[kppdata13][icent]->SetStats(0);
    hdphi[kppdata13][icent]->Draw("");
    hdphi[kppdata13][icent]->SetAxisRange(1.00001e-3,1,"Y");
    hdphi[kppdata13][icent]->SetStats(0);
    fdphiPP[icent]->SetLineWidth(2);
    fdphiPP[icent]->SetLineStyle(7); 
    fdphiPP[icent]->DrawCopy("same");
    gPad->SetLogy();
  
    float dphiWidth13 = fdphiPP[icent]->GetParameter(0) ; 
    float dphiWidth13err = fdphiPP[icent]->GetParError(0);
    hDphiPP2013[icent]->SetBinContent(1,dphiWidth13);
    hDphiPP2013[icent]->SetBinError(  1,dphiWidth13err);
    handsomeTH1(hDphiPP2013[icent],1);
    hDphiPP2013[icent]->SetMarkerStyle(21);
  }

  TCanvas *c1dphi = new TCanvas("c1dphi","",1100,330);
  makeMultiPanelCanvas(c1dphi,4,1,0.0,0.0,0.24,0.18,0.075);
  c1dphi->cd(0);
  drawCMSppPbPb(0.1,0.95);

  for ( int icent=0; icent<=3 ; icent++) {
    c1dphi->cd( 4 - icent);
    hdphi[khimc][icent]->SetAxisRange(1.e-3,1,"Y");
    //   hdphi[khimc][icent]->SetNdivisions(505);
    hdphi[khimc][icent]->GetXaxis()->SetNdivisions(3,5,0,kFALSE);
    
    hdphi[khimc][icent]->SetTitle(";#Delta#phi_{J#gamma};Pair Fraction");

    handsomeTH1(hdphi[khimc][icent]);
    fixedFontHist(hdphi[khimc][icent],1,1.35);
    mcStyle2(hdphi[khimc][icent]);
    handsomeTH1(hdphi[khidata][icent],2);

    TF1 *fdphi = new TF1("fdphi",fitFunc.Data(),2.0*TMath::Pi()/3.0,TMath::Pi());
    fdphi->SetParName(0,"width");
    fdphi->SetParameter("width",0.3);
    
    hdphi[khimc][icent]->Fit("fdphi","0llm","",fitxmin,3.1415926);

    fdphi->SetLineWidth(1);
    fdphi->SetLineStyle(2);
    fdphi->SetLineColor(kBlue); 
    // float dphiWidth = fdphi->GetParameter("width");
    // float dphiWidthErr = fdphi->GetParError(0);
    // std::cout << " dphiWidth,dphiWidthErr = " << dphiWidth <<"   "<< dphiWidthErr << std::endl;
    hdphi[khimc][icent]->SetAxisRange(1.00001e-3,1,"Y");
    hdphi[khimc][icent]->SetStats(0);
    TH1D* hdphitemp = (TH1D*)hdphi[khimc][icent]->Clone(Form("hdphitemp55_%d",icent));
    if(!drawMC)
    {
      for ( int i=0;i<=30;i++) { 
	hdphitemp->SetBinContent(i,0);
	hdphitemp->SetBinError(i,0);
      }
    }
    hdphitemp->Draw("hist");
    hdphi[khidata][icent]->SetAxisRange(1.00001e-3,1,"Y");
    if (!mcOnly) hdphi[khidata][icent]->Draw("same ");
    hdphi[khidata][icent]->SetStats(0);
    fdphi->SetLineWidth(2);
    fdphi->SetLineStyle(7);
    if ( mcOnly )    fdphi->DrawCopy("same");


    gPad->SetLogy();
    handsomeTH1(hdphi[kppdata13][icent+1]);
    
    hdphi[kppdata13][icent+1]->SetMarkerStyle(21);
    hdphi[kppdata13][icent+1]->Draw("same");

    
    if ( icent == 3) {
      TLegend *leg0  = new TLegend(0.32,0.7,0.9,0.89,NULL,"brNDC");
      easyLeg(leg0);
      if ( !mcOnly )   leg0->AddEntry(hdphi[kppdata13][icent+1],"Smeared pp reference","p");
      if ( !mcOnly )      leg0->AddEntry(hdphi[khidata][icent],"PbPb Data","p");
      if(drawMC) leg0->AddEntry(hdphi[khimc][icent],"PYTHIA + HYDJET","f");
      leg0->Draw();
    }

    if (( !mcOnly ) && ( icent == 2)) {
      //drawText("#sqrt{s_{NN}}=2.76 TeV ",0.4,0.88,0,15);
      //drawText("#int L dt = 150 #mub^{-1}",0.4,0.75,0,15);
    }

    if ( icent == 1) {
      drawText(Form("p^{#gamma}_{T} > %d GeV/c     |#eta^{#gamma}| < 1.44",etPho),0.15,0.8,0,15);
      drawText(Form("p^{Jet}_{T} > %d GeV/c    |#eta^{Jet}| < 1.6",etJet),0.15,0.7,0,15);
    }

    if ( icent == 0 ) {
      //      drawText("CMS",0.8,0.9,1);
      //      drawText("pp       #int L dt = 231 nb^{-1}",0.4,0.68,1,15);                                                                     
    }


    if ( icent == 3)
      drawText(Form("%d%% - %d%%",percentBin[icent],percentBin[icent+1]),0.313,0.6,0,15);
    else
      drawText(Form("%d%% - %d%%",percentBin[icent],percentBin[icent+1]),0.1,0.6,0,15);

    if ( icent == 3)
      drawText("(a)",0.275,0.8,1);
    if ( icent == 2)
      drawText("(b)",0.05,0.8,1);
    if ( icent == 1)
      drawText("(c)",0.05,0.8,1);
    if ( icent == 0)
      drawText("(d)",0.05,0.8,1);
    
    double bottomY = 0.0009;
    double pi = TMath::Pi();

    drawPatch(-0.5,bottomY/100,pi+0.5,bottomY);

    bottomY = 0.0005;
    drawText("0",0.05,bottomY,0,18,false);
    drawText("#frac{1}{3}#pi",pi/3-0.05,bottomY,0,18,0);
    drawText("#frac{2}{3}#pi",2*pi/3-0.05,bottomY,0,18,0);
    if ( icent==0) 
      drawText("#pi",pi-0.1,bottomY,0,18,0);
    drawText("#Delta#phi_{J#gamma}",pi/2.-0.1,bottomY-0.0002,0,18,0);
    
  }
  


  gPad->RedrawAxis();
  c1dphi->SaveAs("plotPPPbPb/inclusivePt_ppPbPb_dPhi_dist.pdf");
  //c1dphi->SaveAs("plotPPPbPb/inclusivePt_ppPbPb_dPhi_dist.png");
  //c1dphi->SaveAs("plotPPPbPb/inclusivePt_ppPbPb_dPhi_dist.gif");
  //c1dphi->SaveAs("plotPPPbPb/inclusivePt_ppPbPb_dPhi_dist.C");
  
  ////////////////////////////// summary plots
  TCanvas *c2 = new TCanvas("c2","",350,350);
  TH1D* hTemp2 = new TH1D("htemp2",";N_{part};<x_{J#gamma}>",100,-20,400);
  hTemp2->SetNdivisions(505);
  handsomeTH1(hTemp2,1);
  handsomeTGraph(mxhidata,2);
  mxhidata->SetMarkerStyle(20);
  handsomeTGraph(mxhimc,1);
  mxhimc->SetMarkerStyle(24);
  //  handsomeTGraph(mxppdata,1);
  //  mxppdata->SetMarkerStyle(21);
  handsomeTGraph(mxppmc,1);
  mxppmc->SetMarkerStyle(25);
  
    
  hTemp2->SetAxisRange(0.6,1.1,"Y");
  hTemp2->DrawCopy();
  //  TH1D* hMXpp2013_2 = new TH1D("hmxpp2013_2","",1,-10,370);
  //  hMXpp2013_2->SetBinContent(1,hMXpp2013->GetBinContent(1));
  //  if ( !mcOnly )   drawSys(hMXpp2013_2,sysMxpp,kGreen,3001);
  if ( !mcOnly )   drawSys(mxhidata,sysMx,10);
  
  // mxppmc->Draw("p");
  //  if ( !mcOnly )   mxppdata->Draw("p");
  if ( !mcOnly )  {
    hMXpp2013[5]->SetMarkerStyle(20);
    for ( int icent = 1 ; icent<=5 ; icent++) {
      drawSys(hMXpp2013[icent],ppSysMx,kGreen,3001);
      hMXpp2013[icent]->DrawCopy("p same");
    }
  }
  if(drawMC) mxhimc->Draw("p same");
  if ( !mcOnly )  mxhidata->Draw("p same");

  hMXpp2013[5]->SetFillStyle(3001);
  hMXpp2013[5]->SetFillColor(kGreen);
  hMXpp2013[5]->SetLineColor(0);
  hDphiPP2013[2]->SetFillStyle(3001);
  hDphiPP2013[2]->SetFillColor(kGreen);
  hDphiPP2013[2]->SetLineColor(0);
  TH1D *dummyHist = new TH1D("dummyHist","",10,0,1);
  dummyHist->SetFillStyle(1001);
  dummyHist->SetMarkerColor(kRed);
  dummyHist->SetFillColor(90);
  dummyHist->SetLineColor(0);
  
  //// sys bar by energy scale
  /*
  TBox *b1 = new TBox(400-20,0.85 - corrSysMx ,400,  0.85 + corrSysMx);
  b1->SetFillColor(1);
  b1->SetFillStyle(1001);
  if ( !mcOnly )   b1->Draw();
  TBox *b2 = new TBox(400-20+3,0.85 - corrSysMx+ 0.002 ,400-3,  0.85 + corrSysMx- 0.002);
  b2->SetFillColor(0);
  b2->SetFillStyle(1001);
  if ( !mcOnly )   b2->Draw();
  */
  //  drawText("Common uncertainty due to jet ",0.35,0.25,0,15);
  //  drawText("energy scale & photon purity",0.35,0.2,0,15);
  //  drawText("#sqrt{s_{NN}}=2.76 TeV ",0.5,0.85,0,15);
  //  drawText("#int L dt = 150 #mub^{-1}",0.5,0.72,0,15);
  //  drawText("(a)",0.22,0.87,1);
  drawText("#Delta#phi_{J#gamma} > #frac{7}{8}#pi",0.7,0.75,0);
  drawCMSppPbPb(0.1,0.95,12);
  //  drawText("#Delta#phi_{J#gamma} > #frac{7}{8}#pi",0.5,0.38,0);
  // drawText("CMS",0.78,0.88,1);

  TLegend *leg4 =  new TLegend(0.1630303,0.6054839,0.7590909,0.8931183,NULL,"brNDC");
  easyLeg(leg4,"");
  //  if ( !mcOnly )  leg4->AddEntry(mxppdata,"pp Data 231nb^{-1}","p");
  if ( !mcOnly ) leg4->AddEntry(dummyHist,"PbPb Data","fp");
  if ( !mcOnly )  leg4->AddEntry(hMXpp2013[5],"pp Data","fp");
  if ( !mcOnly )  leg4->AddEntry(hDphiPP2013[2],"Smeared pp reference","fp");
  if(drawMC) leg4->AddEntry(mxhimc,"PYTHIA + HYDJET","p");
  //  leg4->AddEntry(mxppmc,"PYTHIA","p");




  //  leg4->AddEntry(hSysTemp,"Sys. Uncertainty","f");                                                                                         
  leg4->Draw();


  gPad->RedrawAxis();
  
  c2->SaveAs("plotPPPbPb/inclusivePt_ppPbPb_xjg_npart.pdf");
  //c2->SaveAs("plotPPPbPb/inclusivePt_ppPbPb_xjg_npart.png");
  //c2->SaveAs("plotPPPbPb/inclusivePt_ppPbPb_xjg_npart.gif");
  //c2->SaveAs("plotPPPbPb/inclusivePt_ppPbPb_xjg_npart.C");
  
  ////////////////////////////// rx 
  TCanvas *c3 = new TCanvas("c3","",350,350);

  TH1D* hTemp3 = new TH1D("htemp2",";N_{part};R_{J#gamma}",100,-20,400);
  hTemp3->SetNdivisions(505);
  handsomeTH1(hTemp3,1);
  handsomeTGraph(rxhidata,2);
  rxhidata->SetMarkerStyle(20);
  handsomeTGraph(rxhimc,1);
  rxhimc->SetMarkerStyle(24);
  handsomeTGraph(rxppdata,1);
  rxppdata->SetMarkerStyle(21);
  handsomeTGraph(rxppmc,1);
  rxppmc->SetMarkerStyle(25);

  hTemp3->SetAxisRange(0.41,1.,"Y");
  hTemp3->Draw();


  // Ratio
  // DivideTG(rxppdata,rxppmc);
  // DivideTG(rxhidata,rxhimc);
  //////////////////////////////////////////////
  //  TH1D* hdphi2013_2 = new TH1D("hdphi2013_2","",1,-10,370);
  //  hdphi2013_2->SetBinContent(1,hRpp2013->GetBinContent(1));
  //  if ( !mcOnly )   drawSys(hdphi2013_2,sysRpp,kGreen,3001);
  if ( !mcOnly )   drawSys(rxhidata,sysR,10);
  //  if ( !mcOnly )   drawSys(rxppdata,sysRpp,10);
  // jumSun(-10,1,400,1);
  if(drawMC) rxhimc->Draw("p");
  //  rxppmc->Draw("p");
  //  if ( !mcOnly )   rxppdata->Draw("p");
  if ( !mcOnly )   {
    hRpp2013[5]->SetMarkerStyle(20);
    for ( int icent =1 ; icent<=5 ; icent++) { 
      drawSys(hRpp2013[icent],ppSysR,kGreen,3001);
      hRpp2013[icent]->Draw("same");
    }
  }
  if ( !mcOnly )   rxhidata->Draw("p same");
  //  drawText(Form("p^{#gamma}_{T} > %d GeV/c",etPho),0.6,0.75,0,15);
  //  drawText(Form("p^{Jet}_{T} > %d GeV/c",etJet),0.6,0.67,0,15);
  //  drawText("CMS",0.78,0.88,1);
  //  drawText("(b)",0.22,0.87,1);
  drawText("#Delta#phi_{J#gamma} > #frac{7}{8}#pi",0.7,0.75,0);
  drawCMSppPbPb(0.1,0.95,12);

  leg4->Draw();
  gPad->RedrawAxis();

  c3->SaveAs("plotPPPbPb/inclusivePt_ppPbPb_r_npart.pdf");
  //c3->SaveAs("plotPPPbPb/inclusivePt_ppPbPb_r_npart.png");
  //c3->SaveAs("plotPPPbPb/inclusivePt_ppPbPb_r_npart.gif");
  //c3->SaveAs("plotPPPbPb/inclusivePt_ppPbPb_r_npart.C");
  
  TCanvas *c4 = new TCanvas("c4","",350,350);

  TH1D* hTemp4 = new TH1D("htemp2",";N_{part};#sigma(#Delta#phi_{J#gamma})",100,-20,400);
  hTemp4->SetNdivisions(505);
  handsomeTH1(hTemp4,1);
  handsomeTGraph(dphihidata,2);
  dphihidata->SetMarkerStyle(20);
  handsomeTGraph(dphihimc,1);
  dphihimc->SetMarkerStyle(24);
  handsomeTGraph(dphippdata,1);
  dphippdata->SetMarkerStyle(21);
  handsomeTGraph(dphippmc,1);
  dphippmc->SetMarkerStyle(25);

  hTemp4->SetAxisRange(0,.5,"Y");
  hTemp4->Draw();
  
                                                                                 
  // TH1D* h2013_3 = new TH1D("hdphi2013_3","",1,-10,370);
  //  h2013_3->SetBinContent(1,hDphiPP2013->GetBinContent(1));

  //  if ( !mcOnly )  drawSys(h2013_3,sysDphipp,kGreen,3001);
  if ( !mcOnly )  drawSys(dphihidata,sysDphi,10);
  if(drawMC) dphihimc->Draw("p same");
  // dphippmc->Draw("p");
  //  if ( !mcOnly )  dphippdata->Draw("p");
  for ( int icent=1 ; icent<=5 ; icent++){
    drawSys(hDphiPP2013[icent], hDphiPPUnc, kGreen,3001);
  }
  
  TH1D* hDphiPP2013Temp = new TH1D("hDphiPP2013Temp","",1,380,400);
  hDphiPP2013Temp->SetBinContent(1,0.27);
  hDphiPP2013[5]->SetMarkerStyle(20);
  for ( int icent=1 ; icent<=5 ; icent++){ 
    hDphiPP2013[icent]->Draw("same");
  }
  if ( !mcOnly )  dphihidata->Draw("p");

  // TLegend *legDphi =  new TLegend(0.32,0.18,0.93,0.7,NULL,"brNDC");
  // easyLeg(legDphi,"");
  // legDphi->SetTextSize(17);
  // //  drawText("|#Delta#phi_{J#gamma}| > #frac{2}{3}#pi",0.5,0.38,0);
  // //  drawText("Fit : #frac{e^{#frac{|#Delta#phi_{J#gamma}|-#pi}{#sigma}}}{#sigma(1-e^{-#pi/#sigma})}",0.5,0.23,0);
  // legDphi->Draw();
  // //  drawText("(a)",0.22,0.87,1);
  // //  drawText("CMS",0.78,0.88,1);


  TH1D* hSysTemp = new TH1D("hSystemp","",1,0,1);
  hSysTemp->SetFillColor(newYellow);
  hSysTemp->SetLineColor(newYellow);

  leg4->Draw();

  drawText(Form("p^{#gamma}_{T} > %d GeV/c     |#eta^{#gamma}| < 1.44",etPho),0.25,0.3,0,15);
  drawText(Form("p^{Jet}_{T} > %d GeV/c    |#eta^{Jet}| < 1.6",etJet),0.25,0.2,0,15);
  drawCMSppPbPb(0.1,0.95,12);

  gPad->RedrawAxis();
  c4->SaveAs("plotPPPbPb/inclusivePt_ppPbPb_dphi_npart.pdf");
  //c4->SaveAs("plotPPPbPb/inclusivePt_ppPbPb_dphi_npart.png");
  //c4->SaveAs("plotPPPbPb/inclusivePt_ppPbPb_dphi_npart.gif");
  //c4->SaveAs("plotPPPbPb/inclusivePt_ppPbPb_dphi_npart.C");

   // // print numbers
   // std::cout << " Summary of Points for PbPb " << std::endl;
   // PrintGraphAndSys(dphihidata,sysDphi);
   // PrintGraphAndSys(mxhidata,sysMx);
   // PrintGraphAndSys(rxhidata,sysR);

   // std::cout << " Summary of Points for PYTHIA + HYDJET " << std::endl;
   // PrintGraph(dphihimc);
   // PrintGraph(mxhimc);
   // PrintGraph(rxhimc);

   //   std::cout << " Summary of Points for pp " << std::endl;
   //  PrintGraphAndSys(dphippdata[5],sysDphipp);
   //  PrintGraphAndSys(mxppdata[5],sysMxpp);
   // PrintGraphAndSys(rxppdata[5],sysRpp);


   
   /*
   TCanvas *c5 = new TCanvas("c5","",500,500);
   hxgj[khidata][0]->SetAxisRange(-.2,2.5,"Y");
   hxgj[khidata][0]->SetNdivisions(505);
   fixedFontHist(hxgj[khidata][0],1,1.35);
   handsomeTH1(hxgj[khidata][0],2);
   hxgj[khidata][0]->GetYaxis()->SetTitleOffset(1.5);
   hxgj[khidata][0]->DrawCopy("");
   handsomeTH1(hxgj[kppdata13][5],4);
   hxgj[kppdata13][5]->Draw("same hist");
   onSun(0,0,2,0);
   */
}

