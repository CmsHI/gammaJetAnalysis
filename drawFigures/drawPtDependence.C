#include "../../HiForestAnalysis/hiForest.h"
#include "../CutAndBinCollection2012.h"
#include <TRandom3.h>
#include <time.h>

void drawPtDependence(TString dirName="nominal", int prodDate=20131021, int jetPtCut=30) {
  TH1::SetDefaultSumw2();

  const int nPtBin = 4;
  double ptBin[nPtBin+1] = {40, 50,60,80,9999}; 
  double ptBinPaDraw[nPtBin+1] = { 40.5 ,49.5, 58.5,  76.5,  123. } ;
  // double AvePtBin[nPtBin+1] = { 45, 54.1479, 67.4204, 99.6956, 9999};
    
  const int nCentBinHI = 2;
  const int centBinHI[nCentBinHI +1] = {-1, 10030, 13099}; 

  TH1D* hxjg[7][10][6]; // [Collision][centrality][pt]
  TH1D* hJetPt[7][10][6]; // [Collision][centrality][pt]
  TH1D* hJetPtIaaBin[7][10][6]; // [Collision][centrality][pt]
  TH1D* hDphi[7][10][6]; // [Collision][centrality][pt]
  TH1D* hEta[7][10][6]; // [Collision][centrality][pt]
  TH1D* meanXjg[7][10];      // [Collision][centrality]
  TH1D* meanJetPt[7][10];      // [Collisi on][centrality]

  TH1D* rjg[7][10];     //  [Collision][centrality]
  for (int icoll=0 ; icoll<6  ; icoll++) {
    for (int icent=1 ; icent<= 10 ; icent++) {
      meanXjg[icoll][icent] = new TH1D( Form("meanXjg_icoll%d_icent%d",icoll,icent), ";p_{T}^{#gamma} (GeV); <X_{J#gamma}>",nPtBin,ptBinPaDraw);
      meanJetPt[icoll][icent] = new TH1D( Form("meanJetPt_icoll%d_icent%d",icoll,icent), ";p_{T}^{#gamma} (GeV); <p_{T}^{Jet}>",nPtBin,ptBinPaDraw);
      rjg[icoll][icent] = new TH1D( Form("rjg_icoll%d_icent%d",icoll,icent), ";p_{T}^{#gamma} (GeV); R_{J#gamma}",nPtBin,ptBinPaDraw);

      for (int ipt=1 ; ipt<=nPtBin ; ipt++) {
	hxjg[icoll][icent][ipt] = NULL;
	hJetPt[icoll][icent][ipt] = NULL;
	hJetPtIaaBin[icoll][icent][ipt] = NULL;
	hDphi[icoll][icent][ipt] = NULL;
	hEta[icoll][icent][ipt] = NULL;
      }

    }
  }

  TFile* histFile[7][6];  // [Collision][pt]                                                                                          
  for (int ipt=1 ; ipt<=nPtBin ; ipt++) {
    for (int icoll=0 ; icoll<6 ; icoll++) {
      TString sampleName = getSampleName( icoll ) ;
      char* fname =  Form("ffFiles/%s/photonTrackCorr_%s_output_photonPtThr%d_to_%d_jetPtThr%d_%d.root",dirName.Data(), sampleName.Data(), (int)ptBin[ipt-1], (int)ptBin[ipt], (int)jetPtCut, prodDate);

      histFile[icoll][ipt] = new TFile(fname) ;
      cout << " Reading file : " << fname << endl;

      if ( histFile[icoll][ipt]->IsZombie()  == false ) {
	cout << " Success." << endl;
	if ( (icoll == kPPDATA) || (icoll == kPPMC) )   {  //  PP
	  int icent = 7 ; 
	  hxjg[icoll][icent][ipt] = (TH1D*)histFile[icoll][ipt]->Get(Form("xjg_icent%d_final", icent)) ;
	  cout << " Getting histogram : " << Form("xjg_icent%d_final", icent) << endl;
	  hJetPt[icoll][icent][ipt] = (TH1D*)histFile[icoll][ipt]->Get(Form("jetPt_icent%d_final", icent)) ;
	  cout << " Getting histogram : " << Form("jetPt_icent%d_final", icent) << endl;
	  hJetPtIaaBin[icoll][icent][ipt] = (TH1D*)histFile[icoll][ipt]->Get(Form("jetPtForIaa_icent%d_final", icent)) ;
	  cout << " Getting histogram : " << Form("jetPtForIaa_icent%d_final", icent) << endl;
	  hDphi[icoll][icent][ipt]  = (TH1D*)histFile[icoll][ipt]->Get(Form("jetDphi_icent%d_final", icent)) ;
	  cout << " Getting histogram : " << Form("jetDphi_icent%d_final", icent) << endl;
	  hEta[icoll][icent][ipt]  = (TH1D*)histFile[icoll][ipt]->Get(Form("etaJg_icent%d_final", icent)) ;
	  cout << " Getting histogram : " << Form("etaJg_icent%d_final", icent) << endl;

	}

	if ( ( icoll == kHIDATA) ||  (icoll == kHIMC) ) { // PbPb
	  for ( int icent = 1; icent <= nCentBinHI ; icent++ ) {
	    hxjg[icoll][icent][ipt] = (TH1D*)histFile[icoll][ipt]->Get(Form("xjg_icent%d_final", centBinHI[icent] )) ;
	    cout << " Getting histogram : " << Form("xjg_icent%d_final", centBinHI[icent] ) << endl;
	    hJetPt[icoll][icent][ipt] = (TH1D*)histFile[icoll][ipt]->Get(Form("jetPt_icent%d_final", centBinHI[icent] ) );
	    cout << " Getting histogram : " << Form("jetPt_icent%d_final", centBinHI[icent] ) << endl;
	    hJetPtIaaBin[icoll][icent][ipt] = (TH1D*)histFile[icoll][ipt]->Get(Form("jetPtForIaa_icent%d_final", centBinHI[icent] ) );
	    cout << " Getting histogram : " << Form("jetPtForIaa_icent%d_final", centBinHI[icent] ) << endl;
	    hDphi[icoll][icent][ipt]  = (TH1D*)histFile[icoll][ipt]->Get(Form("jetDphi_icent%d_final", centBinHI[icent] ) ) ;
	    cout << " Getting histogram : " << Form("jetDphi_icent%d_final", centBinHI[icent])<< endl;
	    hEta[icoll][icent][ipt]  = (TH1D*)histFile[icoll][ipt]->Get(Form("etaJg_icent%d_final", centBinHI[icent] ) ) ;
	    cout << " Getting histogram : " << Form("etaJg_icent%d_final", centBinHI[icent])<< endl;
	  }
	}
      }
      else  
	cout << " no such file " << endl;
    }
  }

  for (int ipt=1 ; ipt<=nPtBin ; ipt++) {
    for (int icoll=0 ; icoll<6 ; icoll++) {
      for (int icent=1 ; icent<= 10 ; icent++ ) { 
	if ( hxjg[icoll][icent][ipt] == NULL ) 
	  continue;   // emtpy histogram 
	double rVal, rErr;

	rVal = hxjg[icoll][icent][ipt]->IntegralAndError(1, hxjg[icoll][icent][ipt]->GetNbinsX(), rErr, "width");
	printf("icoll: %d icent: %d ipt: %d rval: %lf\n",icoll, icent, ipt, rVal);
	rjg[icoll][icent]->SetBinContent( ipt, rVal );
	rjg[icoll][icent]->SetBinError  ( ipt, rErr );

	meanXjg[icoll][icent]->SetBinContent( ipt, hxjg[icoll][icent][ipt]->GetMean() );
	meanXjg[icoll][icent]->SetBinError  ( ipt, hxjg[icoll][icent][ipt]->GetMeanError() );

	meanJetPt[icoll][icent]->SetBinContent( ipt, hJetPt[icoll][icent][ipt]->GetMean() );
	meanJetPt[icoll][icent]->SetBinError  ( ipt, hJetPt[icoll][icent][ipt]->GetMeanError() );


      }
    }
  }

  TCanvas* c1 = new TCanvas("c1","",1200,350);
  makeMultiPanelCanvas(c1,nPtBin,1,0.0,0.0,0.2,0.15,0.02);
  for ( int ipt = 1 ; ipt<=nPtBin  ; ipt++) {

    c1->cd(ipt);

    // draw pp
    handsomeTH1(hDphi[kPPDATA][7][ipt], 1);
    hDphi[kPPDATA][7][ipt]->SetXTitle("#Delta#phi_{J#gamma}");
    hDphi[kPPDATA][7][ipt]->SetYTitle("Normalized entries");
    hDphi[kPPDATA][7][ipt]->SetMarkerStyle(24);
    hDphi[kPPDATA][7][ipt]->Scale(1./hDphi[kPPDATA][7][ipt]->Integral("width"));       
    hDphi[kPPDATA][7][ipt]->SetAxisRange(0,3.141592,"X");
    hDphi[kPPDATA][7][ipt]->SetAxisRange(0.01,30,"Y");
    hDphi[kPPDATA][7][ipt]->Draw();
    // draw pbpb 
    for ( int icent = 1; icent <= nCentBinHI ; icent++ ) {
      handsomeTH1(hDphi[kHIDATA][icent][ipt],kRed);
      if ( icent == 2 ) hDphi[kHIDATA][icent][ipt]->SetMarkerStyle(24);
      hDphi[kHIDATA][icent][ipt]->Scale(1./hDphi[kHIDATA][icent][ipt]->Integral("width"));
      hDphi[kHIDATA][icent][ipt]->Draw("same");

    }

    gPad->SetLogy();
    //    onSun(30,0,200,0);
  }
  c1->cd(1);
  TLegend *ly = new TLegend(0.4913112,0.6561548,0.9997611,0.9431145,NULL,"brNDC");
  easyLeg(ly,"2.76TeV");
  ly->AddEntry(hDphi[kPPDATA][7][1],"pp ","p");
  ly->AddEntry(hDphi[kHIDATA][2][1],"PbPb 0-30%","p");

  TLegend *l1 = new TLegend(0.1995968,0.7097458,0.7076613,0.9237288,NULL,"brNDC");
  easyLeg(l1,"2.76TeV");
  l1->AddEntry(hDphi[kPPDATA][7][1],"pp ","p");
  l1->AddEntry(hDphi[kHIDATA][2][1],"PbPb 30-100%","p");
  l1->AddEntry(hDphi[kHIDATA][1][1],"PbPb 0-30%","p");
  l1->Draw();

  // c1->SaveAs("figures/pT_dependence_dphi_pp_pbpb_figure1.pdf");
  // c1->SaveAs("figures/pT_dependence_dphi_pp_pbpb_figure1.gif");





  TH1D* hTempPt = new TH1D("hTemp",";p_{T}^{#gamma} (GeV);",200,10,300);

  TCanvas* c2 = new TCanvas("c2","",1200,350);
  makeMultiPanelCanvas(c2,nPtBin,1,0.0,0.0,0.2,0.15,0.02);
  for ( int ipt = 1 ; ipt<=nPtBin  ; ipt++) {

    c2->cd(ipt);

    // draw pp
    hTempPt->SetXTitle("p_{T}^{Jet} (GeV)");
    hTempPt->SetYTitle("#frac{dN}{dp_{T}} #frac{1}{N}");
    hTempPt->SetAxisRange(10,150,"X");
    //        hTempPt->SetAxisRange(0,0.025,"Y");
    hTempPt->SetAxisRange(0,0.08,"Y");
    handsomeTH1(hTempPt,0);
    hTempPt->DrawCopy();
	
    //    hJetPt[kPPDATA][7][ipt]->Scale(1./rjg[kPPDATA][7]->GetBinContent(ipt)); // rjg normalization                      
		

    handsomeTH1(hJetPt[kPPDATA][7][ipt], 1);
    hJetPt[kPPDATA][7][ipt]->SetMarkerStyle(24);
    hJetPt[kPPDATA][7][ipt]->Draw("same");

    // draw pbpb 
    for ( int icent = 1; icent <= nCentBinHI ; icent++ ) {
      handsomeTH1(hJetPt[kHIDATA][icent][ipt],kRed);
      //      hJetPt[kHIDATA][icent][ipt]->Scale(1./rjg[kHIDATA][icent]->GetBinContent(ipt));  // rjg normalization
      //      hJetPt[kHIDATA][icent][ipt]->Rebin(10);
      //     hJetPt[kHIDATA][icent][ipt]->Scale(1/10.);
      if ( icent == 2 ) hJetPt[kHIDATA][icent][ipt]->SetMarkerStyle(24);
      if ( icent == 2 ) hJetPt[kHIDATA][icent][ipt]->Draw("same");
      if ( icent == 1 ) hJetPt[kHIDATA][icent][ipt]->Draw("same");
    }
       
    double dx1=0.15;
    if ( ipt == nPtBin ) 
      drawText(Form("p_{T}^{#gamma} > %dGeV, ", (int)ptBin[ipt-1]), 0.12+dx1+0.25,0.85,1,15);//yeonju 130823
    else
      drawText(Form("%dGeV < p_{T}^{#gamma} < %dGeV, ", (int)ptBin[ipt-1], (int)ptBin[ipt]), 0.12+dx1,0.85,1,15);//yeonju 130823

    onSun(30,0,200,0);
  }
  c2->cd(1);
  l1->Draw();  

  c2->SaveAs("figures/pT_dependence_jetPt_pp_pbpb_figure1.pdf");
  c2->SaveAs("figures/pT_dependence_jetPt_pp_pbpb_figure1.gif");

  TLegend *l2 = new TLegend(0.2116935,0.7012712,0.6149194,0.904661,NULL,"brNDC");
  easyLeg(l2,"2.76TeV");
  l2->AddEntry(hDphi[kPPDATA][7][1],"pp ","p");
  l2->AddEntry(hDphi[kHIDATA][1][1],"PbPb 30-100%","p");
  l2->AddEntry(hDphi[kHIDATA][2][1],"PbPb 0-30%","p");

  TCanvas* c21 = new TCanvas("c21","",500,500);
  handsomeTH1(meanJetPt[kPPDATA][7], 1);
  meanJetPt[kPPDATA][7]->SetYTitle("<p_{T}^{Jet}>  (>30GeV)");
  meanJetPt[kPPDATA][7]->SetMarkerStyle(24);
  //  meanJetPt[kPPDATA][7]->SetAxisRange(-2,2,"X");
  meanJetPt[kPPDATA][7]->SetAxisRange(40,90,"Y");
  meanJetPt[kPPDATA][7]->Draw();

  for ( int icent = 1; icent <= nCentBinHI ; icent++ ) {
    handsomeTH1(meanJetPt[kHIDATA][icent],kRed);
    if ( icent == 2 ) meanJetPt[kHIDATA][icent]->SetMarkerStyle(24);
    //if ( icent == 2 ) meanJetPt[kHIDATA][icent]->Draw("same");
    meanJetPt[kHIDATA][icent]->Draw("same");
  }
  l1 ->Draw();
  c21->SaveAs("figures/pT_dependence_jetPt_pp_pbpb_figure2.pdf");
  c21->SaveAs("figures/pT_dependence_jetPt_pp_pbpb_figure2.gif");

  TCanvas* c3 = new TCanvas("c3","",1200,350);
  makeMultiPanelCanvas(c3,nPtBin,1,0.0,0.0,0.2,0.15,0.02);
  for ( int ipt = 1 ; ipt<=nPtBin  ; ipt++) {

    c3->cd(ipt);

    // draw pp
    handsomeTH1(hxjg[kPPDATA][7][ipt], 1);
    hxjg[kPPDATA][7][ipt]->SetXTitle("x_{J#gamma}");
    hxjg[kPPDATA][7][ipt]->SetYTitle("#frac{dN}{dp_{T}} #frac{1}{N}");
    hxjg[kPPDATA][7][ipt]->SetMarkerStyle(24);

    //    hxjg[kPPDATA][7][ipt]->Scale(1./rjg[kPPDATA][7]->GetBinContent(ipt)); // rjg normalization 

    //    hxjg[kPPDATA][7][ipt]->Rebin(10);
    //    hxjg[kPPDATA][7][ipt]->Scale(1./10);
    hxjg[kPPDATA][7][ipt]->SetAxisRange(0,2.5,"X");
    hxjg[kPPDATA][7][ipt]->SetAxisRange(0,3,"Y");
    hxjg[kPPDATA][7][ipt]->Draw();

    // draw pbpb
    for ( int icent = 1; icent <= nCentBinHI ; icent++ ) {
      handsomeTH1(hxjg[kHIDATA][icent][ipt],kRed);
      //      hxjg[kHIDATA][icent][ipt]->Scale(1./rjg[kHIDATA][icent]->GetBinContent(ipt)); // rjg normalization    
      //      hxjg[kHIDATA][icent][ipt]->Rebin(10);
      //   hxjg[kHIDATA][icent][ipt]->Scale(1./10);
      if ( icent == 2 ) hxjg[kHIDATA][icent][ipt]->SetMarkerStyle(24);
      if ( icent == 2 ) hxjg[kHIDATA][icent][ipt]->Draw("same");
      if ( icent == 1 ) hxjg[kHIDATA][icent][ipt]->Draw("same");

    }
    double dx1=0.15;
    if ( ipt == nPtBin ) 
      drawText(Form("p_{T}^{#gamma} > %dGeV, ", (int)ptBin[ipt-1]), 0.12+dx1+0.25,0.85,1,15);//yeonju 130823
    else
      drawText(Form("%dGeV < p_{T}^{#gamma} < %dGeV, ", (int)ptBin[ipt-1], (int)ptBin[ipt]), 0.12+dx1,0.85,1,15);//yeonju 130823

 
    onSun(30,0,200,0);
  }
  c3->cd(1);
  l1->Draw();  

  c3->SaveAs("figures/pT_dependence_xjg_pp_pbpb_figure1.pdf");
  c3->SaveAs("figures/pT_dependence_xjg_pp_pbpb_figure1.gif");

  TCanvas* c31 = new TCanvas("c31","",500,500);
  handsomeTH1(meanXjg[kPPDATA][7], 1);
  meanXjg[kPPDATA][7]->SetYTitle("<x_{J#gamma}>  (>30GeV)");
  meanXjg[kPPDATA][7]->SetMarkerStyle(24);
  //  meanXjg[kPPDATA][7]->SetAxisRange(-2,2,"X");
  meanXjg[kPPDATA][7]->SetAxisRange(0.6,1.2,"Y");
  meanXjg[kPPDATA][7]->Draw();

  for ( int icent = 1; icent <= nCentBinHI ; icent++ ) {
    handsomeTH1(meanXjg[kHIDATA][icent],kRed);
    if ( icent == 2 ) meanXjg[kHIDATA][icent]->SetMarkerStyle(24);
    //if ( icent != 2 ) meanXjg[kHIDATA][icent]->Draw("same");
    meanXjg[kHIDATA][icent]->Draw("same");
  }
  l1->Draw();
  c31->SaveAs("figures/pT_dependence_xjg_pp_pbpb_figure2.pdf");
  c31->SaveAs("figures/pT_dependence_xjg_pp_pbpb_figure2.gif");
    

  TCanvas* c_rjg = new TCanvas("c_rjg","",500,500);
  handsomeTH1(rjg[kPPDATA][7], 1);
  rjg[kPPDATA][7]->SetYTitle("r_{J#gamma}");
  rjg[kPPDATA][7]->SetMarkerStyle(24);
  //  rjg[kPPDATA][7]->SetAxisRange(-2,2,"X");
  rjg[kPPDATA][7]->SetAxisRange(0.0,1.1,"Y");
  rjg[kPPDATA][7]->Draw();

  for ( int icent = 1; icent <= nCentBinHI ; icent++ ) {
    handsomeTH1(rjg[kHIDATA][icent],kRed);
    if ( icent == 2 ) rjg[kHIDATA][icent]->SetMarkerStyle(24);
    //        if ( icent != 2 ) rjg[kHIDATA][icent]->Draw("same");
    rjg[kHIDATA][icent]->Draw("same");
  }
  l1->Draw();
     c_rjg->SaveAs("figures/pT_dependence_rjg_pp_pbpb_figure1.pdf");
     c_rjg->SaveAs("figures/pT_dependence_rjg_pp_pbpb_figure1.gif");


    
  // Save the final root histogram files
  TFile * fResults =  new TFile("resultHistograms.root","update");
  //TFile * fResults =  new TFile("resultHistograms_photonEnergy_ScaledBy-0.015.root","update");
    
  // pp 
    
  meanJetPt[kPPDATA][7]->SetName(Form("meanJetPt_pp"));
  meanJetPt[kPPDATA][7]->Write();

  meanXjg[kPPDATA][7]->SetName(Form("meanXjg_pp"));
  meanXjg[kPPDATA][7]->Write();
    
  rjg[kPPDATA][7]->SetName(Form("meanRjg_pp"));
  rjg[kPPDATA][7]->Write();
    
  for ( int ipt = 1 ; ipt<=nPtBin  ; ipt++) {
    hJetPt[kPPDATA][7][ipt]->SetName(Form("dNdJetPt_pp_ptBin%d",ipt));
    hJetPt[kPPDATA][7][ipt]->Write();
    hJetPtIaaBin[kPPDATA][7][ipt]->SetName(Form("dNdJetPt_IaaBin_pp_ptBin%d",ipt));
    hJetPtIaaBin[kPPDATA][7][ipt]->Write();
    
    hxjg[kPPDATA][7][ipt]->SetName(Form("dNdXjg_pp_ptBin%d",ipt));
    hxjg[kPPDATA][7][ipt]->Write();
    
    hDphi[kPPDATA][7][ipt]->SetName(Form("dNdphi_pp_ptBin%d",ipt));
    hDphi[kPPDATA][7][ipt]->Write();	
    
    
  }
  

  // PbPb 
    
  for ( int icent = 1; icent <= nCentBinHI ; icent++ ) {
    for ( int ipt = 1 ; ipt<=nPtBin  ; ipt++) {
      hJetPt[kHIDATA][icent][ipt]->SetName(Form("dNdJetPt_pbpb_centralityBin%d_ptBin%d",icent,ipt));
      hJetPt[kHIDATA][icent][ipt]->Write();	

      hDphi[kHIDATA][icent][ipt]->SetName(Form("dNdphi_pbpb_centralityBin%d_ptBin%d",icent,ipt));
      hDphi[kHIDATA][icent][ipt]->Write();	

      hJetPtIaaBin[kHIDATA][icent][ipt]->SetName(Form("dNdJetPt_IaaBin_pbpb_centralityBin%d_ptBin%d",icent,ipt));
      hJetPtIaaBin[kHIDATA][icent][ipt]->Write();	
      
      hxjg[kHIDATA][icent][ipt]->SetName(Form("dNdXjg_pbpb_centralityBin%d_ptBin%d",icent,ipt));
      hxjg[kHIDATA][icent][ipt]->Write();

    }
    
    meanJetPt[kHIDATA][icent]->SetName(Form("meanJetPt_pbpb_centralityBin%d",icent));
    meanJetPt[kHIDATA][icent]->Write();
    meanXjg[kHIDATA][icent]->SetName(Form("meanXjg_pbpb_centralityBin%d",icent));
    meanXjg[kHIDATA][icent]->Write();
      
    rjg[kHIDATA][icent]->SetName(Form("meanRjg_pbpb_centralityBin%d",icent));
    rjg[kHIDATA][icent]->Write();
  }
    
  fResults->Close();
    
    
}

