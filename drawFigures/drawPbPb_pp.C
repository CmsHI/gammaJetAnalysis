#include "../../HiForestAnalysis/hiForest.h"
#include "../CutAndBinCollection2012.h"
#include <TRandom3.h>
#include <time.h>

void drawPbPb_pp( bool saveFigures=true, int prodDate=20131021, int jetPtCut=30) {

    const int nPtBin = 4;
    double ptBin[nPtBin+1] = {40, 50,60,80,9999}; 
    double ptBinPaDraw[nPtBin+1] = { 40.5 ,49.5, 58.5,  76.5,  123. } ;
    // double AvePtBin[nPtBin+1] = { 45, 54.1479, 67.4204, 99.6956, 9999};
    
    const int nCentBinHI = 2;
    const int centBinHI[nCentBinHI +1] = {-1, 10030, 13099}; 

    TH1D* hxjg[7][10][6]; // [Collision][centrality][pt]
    TH1D* hJetPt[7][10][6]; // [Collision][centrality][pt]
    TH1D* hGenJetPt[7][10][6]; // [Collision][centrality][pt]
    TH1D* hIaa[7][10][6]; // [Collision][centrality][pt]
    TH1D* hDphi[7][10][6]; // [Collision][centrality][pt]
    TH1D* hEta[7][10][6]; // [Collision][centrality][pt]
    TH1D* meanXjg[7][10];      // [Collision][centrality]
    TH1D* meanJetPt[7][10];      // [Collisi on][centrality]
    TH1D* meanGenJetPt[7][10];      // [Collisi on][centrality]

    TH1D* rjg[7][5];     //  [Collision][centrality]
    TH1D* rGenjg[7][5];     //  [Collision][centrality]
    for (int icoll=0 ; icoll<4  ; icoll++) {   // only pp and PbPb
      for (int icent=1 ; icent<= 10 ; icent++) {
	meanXjg[icoll][icent] = new TH1D( Form("meanXjg_icoll%d_icent%d",icoll,icent), ";p_{T}^{#gamma} (GeV); <X_{J#gamma}>",nPtBin,ptBinPaDraw);
	meanJetPt[icoll][icent] = new TH1D( Form("meanJetPt_icoll%d_icent%d",icoll,icent), ";p_{T}^{#gamma} (GeV); <p_{T}^{Jet}>",nPtBin,ptBinPaDraw);
	meanGenJetPt[icoll][icent] = new TH1D( Form("meanGenJetPt_icoll%d_icent%d",icoll,icent), ";p_{T}^{#gamma} (GeV); <p_{T}^{Jet}>",nPtBin,ptBinPaDraw);
	rjg[icoll][icent] = new TH1D( Form("rjg_icoll%d_icent%d",icoll,icent), ";p_{T}^{#gamma} (GeV); R_{J#gamma}",nPtBin,ptBinPaDraw);
	rGenjg[icoll][icent] = new TH1D( Form("rGenjg_icoll%d_icent%d",icoll,icent), ";p_{T}^{#gamma} (GeV); R_{J#gamma}",nPtBin,ptBinPaDraw);
	
	
	for (int ipt=1 ; ipt<=nPtBin ; ipt++) {
	  hxjg[icoll][icent][ipt] = NULL;
	  hJetPt[icoll][icent][ipt] = NULL;
	  hGenJetPt[icoll][icent][ipt] = NULL;
	  hIaa[icoll][icent][ipt] = NULL;
	  hDphi[icoll][icent][ipt] = NULL;
	  hEta[icoll][icent][ipt] = NULL;
	}
	
      }
    }
    
    TFile* histFile[7][6];  // [Collision][pt]                                                                                          
    for (int ipt=1 ; ipt<=nPtBin ; ipt++) {
      for (int icoll=0 ; icoll<4 ; icoll++) {
	TString sampleName = getSampleName( icoll ) ;
            char* fname =  Form("ffFiles/photonTrackCorr_%s_output_photonPtThr%d_to_%d_jetPtThr%d_%d.root",sampleName.Data(), (int)ptBin[ipt-1], (int)ptBin[ipt], (int)jetPtCut, prodDate);
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
		    hDphi[icoll][icent][ipt]  = (TH1D*)histFile[icoll][ipt]->Get(Form("jetDphi_icent%d_final", icent)) ;
                    cout << " Getting histogram : " << Form("jetDphi_icent%d_final", icent) << endl;
                    hEta[icoll][icent][ipt]  = (TH1D*)histFile[icoll][ipt]->Get(Form("etaJg_icent%d_final", icent)) ;
                    cout << " Getting histogram : " << Form("etaJg_icent%d_final", icent) << endl;

		    if ( icoll%2 == 1)  {  // if it is MC 
		      hGenJetPt[icoll][icent][ipt] = (TH1D*)histFile[icoll][ipt]->Get(Form("genJetPt_icent%d_final", icent)) ;
		      cout << " Getting histogram : " << Form("genJetPt_icent%d_final", icent) << endl;
		    }
                }

                if ( ( icoll == kHIDATA) ||  (icoll == kHIMC) ) { // PbPb
                    for ( int icent = 1; icent <= nCentBinHI ; icent++ ) {
                        hxjg[icoll][icent][ipt] = (TH1D*)histFile[icoll][ipt]->Get(Form("xjg_icent%d_final", centBinHI[icent] )) ;
                        cout << " Getting histogram : " << Form("xjg_icent%d_final", centBinHI[icent] ) << endl;
                        hJetPt[icoll][icent][ipt] = (TH1D*)histFile[icoll][ipt]->Get(Form("jetPt_icent%d_final", centBinHI[icent] ) );
                        cout << " Getting histogram : " << Form("jetPt_icent%d_final", centBinHI[icent] ) << endl;
                        hDphi[icoll][icent][ipt]  = (TH1D*)histFile[icoll][ipt]->Get(Form("jetDphi_icent%d_final", centBinHI[icent] ) ) ;
                        cout << " Getting histogram : " << Form("jetDphi_icent%d_final", centBinHI[icent])<< endl;
                        hEta[icoll][icent][ipt]  = (TH1D*)histFile[icoll][ipt]->Get(Form("etaJg_icent%d_final", centBinHI[icent] ) ) ;
                        cout << " Getting histogram : " << Form("etaJg_icent%d_final", centBinHI[icent])<< endl;
			if ( icoll%2 == 1)  {  // if it is MC           
			  hGenJetPt[icoll][icent][ipt] = (TH1D*)histFile[icoll][ipt]->Get(Form("genJetPt_icent%d_final",centBinHI[icent]));
			  cout << " Getting histogram : " << Form("genJetPt_icent%d_final", centBinHI[icent] ) << endl;
			}
                    }
                }
            }
            else  
                cout << " no such file " << endl;
        }
    }

    for (int ipt=1 ; ipt<=nPtBin ; ipt++) {
        for (int icoll=0 ; icoll<4 ; icoll++) {
            for (int icent=1 ; icent<= 10 ; icent++ ) { 
                if ( hxjg[icoll][icent][ipt] == NULL ) 
                    continue;   // emtpy histogram 
                double rVal, rErr;

                rVal = hxjg[icoll][icent][ipt]->IntegralAndError(1, hxjg[icoll][icent][ipt]->GetNbinsX(), rErr, "width");
                rjg[icoll][icent]->SetBinContent( ipt, rVal );
                rjg[icoll][icent]->SetBinError  ( ipt, rErr );

                meanXjg[icoll][icent]->SetBinContent( ipt, hxjg[icoll][icent][ipt]->GetMean() );
                meanXjg[icoll][icent]->SetBinError  ( ipt, hxjg[icoll][icent][ipt]->GetMeanError() );

                meanJetPt[icoll][icent]->SetBinContent( ipt, hJetPt[icoll][icent][ipt]->GetMean() );
                meanJetPt[icoll][icent]->SetBinError  ( ipt, hJetPt[icoll][icent][ipt]->GetMeanError() );

		if ( icoll%2 == 1)  {  // if it is MC                                                                                
		  meanGenJetPt[icoll][icent]->SetBinContent( ipt, hGenJetPt[icoll][icent][ipt]->GetMean() );
		  meanGenJetPt[icoll][icent]->SetBinError  ( ipt, hGenJetPt[icoll][icent][ipt]->GetMeanError() );
		  rVal = hGenJetPt[icoll][icent][ipt]->IntegralAndError(1, hGenJetPt[icoll][icent][ipt]->GetNbinsX(), rErr, "width");
		  rGenjg[icoll][icent]->SetBinContent( ipt, rVal );
		  rGenjg[icoll][icent]->SetBinError  ( ipt, rErr );
		
		}
		
            }
        }
    }

    TCanvas* c1 = new TCanvas("c1_dphi","",1200,900);
    makeMultiPanelCanvas(c1,nPtBin,3,0.0,0.0,0.2,0.15,0.02);
    
    TH1D* htempdphi = (TH1D*)hDphi[kPPDATA][7][1]->Clone("htempdphi");
    htempdphi->Reset();
    handsomeTH1(htempdphi, 0);
    htempdphi->SetXTitle("#Delta#phi_{J#gamma}");
    htempdphi->SetYTitle("Normalized entries");
    htempdphi->SetAxisRange(0,3.141592,"X");
    htempdphi->SetAxisRange(0.01,30,"Y");

    for ( int ipt = 1 ; ipt<=nPtBin  ; ipt++) {

        c1->cd(ipt);
	htempdphi->Draw();
        // draw pp
        handsomeTH1(hDphi[kPPDATA][7][ipt], 1);
        hDphi[kPPDATA][7][ipt]->SetMarkerStyle(24);
        hDphi[kPPDATA][7][ipt]->Scale(1./hDphi[kPPDATA][7][ipt]->Integral("width"));       
        hDphi[kPPDATA][7][ipt]->SetAxisRange(0,3.141592,"X");
        hDphi[kPPDATA][7][ipt]->SetAxisRange(0.01,30,"Y");
        hDphi[kPPDATA][7][ipt]->Draw("same");
        // draw pp MC
	hDphi[kPPMC][7][ipt]->Scale(1./hDphi[kPPMC][7][ipt]->Integral("width"));
	hDphi[kPPMC][7][ipt]->Draw("same hist");
        gPad->SetLogy();
	
	double dx1=0.15;
	if ( ipt == nPtBin ) 
	  drawText(Form("p_{T}^{#gamma} > %dGeV, ", (int)ptBin[ipt-1]), 0.12+dx1+0.25,0.85,1,15);//yeonju 130823
        else
	  drawText(Form("%dGeV < p_{T}^{#gamma} < %dGeV, ", (int)ptBin[ipt-1], (int)ptBin[ipt]), 0.12+dx1,0.85,1,15);//yeonju 130823       
	
	if ( ipt == 1 )  {
	  TLegend *l0 = new TLegend(0.2885025,0.5019232,0.7969524,0.7908838,NULL,"brNDC");
	  easyLeg(l0,"pp");
	  l0->AddEntry(hDphi[kPPDATA][7][ipt],"Data","p");
	  l0->AddEntry(hDphi[kPPMC][7][ipt],"PYTHIA","l");
	  l0->Draw();
	}	  
	
        for ( int icent = 1; icent <= nCentBinHI ; icent++ ) {
	  c1->cd(ipt+nPtBin * icent);
	  htempdphi->Draw();
	  handsomeTH1(hDphi[kHIDATA][icent][ipt],kRed);
	  if ( icent == 2 ) hDphi[kHIDATA][icent][ipt]->SetMarkerStyle(24);
	  hDphi[kHIDATA][icent][ipt]->Scale(1./hDphi[kHIDATA][icent][ipt]->Integral("width"));
	  hDphi[kHIDATA][icent][ipt]->Draw("same");

	  hDphi[kHIMC][icent][ipt]->Scale(1./hDphi[kHIMC][icent][ipt]->Integral("width"));
	  hDphi[kHIMC][icent][ipt]->Draw("same hist");
	  gPad->SetLogy();
	  if ( ipt == 1 )  {
	    TLegend *l0 = new TLegend(0.2885025,0.5019232,0.7969524,0.7908838,NULL,"brNDC");
	    if ( icent == 1 ) easyLeg(l0,"PbPb 0-30%");	    
	    else if ( icent == 2 ) easyLeg(l0,"PbPb 30-100%");
	    l0->AddEntry(hDphi[kHIDATA][icent][ipt],"Data","p");
	    l0->AddEntry(hDphi[kHIMC][icent][ipt],"PYTHIA+HYDJET","l");
	    l0->Draw();
	  }	  
	  
        }
	
        //    onSun(30,0,200,0);
    }
    c1->cd(1);

    
    TH1D* hTempPt = new TH1D("hTemp",";p_{T}^{Jet} (GeV);#frac{dN}{dp_{T}} #frac{1}{N}",200,10,150);
    hTempPt->SetAxisRange(30,150,"X");
    hTempPt->SetAxisRange(0,0.05,"Y");
    hTempPt->GetYaxis()->SetTitleOffset(1.35);
    handsomeTH1(hTempPt,0);
    
    c1->SaveAs("figures/drawPbPb_pp_hDphi.pdf");
    c1->SaveAs("figures/drawPbPb_pp_hDphi.gif");

    TCanvas* c2 = new TCanvas("c2_jetPt","",1200,350);
    makeMultiPanelCanvas(c2,nPtBin,1,0.0,0.0,0.2,0.15,0.02);
    for ( int ipt = 1 ; ipt<=nPtBin  ; ipt++) {
      
      c2->cd(ipt);
      
      // draw pp
      hTempPt->DrawCopy();
      hJetPt[kPPMC][7][ipt]->Scale(1./rjg[kPPMC][7]->GetBinContent(ipt)); // rjg normalization                      
      handsomeTH1(hJetPt[kPPMC][7][ipt], 1);
      hJetPt[kPPMC][7][ipt]->SetMarkerStyle(24);
      hJetPt[kPPMC][7][ipt]->Draw("same hist");
      // draw pbpb 
      for ( int icent = 1; icent <= nCentBinHI ; icent++ ) {
	handsomeTH1(hJetPt[kHIMC][icent][ipt],kRed);
	hJetPt[kHIMC][icent][ipt]->Scale(1./rjg[kHIMC][icent]->GetBinContent(ipt));  // rjg normalization
	if ( icent == 2 ) hJetPt[kHIMC][icent][ipt]->SetMarkerStyle(24);

	if ( icent == 2 ) hJetPt[kHIMC][icent][ipt]->Draw("same hist");
	if ( icent == 1 ) hJetPt[kHIMC][icent][ipt]->Draw("same");
	
      }
       
      double dx1=0.15;
      if ( ipt == nPtBin ) 
	drawText(Form("p_{T}^{#gamma} < %dGeV, ", (int)ptBin[ipt-1]), 0.15+dx1+0.25,0.85,1,15);//yeonju 130823
      else
	drawText(Form("%dGeV < p_{T}^{#gamma} < %dGeV, ", (int)ptBin[ipt-1], (int)ptBin[ipt]), 0.15+dx1,0.85,1,15);//yeonju 130823
      
      if ( ipt == nPtBin )  {
	TLegend *l0 = new TLegend(0.1542202,0.4928762,0.9769094,0.7803873,NULL,"brNDC");
	easyLeg(l0,"MC  2.76TeV");
	l0->AddEntry(hJetPt[kPPMC][7][ipt],"PYTHIA   pp ","l");
	l0->AddEntry(hJetPt[kHIMC][2][ipt],"PYTHIA+HYDJET 30-100%","l");
	l0->AddEntry(hJetPt[kHIMC][1][ipt],"PYTHIA+HYDJET 0-30%","p");
	l0->Draw();
      }
      
      onSun(30,0,200,0);
    }

    c2->SaveAs("figures/drawPbPb_pp_hJetPt.pdf");
    c2->SaveAs("figures/drawPbPb_pp_hJetPt.gif");


    TCanvas* c3 = new TCanvas("c3_GenjetPt","",1200,350);
    makeMultiPanelCanvas(c3,nPtBin,1,0.0,0.0,0.2,0.15,0.02);
    for ( int ipt = 1 ; ipt<=nPtBin  ; ipt++) {
      
      c3->cd(ipt);
      
      // draw pp
      hTempPt->DrawCopy();
      hGenJetPt[kPPMC][7][ipt]->Scale(1./rGenjg[kPPMC][7]->GetBinContent(ipt)); // rjg normalization                      
      handsomeTH1(hGenJetPt[kPPMC][7][ipt], 1);
      hGenJetPt[kPPMC][7][ipt]->SetMarkerStyle(24);
      hGenJetPt[kPPMC][7][ipt]->Draw("same hist");
      // draw pbpb 
      for ( int icent = 1; icent <= nCentBinHI ; icent++ ) {
	handsomeTH1(hGenJetPt[kHIMC][icent][ipt],kRed);
	hGenJetPt[kHIMC][icent][ipt]->Scale(1./rGenjg[kHIMC][icent]->GetBinContent(ipt));  // rjg normalization
	if ( icent == 2 ) hGenJetPt[kHIMC][icent][ipt]->SetMarkerStyle(24);

	if ( icent == 2 ) hGenJetPt[kHIMC][icent][ipt]->Draw("same hist");
	if ( icent == 1 ) hGenJetPt[kHIMC][icent][ipt]->Draw("same");
	
      }
       
      double dx1=0.15;
      if ( ipt == nPtBin ) 
	drawText(Form("p_{T}^{#gamma} < %dGeV, ", (int)ptBin[ipt-1]), 0.15+dx1+0.25,0.85,1,15);//yeonju 130823
      else
	drawText(Form("%dGeV < p_{T}^{#gamma} < %dGeV, ", (int)ptBin[ipt-1], (int)ptBin[ipt]), 0.15+dx1,0.85,1,15);//yeonju 130823
      
      if ( ipt == nPtBin )  {
	TLegend *l0 = new TLegend(0.1542202,0.4928762,0.9769094,0.7803873,NULL,"brNDC");
	easyLeg(l0,"Gen Jet Used");
	l0->AddEntry(hGenJetPt[kPPMC][7][ipt],"PYTHIA   pp ","l");
	l0->AddEntry(hGenJetPt[kHIMC][2][ipt],"PYTHIA+HYDJET 30-100%","l");
	l0->AddEntry(hGenJetPt[kHIMC][1][ipt],"PYTHIA+HYDJET 0-30%","p");
	l0->Draw();
      }
      
      onSun(30,0,200,0);
    }
    
    c3->SaveAs("figures/drawPbPb_pp_hGenJetPt.pdf");
    c3->SaveAs("figures/drawPbPb_pp_hGenJetPt.gif");
    
      
    TCanvas* c101 = new TCanvas("c101_xjg","",1200,350);
    makeMultiPanelCanvas(c101,nPtBin,1,0.0,0.0,0.2,0.15,0.02);
    TH1D* hTempXjg = new TH1D("htempxjg",";x_{J#gamma};#frac{dN}{dp_{T}} #frac{1}{N}",200,0,2);
    hTempXjg->SetAxisRange(0,2,"X");
    hTempXjg->SetAxisRange(0,3,"Y");
    hTempXjg->GetYaxis()->SetTitleOffset(1.35);
    handsomeTH1(hTempXjg,0);
    
    for ( int ipt = 1 ; ipt<=nPtBin  ; ipt++) {
      
      c101->cd(ipt);
      
      // draw pp
      hTempXjg->DrawCopy();
      hxjg[kPPMC][7][ipt]->Scale(1./rjg[kPPMC][7]->GetBinContent(ipt)); // rjg normalization                      
      handsomeTH1(hxjg[kPPMC][7][ipt], 1);
      hxjg[kPPMC][7][ipt]->SetMarkerStyle(24);
      hxjg[kPPMC][7][ipt]->Draw("same hist");
      // draw pbpb 
      for ( int icent = 1; icent <= nCentBinHI ; icent++ ) {
	handsomeTH1(hxjg[kHIMC][icent][ipt],kRed);
	hxjg[kHIMC][icent][ipt]->Scale(1./rjg[kHIMC][icent]->GetBinContent(ipt));  // rjg normalization
	if ( icent == 2 ) hxjg[kHIMC][icent][ipt]->SetMarkerStyle(24);

	if ( icent == 2 ) hxjg[kHIMC][icent][ipt]->Draw("same hist");
	if ( icent == 1 ) hxjg[kHIMC][icent][ipt]->Draw("same");
	
      }
    
       
      double dx1=0.15;
      if ( ipt == nPtBin ) 
	drawText(Form("p_{T}^{#gamma} < %dGeV, ", (int)ptBin[ipt-1]), 0.15+dx1+0.25,0.85,1,15);//yeonju 130823
      else
	drawText(Form("%dGeV < p_{T}^{#gamma} < %dGeV, ", (int)ptBin[ipt-1], (int)ptBin[ipt]), 0.15+dx1,0.85,1,15);//yeonju 130823
      
      if ( ipt == nPtBin )  {
	TLegend *l0 = new TLegend(-0.0007607977,0.6729655,0.8215427,0.9383604,NULL,"brNDC");
	easyLeg(l0,"MC  2.76TeV");
	l0->AddEntry(hxjg[kPPMC][7][ipt],"PYTHIA   pp ","l");
	l0->AddEntry(hxjg[kHIMC][2][ipt],"PYTHIA+HYDJET 30-100%","l");
	l0->AddEntry(hxjg[kHIMC][1][ipt],"PYTHIA+HYDJET 0-30%","p");
	l0->Draw();
      }
      
      onSun(30,0,200,0);
    }
    c101->SaveAs("figures/drawPbPb_pp_hxjg.pdf");
    c101->SaveAs("figures/drawPbPb_pp_hxjg.gif");


    TCanvas* c32 = new TCanvas("c32","",500,500);
    handsomeTH1(meanXjg[kPPMC][7], 1);
    meanXjg[kPPMC][7]->SetYTitle("<x_{J#gamma}>  (>30GeV)");
    meanXjg[kPPMC][7]->SetMarkerStyle(24);
    //  meanXjg[kPPMC][7]->SetAxisRange(-2,2,"X");
    meanXjg[kPPMC][7]->SetAxisRange(0.8,1.05,"Y");
    meanXjg[kPPMC][7]->Draw();

    for ( int icent = 1; icent <= nCentBinHI ; icent++ ) {
        handsomeTH1(meanXjg[kHIMC][icent],kRed);
        if ( icent == 2 ) meanXjg[kHIMC][icent]->SetMarkerStyle(24);
	meanXjg[kHIMC][icent]->Draw("same");
    }
    
    if (1 == 1) {
      TLegend *l1mc = new TLegend(0.3387097,0.6864407,0.8729839,0.9004237,NULL,"brNDC");
      easyLeg(l1mc,"MC  2.76TeV");
      l1mc->AddEntry(meanXjg[kPPMC][7],"PYTHIA   pp ","l");
      l1mc->AddEntry(meanXjg[kHIMC][2],"PYTHIA+HYDJET 30-100%","l");
      l1mc->AddEntry(meanXjg[kHIMC][1],"PYTHIA+HYDJET 0-30%","p");
      l1mc->Draw();
    }
    c32->SaveAs("figures/drawPbPb_pp_meanXjg_kPPMC_7.pdf");
    c32->SaveAs("figures/drawPbPb_pp_meanXjg_kPPMC_7.gif");

    TCanvas* c33 = new TCanvas("c33","",500,500);
    handsomeTH1(meanJetPt[kPPMC][7], 1);
    meanJetPt[kPPMC][7]->SetYTitle("<p_{T}^{Jet}>  (>30GeV)");
    meanJetPt[kPPMC][7]->SetMarkerStyle(24);
    //  meanJetPt[kPPMC][7]->SetAxisRange(-2,2,"X");
    meanJetPt[kPPMC][7]->SetAxisRange(40,80,"Y");
    meanJetPt[kPPMC][7]->Draw();

    for ( int icent = 1; icent <= nCentBinHI ; icent++ ) {
        handsomeTH1(meanJetPt[kHIMC][icent],kRed);
        if ( icent == 2 ) meanJetPt[kHIMC][icent]->SetMarkerStyle(24);
	meanJetPt[kHIMC][icent]->Draw("same");
    }
    c33->SaveAs("figures/drawPbPb_pp_meanJetPt_kPPMC_7.pdf");
    c33->SaveAs("figures/drawPbPb_pp_meanJetPt_kPPMC_7.gif");

    if (1 == 1) {
      TLegend *l1mc = new TLegend(0.3810484,0.1864407,0.891129,0.4004237,NULL,"brNDC");
      easyLeg(l1mc,"MC  2.76TeV");
      l1mc->AddEntry(meanJetPt[kPPMC][7],"PYTHIA   pp ","p");
      l1mc->AddEntry(meanJetPt[kHIMC][2],"PYTHIA+HYDJET 30-100%","p");
      l1mc->AddEntry(meanJetPt[kHIMC][1],"PYTHIA+HYDJET 0-30%","p");
      l1mc->Draw();
    }
    
    TCanvas* c33_genJet = new TCanvas("c33_genJet","",500,500);
    handsomeTH1(meanGenJetPt[kPPMC][7], 1);
    meanGenJetPt[kPPMC][7]->SetYTitle("<p_{T}^{Jet}>  (>30GeV)");
    meanGenJetPt[kPPMC][7]->SetMarkerStyle(24);
    //  meanGenJetPt[kPPMC][7]->SetAxisRange(-2,2,"X");
    meanGenJetPt[kPPMC][7]->SetAxisRange(40,80,"Y");
    meanGenJetPt[kPPMC][7]->Draw();

    for ( int icent = 1; icent <= nCentBinHI ; icent++ ) {
        handsomeTH1(meanGenJetPt[kHIMC][icent],kRed);
        if ( icent == 2 ) meanGenJetPt[kHIMC][icent]->SetMarkerStyle(24);
	meanGenJetPt[kHIMC][icent]->Draw("same");
    }

    if (1 == 1) {
      TLegend *l1mc = new TLegend(0.3810484,0.1864407,0.891129,0.4004237,NULL,"brNDC");
      easyLeg(l1mc,"Gen Jet Used");
      l1mc->AddEntry(meanGenJetPt[kPPMC][7],"PYTHIA   pp ","p");
      l1mc->AddEntry(meanGenJetPt[kHIMC][2],"PYTHIA+HYDJET 30-100%","p");
      l1mc->AddEntry(meanGenJetPt[kHIMC][1],"PYTHIA+HYDJET 0-30%","p");
      l1mc->Draw();
    }
    c33_genJet->SaveAs("figures/drawPbPb_pp_meanGenJetPt_kPPMC_7.pdf");
    c33_genJet->SaveAs("figures/drawPbPb_pp_meanGenJetPt_kPPMC_7.gif");

    TCanvas* c34 = new TCanvas("c34","",500,500);
    handsomeTH1(rjg[kPPMC][7], 1);
    rjg[kPPMC][7]->SetYTitle("r_{j#gamma}  (>30GeV)");
    rjg[kPPMC][7]->SetMarkerStyle(24);
    //  rjg[kPPMC][7]->SetAxisRange(-2,2,"X");
    rjg[kPPMC][7]->SetAxisRange(.2,1,"Y");
    rjg[kPPMC][7]->Draw();

    for ( int icent = 1; icent <= nCentBinHI ; icent++ ) {
        handsomeTH1(rjg[kHIMC][icent],kRed);
        if ( icent == 2 ) rjg[kHIMC][icent]->SetMarkerStyle(24);
	rjg[kHIMC][icent]->Draw("same");
    }

    if (1 == 1) {
      TLegend *l1mc = new TLegend(0.3810484,0.1864407,0.891129,0.4004237,NULL,"brNDC");
      easyLeg(l1mc,"MC  2.76TeV");
      l1mc->AddEntry(rjg[kPPMC][7],"PYTHIA   pp ","p");
      l1mc->AddEntry(rjg[kHIMC][2],"PYTHIA+HYDJET 30-100%","p");
      l1mc->AddEntry(rjg[kHIMC][1],"PYTHIA+HYDJET 0-30%","p");
      l1mc->Draw();
    }
    c34->SaveAs("figures/drawPbPb_pp_rjg_kPPMC_7.pdf");
    c34->SaveAs("figures/drawPbPb_pp_rjg_kPPMC_7.gif");

    TCanvas* c34_genJet = new TCanvas("c34_genJet","",500,500);
    handsomeTH1(rGenjg[kPPMC][7], 1);
    rGenjg[kPPMC][7]->SetYTitle("r_{j#gamma}  (>30GeV)");
    rGenjg[kPPMC][7]->SetMarkerStyle(24);
    //  rGenjg[kPPMC][7]->SetAxisRange(-2,2,"X");
    rGenjg[kPPMC][7]->SetAxisRange(.2,1,"Y");
    rGenjg[kPPMC][7]->Draw();

    for ( int icent = 1; icent <= nCentBinHI ; icent++ ) {
        handsomeTH1(rGenjg[kHIMC][icent],kRed);
        if ( icent == 2 ) rGenjg[kHIMC][icent]->SetMarkerStyle(24);
	rGenjg[kHIMC][icent]->Draw("same");
    }

    if (1 == 1) {
      TLegend *l1mc = new TLegend(0.3810484,0.1864407,0.891129,0.4004237,NULL,"brNDC");
      easyLeg(l1mc,"Gen Jet Used");
      l1mc->AddEntry(rGenjg[kPPMC][7],"PYTHIA   pp ","p");
      l1mc->AddEntry(rGenjg[kHIMC][2],"PYTHIA+HYDJET 30-100%","p");
      l1mc->AddEntry(rGenjg[kHIMC][1],"PYTHIA+HYDJET 0-30%","p");
      l1mc->Draw();
    }
    c34_genJet->SaveAs("figures/drawPbPb_pp_rGenjg_kPPMC_7.pdf");
    c34_genJet->SaveAs("figures/drawPbPb_pp_rGenjg_kPPMC_7.gif");
    
} 

/*  
  //    onSun(meanX[iSpecies][iglb].val - meanX[iSpecies][iglb].err, 0, meanX[iSpecies][iglb].val+meanX[iSpecies][iglb].err,0,ycolor[ipt+1],2);
    
    float dx1;    
    (ipt==0)? dx1=0.15 : dx1=0 ;
    
    if ( ipt == nPtBin-1 ) 
      drawText(Form("p_{T}^{#gamma} > %dGeV,   p_{T}^{jet}>30GeV", (int)ptBin[ipt]), 0.12+dx1+0.25,0.889118,1,15);
    else 
      drawText(Form("%dGeV < p_{T}^{#gamma} < %dGeV,   p_{T}^{jet}>30GeV", (int)ptBin[ipt], (int)ptBin[ipt+1]), 0.12+dx1,0.889118,1,15);

      
    TLegend *l1 = new TLegend(0.6365615,0.6445304,0.9577623,0.846736,NULL,"brNDC");
    easyLeg(l1,"2.76TeV");
    l1->AddEntry(hdptNorm[kPPDATA][ipt + kPPDATA*50],"pp ","p");
    if (drawPbPb)    l1->AddEntry(hdptNorm[kHIDATA][iglb],"PbPb 50-100%","p");
    if ( ipt==0 )   l1->Draw();
    

  }
  c1->SaveAs(Form("gifs/dpt_ptDependence_drawPbPb%d_norm%d.pdf",drawPbPb,xNorm));

  TCanvas* cDphi = new TCanvas("cDphi","",900,350);
  makeMultiPanelCanvas(cDphi,nPtBin,1,0.0,0.0,0.2,0.15,0.02);
  for ( int ipt = 0 ; ipt<nPtBin  ; ipt++) {
    cDphi->cd(ipt+1);
    
    int iSpecies = kPPDATA;
    int iglb = ipt + iSpecies*50.;
    handsomeTH1(hdphi[iSpecies][iglb],1);
    hdphi[iSpecies][iglb]->SetYTitle("x_{J,#gamma}");
    hdphi[iSpecies][iglb]->SetXTitle("#Delta#phi_{J,#gamma}");
    hdphi[iSpecies][iglb]->Scale(1./hdphi[iSpecies][iglb]->Integral("width"));
    hdphi[iSpecies][iglb]->SetAxisRange(0.01,30,"Y");
    hdphi[iSpecies][iglb]->SetMarkerStyle(24);
    hdphi[iSpecies][iglb]->Draw();

    iSpecies = kHIDATA;
    iglb = ipt + iSpecies*50.;
    hdphi[iSpecies][iglb]->Scale(1./hdphi[iSpecies][iglb]->Integral("width"));
    handsomeTH1(hdphi[iSpecies][iglb],2);
    if (drawPbPb)     hdphi[iSpecies][iglb]->Draw("same");
    gPad->SetLogy();
    float dx1;    
    (ipt==0)? dx1=0.15 : dx1=0 ;
    
    if ( ipt == nPtBin-1 ) 
      drawText(Form("%dGeV<p_{T}^{#gamma},   p_{T}^{jet}>30GeV", (int)ptBin[ipt]), 0.15+dx1+0.25,0.889118,1,15);
    else 
      drawText(Form("%dGeV<p_{T}^{#gamma}<%dGeV,   p_{T}^{jet}>30GeV", (int)ptBin[ipt], (int)ptBin[ipt+1]), 0.15+dx1,0.889118,1,15);
    
      
    TLegend *l1 = new TLegend(0.6365615,0.6445304,0.9577623,0.846736,NULL,"brNDC");
    easyLeg(l1,"2.76TeV");
    l1->AddEntry(hdphi[kPPDATA][ipt + kPPDATA*50],"pp ","p");
    if (drawPbPb)    l1->AddEntry(hdphi[kHIDATA][iglb],"PbPb 0-30%","p");
    if ( ipt==0 )   l1->Draw();

  }

  
  //  cDphi->SaveAs(Form("gifs/dphi_ptDependence_drawPbPb%d_norm%d.pdf",drawPbPb,xNorm));
  
  TCanvas* c2 = new TCanvas("c2","",800,400);
  c2->Divide(2,1);
  c2->cd(2);
  TH1D* hr = new TH1D("hr",";p_{T}^{#gamma};r_{J,#gamma}",200,20,120);
  handsomeTH1(hr,0);
  hr->SetAxisRange(0.0,1.3,"Y");
  //  hr->SetNdivisions(5);
  hr->Draw();
  handsomeTGraph(gr[kPPDATA],1);
  handsomeTGraph(gr[kHIDATA],2);
  gr[kPPDATA]->SetMarkerStyle(24);
  gr[kPPDATA]->Draw("same p");
  if (drawPbPb)   gr[kHIDATA]->Draw("same p");
  jumSun(20,1,120,1);


  
  c2->cd(1);
  TH1D* hx = (TH1D*)hr->Clone("hx");
  handsomeTH1(hx,0);
  hx->SetYTitle("<#Deltap_{T}^{J,#gamma}>");
  //  hx->SetAxisRange(-60,10,"Y");
  hx->SetAxisRange(30,100,"Y");
  //  hx->SetNdivisions(5);
  hx->Draw();
  handsomeTGraph(gdpt[kPPDATA],1);
  handsomeTGraph(gdpt[kHIDATA],2);
  gdpt[kPPDATA]->SetMarkerStyle(24);
  gdpt[kPPDATA]->Draw("same p");
  if (drawPbPb)   gdpt[kHIDATA]->Draw("same p");
  jumSun(20,1,120,1);

  TLegend *l1=   new TLegend(0.5226071,0.6300415,0.9597724,0.8332584,NULL,"brNDC");
  easyLeg(l1,"2.76TeV");
  l1->AddEntry(gr[kPPDATA],"pp ","p");
  if (drawPbPb)     l1->AddEntry(gr[kHIDATA],"PbPb 30-100%","p");
  l1->Draw();

  //  c2->SaveAs(Form("gifs/summaryPlot_ptDependence_drawPbPb%d_norm%d.pdf",drawPbPb,xNorm));

}

*/
