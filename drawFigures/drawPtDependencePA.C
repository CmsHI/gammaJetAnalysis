#include "../../HiForestAnalysis/hiForest.h"
#include "../CutAndBinCollection2012.h"
#include <TRandom3.h>
#include <time.h>

void drawPtDependencePA(TString dirName="nominal", int prodDate=20131021, int jetPtCut=30) {
  bool saveFigures=true;

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
    TH1D* meanXjg[7][10];      // [Collision][centrality]
    TH1D* meanJetPt[7][10];      // [Collisi on][centrality]

    TH1D* rjg[7][5];     //  [Collision][centrality]
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
                    //hEta[icoll][icent][ipt]  = (TH1D*)histFile[icoll][ipt]->Get(Form("etaJg_icent%d_final", icent)) ;
                    //cout << " Getting histogram : " << Form("etaJg_icent%d_final", icent) << endl;

                }

		if ( (icoll == kPADATA) || (icoll == kPAMC) )   {  //  PA
                    int icent = 1 ;
                    hxjg[icoll][icent][ipt] = (TH1D*)histFile[icoll][ipt]->Get(Form("xjg_icent%d_final", icent)) ;
                    cout << " Getting histogram : " << Form("xjg_icent%d_final", icent) << endl;
                    hJetPt[icoll][icent][ipt] = (TH1D*)histFile[icoll][ipt]->Get(Form("jetPt_icent%d_final", icent)) ;
                    cout << " Getting histogram : " << Form("jetPt_icent%d_final", icent) << endl;
		    hJetPtIaaBin[icoll][icent][ipt] = (TH1D*)histFile[icoll][ipt]->Get(Form("jetPtForIaa_icent%d_final", icent ) ) ;
		    cout << " Getting histogram : " << Form("jetPtForIaa_icent%d_final", icent ) << endl;

		    hDphi[icoll][icent][ipt]  = (TH1D*)histFile[icoll][ipt]->Get(Form("jetDphi_icent%d_final", icent)) ;
                    cout << " Getting histogram : " << Form("jetDphi_icent%d_final", icent) << endl;
                    //hEta[icoll][icent][ipt]  = (TH1D*)histFile[icoll][ipt]->Get(Form("etaJg_icent%d_final", icent)) ;
                    //cout << " Getting histogram : " << Form("etaJg_icent%d_final", icent) << endl;

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
                        //hEta[icoll][icent][ipt]  = (TH1D*)histFile[icoll][ipt]->Get(Form("etaJg_icent%d_final", centBinHI[icent] ) ) ;
                        //cout << " Getting histogram : " << Form("etaJg_icent%d_final", centBinHI[icent])<< endl;
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

        handsomeTH1(hDphi[kPAMC][1][ipt], 1);
        hDphi[kPAMC][1][ipt]->SetXTitle("#Delta#phi_{J#gamma}");
        hDphi[kPAMC][1][ipt]->SetYTitle("Normalized entries");
        hDphi[kPAMC][1][ipt]->SetMarkerStyle(24);
        hDphi[kPAMC][1][ipt]->Scale(1./hDphi[kPAMC][1][ipt]->Integral("width"));
        hDphi[kPAMC][1][ipt]->SetAxisRange(0,3.141592,"X");
        hDphi[kPAMC][1][ipt]->SetAxisRange(0.01,30,"Y");
        hDphi[kPAMC][1][ipt]->Draw();

	handsomeTH1(hDphi[kPADATA][1][ipt], kRed);
	hDphi[kPADATA][1][ipt]->Scale(1./hDphi[kPADATA][1][ipt]->Integral("width"));
	hDphi[kPADATA][1][ipt]->Draw("same");

        gPad->SetLogy();

	double dx1=0.15;
	if ( ipt == nPtBin )
	  drawText(Form("p_{T}^{#gamma} > %dGeV, ", (int)ptBin[ipt-1]), 0.12+dx1+0.25,0.85,1,15);//yeonju 130823
	else
	  drawText(Form("%dGeV < p_{T}^{#gamma} < %dGeV, ", (int)ptBin[ipt-1], (int)ptBin[ipt]), 0.12+dx1,0.85,1,15);//yeonju 130823

    }
    c1->cd(1);

    TLegend *l1 = new TLegend(0.2199474,0.5971384,0.7283974,0.8119819,NULL,"brNDC");
    easyLeg(l1,"5.02TeV");
    l1->AddEntry(hDphi[kPAMC][1][1],"pA pythia+HIJING ","p");
    l1->AddEntry(hDphi[kPADATA][1][1],"pA Data","p");
    l1->Draw();

    if (saveFigures) c1->SaveAs("figures/pT_dependence_dphi_pA_figure1.gif");
    if (saveFigures) c1->SaveAs("figures/pT_dependence_dphi_pA_figure1.pdf");

    TH1D* hTempPt = new TH1D("hTemp",";p_{T}^{#gamma} (GeV);",200,10,300);

    TCanvas* c2 = new TCanvas("c2","",1200,350);
    makeMultiPanelCanvas(c2,nPtBin,1,0.0,0.0,0.2,0.15,0.02);
    for ( int ipt = 1 ; ipt<=nPtBin  ; ipt++) {

        c2->cd(ipt);

        hTempPt->SetXTitle("p_{T}^{Jet} (GeV)");
        hTempPt->SetYTitle("#frac{dN}{dp_{T}} #frac{1}{N}");
        hTempPt->SetAxisRange(10,150,"X");
	//        hTempPt->SetAxisRange(0,0.025,"Y");
	hTempPt->SetAxisRange(0,0.08,"Y");
        handsomeTH1(hTempPt,0);
        hTempPt->DrawCopy();

	//	hJetPt[kPAMC][1][ipt]->Scale(1./rjg[kPAMC][1]->GetBinContent(ipt)); // rjg normalization

        handsomeTH1(hJetPt[kPAMC][1][ipt], 1);
        hJetPt[kPAMC][1][ipt]->SetMarkerStyle(24);
	hJetPt[kPAMC][1][ipt]->Draw("same");

	handsomeTH1(hJetPt[kPADATA][1][ipt],kRed);
	//	hJetPt[kPADATA][1][ipt]->Scale(1./rjg[kPADATA][1]->GetBinContent(ipt));

	hJetPt[kPADATA][1][ipt]->Draw("same");


        double dx1=0.15;
        if ( ipt == nPtBin )
            drawText(Form("p_{T}^{#gamma} > %dGeV, ", (int)ptBin[ipt-1]), 0.12+dx1+0.25,0.85,1,15);//yeonju 130823
        else
            drawText(Form("%dGeV < p_{T}^{#gamma} < %dGeV, ", (int)ptBin[ipt-1], (int)ptBin[ipt]), 0.12+dx1,0.85,1,15);//yeonju 130823

        onSun(30,0,200,0);
    }
    c2->cd(1);
    l1->Draw();

    if (saveFigures)   c2->SaveAs("figures/pT_dependence_jetPt_pA_figure1.pdf");
    if (saveFigures)   c2->SaveAs("figures/pT_dependence_jetPt_pA_figure1.gif");

    TLegend *l2 = new TLegend(0.2116935,0.7012712,0.6149194,0.904661,NULL,"brNDC");
    easyLeg(l2,"5.02TeV");

    l2->AddEntry(hDphi[kPAMC][1][1],"pPb pythia+HIJING","p");
    l2->AddEntry(hDphi[kPADATA][1][1],"pPb Data","p");


    TCanvas* c21 = new TCanvas("c21","",500,500);
    handsomeTH1(meanJetPt[kPAMC][1], 1);
    meanJetPt[kPAMC][1]->SetYTitle("<p_{T}^{Jet}>  (>30GeV)");
    meanJetPt[kPAMC][1]->SetMarkerStyle(24);
    //  meanJetPt[kPAMC][1]->SetAxisRange(-2,2,"X");
    meanJetPt[kPAMC][1]->SetAxisRange(40,115,"Y");
    meanJetPt[kPAMC][1]->Draw();

    handsomeTH1(meanJetPt[kPADATA][1],kRed);
    meanJetPt[kPADATA][1]->Draw("same");

    l1 ->Draw();
    if (saveFigures)   c21->SaveAs("figures/pT_dependence_jetPt_pA_figure2.pdf");
    if (saveFigures)   c21->SaveAs("figures/pT_dependence_jetPt_pA_figure2.gif");

    TCanvas* c3 = new TCanvas("c3","",1200,350);
    makeMultiPanelCanvas(c3,nPtBin,1,0.0,0.0,0.2,0.15,0.02);
    for ( int ipt = 1 ; ipt<=nPtBin  ; ipt++) {

        c3->cd(ipt);

        handsomeTH1(hxjg[kPAMC][1][ipt], 1);
        hxjg[kPAMC][1][ipt]->SetXTitle("x_{J#gamma}");
        hxjg[kPAMC][1][ipt]->SetYTitle("#frac{dN}{dp_{T}} #frac{1}{N}");
        hxjg[kPAMC][1][ipt]->SetMarkerStyle(24);

	//	hxjg[kPAMC][1][ipt]->Scale(1./rjg[kPAMC][1]->GetBinContent(ipt)); // rjg normalization


        hxjg[kPAMC][1][ipt]->SetAxisRange(0,2,"X");
        hxjg[kPAMC][1][ipt]->Draw();

	handsomeTH1(hxjg[kPADATA][1][ipt],kRed);
	//	hxjg[kPADATA][1][ipt]->Scale(1./rjg[kPADATA][1]->GetBinContent(ipt)); // rjg normalization


	hxjg[kPADATA][1][ipt]->Draw("same");

        double dx1=0.15;
        if ( ipt == nPtBin )
            drawText(Form("p_{T}^{#gamma} > %dGeV, ", (int)ptBin[ipt-1]), 0.12+dx1+0.25,0.85,1,15);//yeonju 130823
        else
            drawText(Form("%dGeV < p_{T}^{#gamma} < %dGeV, ", (int)ptBin[ipt-1], (int)ptBin[ipt]), 0.12+dx1,0.85,1,15);//yeonju 130823


        onSun(30,0,200,0);
    }
    c3->cd(1);
    l1->Draw();

    if (saveFigures)   c3->SaveAs("figures/pT_dependence_xjg_pA_figure1.pdf");
    if (saveFigures)   c3->SaveAs("figures/pT_dependence_xjg_pA_figure1.gif");

    TCanvas* c31 = new TCanvas("c31","",500,500);
    handsomeTH1(meanXjg[kPAMC][1], 1);
    meanXjg[kPAMC][1]->SetYTitle("<x_{J#gamma}>  (>30GeV)");
    meanXjg[kPAMC][1]->SetMarkerStyle(24);
    //  meanXjg[kPAMC][1]->SetAxisRange(-2,2,"X");
    meanXjg[kPAMC][1]->SetAxisRange(0.6,1.2,"Y");
    meanXjg[kPAMC][1]->Draw();

    handsomeTH1(meanXjg[kPADATA][1],kRed);
    meanXjg[kPADATA][1]->Draw("same");

    l1->Draw();
    if (saveFigures)   c31->SaveAs("figures/pT_dependence_xjg_pA_figure2.pdf");
    if (saveFigures)   c31->SaveAs("figures/pT_dependence_xjg_pA_figure2.gif");


    TCanvas* c_rjg = new TCanvas("c_rjg","",500,500);
    handsomeTH1(rjg[kPAMC][1], 1);
    rjg[kPAMC][1]->SetYTitle("R_{J#gamma}");
    rjg[kPAMC][1]->SetMarkerStyle(24);
    //  rjg[kPAMC][1]->SetAxisRange(-2,2,"X");
    rjg[kPAMC][1]->SetAxisRange(0.0,1.1,"Y");
    rjg[kPAMC][1]->Draw();

    handsomeTH1(rjg[kPADATA][1],kRed);
    rjg[kPADATA][1]->Draw("same");


    l1->Draw();
    if (saveFigures)   c_rjg->SaveAs("figures/pT_dependence_rjg_pA_figure1.pdf");
    if (saveFigures)   c_rjg->SaveAs("figures/pT_dependence_rjg_pA_figure1.gif");


    // Save the final root histogram files
    TFile * fResults =  new TFile("resultHistograms.root","update");
    //    TFile * fResults =  new TFile("resultHistograms_ppb_photonEnergy_ScaledBy0.015.root","update");


    meanJetPt[kPADATA][1]->SetName(Form("meanJetPt_ppb"));
    meanJetPt[kPADATA][1]->Write();

    meanXjg[kPADATA][1]->SetName(Form("meanXjg_ppb"));
    meanXjg[kPADATA][1]->Write();

    rjg[kPADATA][1]->SetName(Form("meanRjg_ppb"));
    rjg[kPADATA][1]->Write();

    for ( int ipt = 1 ; ipt<=nPtBin  ; ipt++) {
      hJetPt[kPADATA][1][ipt]->SetName(Form("dNdJetPt_ppb_ptBin%d",ipt));
      hJetPt[kPADATA][1][ipt]->Write();
      hJetPtIaaBin[kPADATA][1][ipt]->SetName(Form("dNdJetPt_IaaBin_ppb_ptBin%d",ipt));
      hJetPtIaaBin[kPADATA][1][ipt]->Write();

      hxjg[kPADATA][1][ipt]->SetName(Form("dNdXjg_ppb_ptBin%d",ipt));
      hxjg[kPADATA][1][ipt]->Write();

      hDphi[kPADATA][1][ipt]->SetName(Form("dNdphi_ppb_ptBin%d",ipt));
      hDphi[kPADATA][1][ipt]->Write();

    }

    fResults->Close();

}
