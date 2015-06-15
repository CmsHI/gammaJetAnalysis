///////////////////////////////////////////////////////////////////
// gammaJetHistProducer.C                                        //
// Creator : Yongsun Kim (MIT), jazzitup@mit.edu                 //
// Function : Make the correlation function between Photon and   //
//            tracks.  Signal and Background are calculated      //
//            separately, and then background subtraction is     //
//            done.  Output histogrmas are saved in output file  //
///////////////////////////////////////////////////////////////////
#include <TStyle.h>
#include <TH1F.h>
#include <TH2D.h>
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TGraphAsymmErrors.h>
#include <iostream>
#include <TLine.h>
#include <TMath.h>
#include <math.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include <TROOT.h>
#include <TRandom.h>
#include <TCut.h>
#include <TClonesArray.h>
#include <TRandom3.h>
#include "../../HiForestAnalysis/hiForest.h"
#include "../CutAndBinCollection2012.h"
#include "corrFunctionJetTrk.h"

GjSpectra* nullGj;

void gammaTrkSingle(     GjSpectra* gjSpec_=nullGj,
			 multiTreeUtil* tObj[3]=0,
			 corrFunctionTrk* corr_=0,
			 float purity=0,
			 sampleType collision_=kHIDATA,
			 TString var_ = "",
			 TCut cut_ ="",
			 TString theWeight="",
			 TCut phoCandCut_="",
			 TCut phoDecayCut_="",
			 TH1D* hTemplate_ =0,
			 TString outName="");

fitResult getPurity(TString fname="", sampleType collision=kHIDATA, TCut candEvtCut = "", TCut sbEvtCut="", TString ccanvasName="",float photonPtThr=60, float photonPtThrUp=9999);


void gammaJetHistProducer(sampleType collision = kPADATA, float photonPtThr=60, float photonPtThrUp=9999, float jetPtThr=30, int icent =1) {
  TH1::SetDefaultSumw2();

  TString stringSampleType = getSampleName(collision); //"";

  TDatime* date = new TDatime();
  TString  outName=  Form("photonTrackCorr_%s_output_photonPtThr%d_to_%d_jetPtThr%d_%d.root",stringSampleType.Data(),(int)photonPtThr, (int)photonPtThrUp, (int)jetPtThr,  date->GetDate());
  delete date;

  int lowerCent(0),  upperCent(0);
  TCut centCut  = "";
  if ( (collision ==kHIDATA) || (collision==kHIMC) )   {
    if ( icent > 9999) {
      lowerCent = ((icent/100)%100)/2.5;
      upperCent =  (icent%100)/2.5 -1;
    } else {
      lowerCent = centBin1[icent-1];
      upperCent = centBin1[icent]-1;
    }
    centCut = Form("cBin >= %d && cBin<= %d",lowerCent,upperCent);
  }
  else if (  (collision ==kPPDATA) || (collision==kPPMC)  ){  // if it's pp
    centCut = "(1==1)";
    //    icent = 7;   // for pp, centrality is set as the smearing
  }
  else { // pPb
    centCut = Form( "hf4Sum > %f && hf4Sum <= %f", (float)centBinPa[icent-1], (float)centBinPa[icent]);
  }

  cout <<" centrality : " << centCut.GetTitle() << endl;

  ///////// Photon cut //////////////////////////////////////////////////////////////////////////////

  cout <<" photon pt >" << photonPtThr << " GeV" << endl;
  TCut ptPhoCut  = Form("photonEt>%.1f && photonEt<%.1f", (float)photonPtThr, (float)photonPtThrUp  );
  TCut caloIso;

  if ( (collision==kPPMC) || (collision==kPPDATA) )
    caloIso = "(ecalIso < 4.2  &&  hcalIso < 2.2  &&  trackIso < 2) && hovere<0.1";
  else if ( (collision==kHIMC) || (collision==kHIDATA) )
    caloIso = "(sumIso<1) && hovere<0.1";
  else {
    caloIso = "ecalIso < 4.2  &&  hcalIso < 2.2  &&  trackIso < 2 && hovere<0.1";
  }

  TCut sbIso   = "(sumIso>10) && (sumIso<20) && hovere<0.1";
  //  if ( (collision==kPPMC) || (collision==kPPDATA) || (collision==kPAMC) || (collision==kPADATA)  )
  //  sbIso   = "ecalIso < 4.2  &&  hcalIso < 2.2 && trackIso > 2 && trackIso < 5 && hovere<0.1";

  TCut basicPhoCut = centCut && ptPhoCut && caloIso ;
  TCut sbPhoCut    = centCut && ptPhoCut && sbIso   ;
  TCut evtSeltCut = basicPhoCut;
  TCut sbSeltCut  = sbPhoCut;

  TCut phoCandCut   = "sigmaIetaIeta<0.010";
  TCut phoDecayCut  = "(sigmaIetaIeta>0.011) && (sigmaIetaIeta<0.017)";
  if  ( ( collision == kHIMC ) || (collision == kPPMC) || (collision == kPAMC))
    phoCandCut = phoCandCut && "genIso<5 && abs(genMomId)<=22";


  TString fname = "";
  if ( collision == kHIDATA)      fname = fnameHIDATA; //_jetResCorrected;
  else if ( collision == kPADATA) fname = fnamePADATA;
  else if ( collision == kPPDATA) {
    if ( icent == 7 ) fname = fnamePPDATA;
    else if ( icent == 10010 ) fname = fnamePPDATA0010;
    else if ( icent == 11030 ) fname = fnamePPDATA1030;
    else if ( icent == 13050 ) fname = fnamePPDATA3050;
    else if ( icent == 15099 ) fname = fnamePPDATA5099;
    else if ( icent == 10030 ) fname = fnamePPDATA0030;
    else if ( icent == 13099 ) fname = fnamePPDATA30100;
  }
  else fname = "";

  multiTreeUtil* tgj = new multiTreeUtil();
  //multiTreeUtil* tgjMC = new multiTreeUtil();
  if (  ( collision == kHIDATA)   || ( collision==kPADATA) || ( collision == kPPDATA) ) {
    tgj->addFile(fname,  "tgj",  evtSeltCut,  1);
  }
  else if ( collision == kPPMC ) {
    tgj->addFile(fnamePPMC_AllQcdPho30to50,    "tgj", evtSeltCut, wPPMC_AllQcdPho30to50 );
    tgj->addFile(fnamePPMC_AllQcdPho50to80,    "tgj", evtSeltCut, wPPMC_AllQcdPho50to80 );
    tgj->addFile(fnamePPMC_AllQcdPho80to120,   "tgj", evtSeltCut, wPPMC_AllQcdPho80to120 );
    tgj->addFile(fnamePPMC_AllQcdPho120to9999, "tgj", evtSeltCut, wPPMC_AllQcdPho120to9999 );
  }
  else if ( collision == kPAMC ) {
    tgj->addFile(fnamePAMC_AllQcdPho30to50,    "tgj", evtSeltCut, wPAMC_AllQcdPho30to50 );
    tgj->addFile(fnamePAMC_AllQcdPho50to80,    "tgj", evtSeltCut, wPAMC_AllQcdPho50to80 );
    tgj->addFile(fnamePAMC_AllQcdPho80to120,   "tgj", evtSeltCut, wPAMC_AllQcdPho80to120 );
    tgj->addFile(fnamePAMC_AllQcdPho120to170, "tgj", evtSeltCut, wPAMC_AllQcdPho120to170 );
    tgj->addFile(fnamePAMC_AllQcdPho170to9999, "tgj", evtSeltCut, wPAMC_AllQcdPho170to9999 );
  }
  else  {    // kHIMC
    tgj->addFile(fnameHIMC_AllQcdPho30to50,    "tgj", evtSeltCut, wHIMC_AllQcdPho30to50 );
    tgj->addFile(fnameHIMC_AllQcdPho50to80,    "tgj", evtSeltCut, wHIMC_AllQcdPho50to80 );
    tgj->addFile(fnameHIMC_AllQcdPho80to9999,  "tgj", evtSeltCut, wHIMC_AllQcdPho80to9999 );
  }
  tgj->AddFriend("yJet");

  // get purity with the current jet cut !
  float purity(0);

  TString canvasName = Form("gifs/purity_%s_output_icent%d_photonPtThr%d-%d_jetPtThr%d", stringSampleType.Data(),  (int)icent, (int)photonPtThr, (int)photonPtThrUp, (int)jetPtThr);

  //  if ( (collision==kPPDATA) && (photonPtThr < 50 ) ) {
  //   purity = 0.86 ;
  //   cout << " !!!!!!!" << endl << endl << " purity is set as  0.86 for this bin because we don't have pp MC low pt sample " << endl;
  //   cout << endl << endl << endl << " !!!!!!" << endl;
  //  }

  if ( (collision==kHIDATA)||(collision==kPPDATA)||(collision==kPADATA) )  {

    fitResult fitr = getPurity(fname, collision, evtSeltCut, sbSeltCut, canvasName, photonPtThr, photonPtThrUp);
    purity = fitr.purity010;

    /*
      float originalP = purity;
      if (collision==kPPDATA)
      purity = purity - 0.05;
      else if (collision==kHIDATA) {
      if ( icent == 10030)
      purity = purity - 0.08;
      else if ( icent == 13099)
      purity = purity - 0.07;
      }
      cout << "====================================================================================" << endl << endl << endl;
      cout << "====================================================================================" << endl << endl << endl;
      cout << "================================ Purity modified!!! ================================" << endl << endl << endl;
      cout << "=============== "<<originalP<< " --> "<<purity<< "====================================" << endl << endl << endl;
      cout << "====================================================================================" << endl << endl << endl;
    */
  }
  else  {
    purity = 1;
    cout << " MC purity = 1" << endl;
  }

  TString vtxCentReweight = "evt.vtxCentWeight";
  if(collision == kPPDATA)
  {
      vtxCentReweight = "1/100";    // must account for 100 times smearing during "yskim"
  }
  GjSpectra* gSpec = new GjSpectra();
  gSpec->init(Form("icent%d",(int)icent) );
  tgj->Draw2(gSpec->hPtPhoCand,  "photonEt", phoCandCut, vtxCentReweight)  ;
  tgj->Draw2(gSpec->hPtPhoDecay, "photonEt", phoDecayCut, vtxCentReweight) ;

  // Obtain background subtracted spectra

  float candInt = gSpec->hPtPhoCand->Integral();
  float candDecay = gSpec->hPtPhoDecay->Integral();
  gSpec->hPtPhoSig->Reset();
  gSpec->hPtPhoSig->Add(gSpec->hPtPhoCand);
  gSpec->hPtPhoSig->Add(gSpec->hPtPhoDecay, -(1. - purity) * candInt / candDecay);
  gSpec->hPtPhoSig->Scale(1./purity ) ;

  TFile *outf = TFile::Open(Form("ffFiles/%s",outName.Data()),"update");
  gSpec->hPtPhoCand->Write();
  gSpec->hPtPhoDecay->Write();
  gSpec->hPtPhoSig->Write();
  outf->Close();



  // Objects
  multiTreeUtil* tObj[3];
  tObj[kTrkRaw] = new multiTreeUtil();
  tObj[kTrkBkg] = new multiTreeUtil();

  if (  ( collision == kHIDATA)   || ( collision==kPADATA) || ( collision == kPPDATA) ) {
    tObj[kTrkRaw]->addFile(fname,  "yJet",  evtSeltCut,  1);
    tObj[kTrkBkg]->addFile(fname,  "mJet",  evtSeltCut,  1);
  }
  else if ( collision == kHIMC ) {
    tObj[kTrkRaw]->addFile(fnameHIMC_AllQcdPho30to50,   "yJet", evtSeltCut, wHIMC_AllQcdPho30to50 ) ;
    tObj[kTrkRaw]->addFile(fnameHIMC_AllQcdPho50to80,   "yJet", evtSeltCut, wHIMC_AllQcdPho50to80 ) ;
    tObj[kTrkRaw]->addFile(fnameHIMC_AllQcdPho80to9999, "yJet", evtSeltCut, wHIMC_AllQcdPho80to9999 ) ;

    tObj[kTrkBkg]->addFile(fnameHIMC_AllQcdPho30to50,   "mJet", evtSeltCut, wHIMC_AllQcdPho30to50 ) ;
    tObj[kTrkBkg]->addFile(fnameHIMC_AllQcdPho50to80,   "mJet", evtSeltCut, wHIMC_AllQcdPho50to80 ) ;
    tObj[kTrkBkg]->addFile(fnameHIMC_AllQcdPho80to9999, "mJet", evtSeltCut, wHIMC_AllQcdPho80to9999 ) ;
  }
  else if ( collision == kPAMC ) {
    tObj[kTrkRaw]->addFile(fnamePAMC_AllQcdPho30to50,   "yJet", evtSeltCut, wPAMC_AllQcdPho30to50 ) ;
    tObj[kTrkRaw]->addFile(fnamePAMC_AllQcdPho50to80,   "yJet", evtSeltCut, wPAMC_AllQcdPho50to80 ) ;
    tObj[kTrkRaw]->addFile(fnamePAMC_AllQcdPho80to120,  "yJet", evtSeltCut, wPAMC_AllQcdPho80to120 ) ;
    tObj[kTrkRaw]->addFile(fnamePAMC_AllQcdPho120to170, "yJet", evtSeltCut, wPAMC_AllQcdPho120to170 ) ;
    tObj[kTrkRaw]->addFile(fnamePAMC_AllQcdPho170to9999,"yJet", evtSeltCut, wPAMC_AllQcdPho170to9999 ) ;

    tObj[kTrkBkg]->addFile(fnamePAMC_AllQcdPho30to50,   "mJet", evtSeltCut, wPAMC_AllQcdPho30to50 ) ;
    tObj[kTrkBkg]->addFile(fnamePAMC_AllQcdPho50to80,   "mJet", evtSeltCut, wPAMC_AllQcdPho50to80 ) ;
    tObj[kTrkBkg]->addFile(fnamePAMC_AllQcdPho80to120,  "mJet", evtSeltCut, wPAMC_AllQcdPho80to120 ) ;
    tObj[kTrkBkg]->addFile(fnamePAMC_AllQcdPho120to170, "mJet", evtSeltCut, wPAMC_AllQcdPho120to170 ) ;
    tObj[kTrkBkg]->addFile(fnamePAMC_AllQcdPho170to9999,"mJet", evtSeltCut, wPAMC_AllQcdPho170to9999 ) ;
  }
  else if ( collision == kPPMC ) {
    tObj[kTrkRaw]->addFile(fnamePPMC_AllQcdPho30to50,   "yJet", evtSeltCut, wPPMC_AllQcdPho30to50 );
    tObj[kTrkRaw]->addFile(fnamePPMC_AllQcdPho50to80,   "yJet", evtSeltCut, wPPMC_AllQcdPho50to80 );
    tObj[kTrkRaw]->addFile(fnamePPMC_AllQcdPho80to120,  "yJet", evtSeltCut, wPPMC_AllQcdPho80to120 );
    tObj[kTrkRaw]->addFile(fnamePPMC_AllQcdPho120to9999,"yJet", evtSeltCut, wPPMC_AllQcdPho120to9999 );

    tObj[kTrkBkg]->addFile(fnamePPMC_AllQcdPho30to50,   "mJet", evtSeltCut, wPPMC_AllQcdPho30to50 );
    tObj[kTrkBkg]->addFile(fnamePPMC_AllQcdPho50to80,   "mJet", evtSeltCut, wPPMC_AllQcdPho50to80 );
    tObj[kTrkBkg]->addFile(fnamePPMC_AllQcdPho80to120,  "mJet", evtSeltCut, wPPMC_AllQcdPho80to120 );
    tObj[kTrkBkg]->addFile(fnamePPMC_AllQcdPho120to9999,"mJet", evtSeltCut, wPPMC_AllQcdPho120to9999 );
  }



  tObj[kTrkRaw]->AddFriend("tgj");
  tObj[kTrkBkg]->AddFriend("tgj");
  TCut jetCut     =  Form("abs(eta)<%f && pt>%f", (float)cutjetEta, (float)jetPtThr );
  TCut jetCutDphi =  jetCut && (TCut)(Form("abs(dphi)>%f",(float)awayRange));
  TCut jetCutSBcut =  jetCut && (TCut)("abs(dphi)>0.5 && abs(dphi)<2");
  TCut genJetCut     =  Form("abs(jtEta)<%f && jtPt>%f", (float)cutjetEta, (float)jetPtThr );
  TCut genJetCutDphi =  jetCut && (TCut)(Form("abs(refDphi)>%f",(float)awayRange));

  TCut jetCut2   = Form("abs(dphi)>%f && pt>%f", (float)awayRange, (float)jetPtThr ) ;


  //  TString jetWeight = ""

  TH1D* hJetDphi = new TH1D(Form("jetDphi_icent%d",icent),";#Delta#phi_{Jet,#gamma} ;dN/d#Delta#phi",20,0,3.141592);
  corrFunctionTrk* cJetDphi = new corrFunctionTrk();
  TString varJetDphi         = Form("dphi");

  gammaTrkSingle( gSpec,  tObj, cJetDphi,  purity,
		  collision, varJetDphi, jetCut, vtxCentReweight,
		  phoCandCut, phoDecayCut,  hJetDphi, outName);

  TH1D* hJetPt = new TH1D(Form("jetPt_icent%d",icent),";Jet p_{T} (GeV) ;dN/dp_{T} (GeV^{-1})",280, 20,300);
  corrFunctionTrk* cJetPt = new corrFunctionTrk();
  TString varJetPt         = Form("pt");

  gammaTrkSingle( gSpec,  tObj, cJetPt,  purity,
		  collision, varJetPt, jetCutDphi, vtxCentReweight,
		  phoCandCut, phoDecayCut,  hJetPt, outName);

  //  TH1D* hJetPtSB = new TH1D(Form("jetPtDphiSideBand_icent%d",icent),";Jet p_{T} (GeV) ;dN/dp_{T} (GeV^{-1})",280, 20,300);
  //  corrFunctionTrk* cJetPtSB = new corrFunctionTrk();
  // gammaTrkSingle( gSpec,  tObj, cJetPtSB,  purity,
  //		  collision, varJetPt, jetCutSBcut, vtxCentReweight,
  //		  phoCandCut, phoDecayCut,  hJetPtSB, outName);

  cout << "IaaBin START" << endl;
  int nJetIaaBin;
  if (photonPtThr==40 || photonPtThr==50) nJetIaaBin = 4;
  else if (photonPtThr==60 || photonPtThr==80) nJetIaaBin = 5;
  double jetIaaBin[nJetIaaBin+1];
  //const int nJetIaaBin = 7;
  //double jetIaaBin[nJetIaaBin+1] = {30,40,50,70,100,150};
  if (photonPtThr==40 || photonPtThr==50) {
      jetIaaBin[0] = 30;
      jetIaaBin[1] = 40;
      jetIaaBin[2] = 50;
      jetIaaBin[3] = 70;
      jetIaaBin[4] = 150;
  } else if (photonPtThr==60 || photonPtThr==80) {
      jetIaaBin[0] = 30;
      jetIaaBin[1] = 40;
      jetIaaBin[2] = 50;
      jetIaaBin[3] = 70;
      jetIaaBin[4] = 100;
      jetIaaBin[5] = 150;
  }
  TH1D* hJetPtForIaa = new TH1D(Form("jetPtForIaa_icent%d",icent),";Jet p_{T} (GeV) ;dN/dp_{T} (GeV^{-1})",nJetIaaBin, jetIaaBin);
  corrFunctionTrk* cJetIaaPt = new corrFunctionTrk();
  gammaTrkSingle( gSpec,  tObj, cJetIaaPt,  purity,
                  collision, varJetPt, jetCutDphi, vtxCentReweight,
                  phoCandCut, phoDecayCut,  hJetPtForIaa, outName);

  //  TH1D* hDjetPt = new TH1D(Form("dpt_icent%d",icent),";p_{T}^{#gamma} - p_{T}^{Jet} (GeV) ;dN/dp_{T} (GeV^{-1})",30,-150,150);
  //  corrFunctionTrk* cDjetPt = new corrFunctionTrk();
  //  TString varJetDpt         = Form("pt - photonEt");
  // gammaTrkSingle( gSpec,  tObj, cDjetPt,  purity,
  //		  collision, varJetDpt, jetCutDphi, vtxCentReweight,
  //		  phoCandCut, phoDecayCut,  hDjetPt, outName);


  TH1D* hJetXjg = new TH1D(Form("xjg_icent%d",icent),";p_{T}^{Jet}/p_{T}^{#gamma}  ; ",16,0,2);
  corrFunctionTrk* cJetXjg = new corrFunctionTrk();
  TString varJetXjg         = Form("pt/photonEt");

  gammaTrkSingle( gSpec,  tObj, cJetXjg,  purity,
		  collision, varJetXjg, jetCutDphi, vtxCentReweight,
		  phoCandCut, phoDecayCut,  hJetXjg, outName);


  if ( (collision == kPAMC) || (collision == kPPMC) ||(collision == kHIMC) ) {
    TH1D* hGenJetPt = (TH1D*)hJetPt->Clone(Form("genJetPt_icent%d",icent));
    //corrFunctionTrk* cGenJetPt = new corrFunctionTrk();
    TString varGenJetPt         = Form("refPt");
    gammaTrkSingle( gSpec,  tObj, cJetPt,  purity,
		    collision, varGenJetPt, genJetCutDphi, vtxCentReweight,
		    phoCandCut, phoDecayCut,  hGenJetPt, outName);

    TH1D* hGenJetXjg = (TH1D*)hJetXjg->Clone(Form("xjg_genJet_icent%d",icent));
    corrFunctionTrk* cGenJetXjg = new corrFunctionTrk();
    TString varGenJetXjg         = Form("refPt/photonEt");
    gammaTrkSingle( gSpec,  tObj, cGenJetXjg,  purity,
		    collision, varGenJetXjg, genJetCutDphi , vtxCentReweight,
		    phoCandCut, phoDecayCut,  hGenJetXjg, outName);

    TH1D* hGenPhotonXjg = (TH1D*)hJetXjg->Clone(Form("xjg_genPho_icent%d",icent));
    corrFunctionTrk* cGenPhotonXjg = new corrFunctionTrk();
    TString varGenPhotonXjg         = Form("pt/genPhotonEt");
    gammaTrkSingle( gSpec,  tObj, cGenPhotonXjg,  purity,
		    collision, varGenPhotonXjg, jetCutDphi, vtxCentReweight,
		    phoCandCut, phoDecayCut,  hGenPhotonXjg, outName);

    TH1D* hGenPhoGenJetXjg = (TH1D*)hJetXjg->Clone(Form("xjg_genPho_genJet_icent%d",icent));
    corrFunctionTrk* cGenPhoGenJetXjg = new corrFunctionTrk();
    TString varGenPhoGenJetXjg         = Form("refPt/genPhotonEt");
    gammaTrkSingle( gSpec,  tObj, cGenPhoGenJetXjg,  purity,
		    collision, varGenPhoGenJetXjg, genJetCutDphi, vtxCentReweight,
		    phoCandCut, phoDecayCut,  hGenPhoGenJetXjg, outName);
  }

  bool doRapidity= false;
  if ( doRapidity) {
  // Eta distribution
    TH1D* hEtaJg = new TH1D(Form("etaJg_icent%d",icent),";#eta_{J,#gamma} ; ",160,-3,3);
    corrFunctionTrk* cEtaJg = new corrFunctionTrk();
    TString varEtaJg         = Form("(photonEta+eta)/2.");

    gammaTrkSingle( gSpec,  tObj, cEtaJg,  purity,
		    collision, varEtaJg, jetCut2, vtxCentReweight,
		    phoCandCut, phoDecayCut,  hEtaJg, outName);

    TH1D* hEtaJet = new TH1D(Form("etaJet_icent%d",icent),";#eta_{Jet} ; ",160,-3,3);
    corrFunctionTrk* cEtaJet = new corrFunctionTrk();
    TString varEtaJet         = Form("eta");

    gammaTrkSingle( gSpec,  tObj, cEtaJet,  purity,
		    collision, varEtaJet, jetCut2, vtxCentReweight,
		    phoCandCut, phoDecayCut,  hEtaJet, outName);

    TH1D* hEtaPhoton = new TH1D(Form("etaPhoton_icent%d",icent),";#eta_{#gamma} ; ",160,-3,3);
    corrFunctionTrk* cEtaPhoton = new corrFunctionTrk();
    TString varEtaPhoton         = Form("photonEta");

    gammaTrkSingle( gSpec,  tObj, cEtaPhoton,  purity,
		    collision, varEtaPhoton, jetCut2, vtxCentReweight,
		    phoCandCut, phoDecayCut,  hEtaPhoton, outName);
  }

}


void gammaTrkSingle(     GjSpectra* gSpec,  multiTreeUtil* tObj[3],   corrFunctionTrk* corr,
			 float purity,       sampleType collision, 	     TString var,
			 TCut cut, 		TString theWeight,	 TCut phoCandCut,   TCut phoDecayCut,
			 TH1D* hTemplate,  TString outfName)
{
  TH1::SetDefaultSumw2();

  TCanvas* c1 = new TCanvas(Form("canvas_%s",hTemplate->GetName()),"",800,650);
  makeMultiPanelCanvas(c1,2,2,0.0,0.0,0.2,0.15,0.02);
  c1->cd(1);
  corr->init(gSpec, collision, purity, hTemplate);
  cout << "Filling raw jets" << endl;
  tObj[kTrkRaw]->Draw2(corr->Func[kPhoCand][kTrkRaw],  var,  phoCandCut  && cut, theWeight);
  if ( tObj[kTrkBkg]!=0)   {
  cout << "Filling mixed jets" << endl;
    tObj[kTrkBkg]->Draw2(corr->Func[kPhoCand][kTrkBkg],  var,  phoCandCut  && cut, theWeight);
  }

  if ( (collision==kHIDATA) || (collision==kPPDATA) || (collision==kPADATA) ) {
    cout << "Filling decay photon-jet correlation" << endl;
    tObj[kTrkRaw]->Draw2(corr->Func[kPhoDecay][kTrkRaw], var, phoDecayCut && cut,  theWeight);
    if (tObj[kTrkBkg]!=0)   {
      cout << "Filling decay photon- mixed jet correlation" << endl;
      tObj[kTrkBkg]->Draw2(corr->Func[kPhoDecay][kTrkBkg], var, phoDecayCut && cut,  theWeight);
    }
  }
  TH1ScaleByWidth( corr->Func[kPhoCand][kTrkRaw]);
  TH1ScaleByWidth( corr->Func[kPhoCand][kTrkBkg]);
  TH1ScaleByWidth( corr->Func[kPhoDecay][kTrkRaw]);
  TH1ScaleByWidth( corr->Func[kPhoDecay][kTrkBkg]);

  corr->calCorr();
  c1->cd(1);
  handsomeTH1(corr->Func[kPhoCand][kTrkRaw],1);
  handsomeTH1(corr->Func[kPhoCand][kTrkBkg],1);
  handsomeTH1(corr->Func[kPhoCand][kTrkSig],2);
  corr->Func[kPhoCand][kTrkRaw]->Draw();
  corr->Func[kPhoCand][kTrkBkg]->Draw("same hist");
  corr->Func[kPhoCand][kTrkSig]->Draw("same");
  gPad->SetLogy();

  c1->cd(2);
  handsomeTH1(corr->Func[kPhoDecay][kTrkRaw],1);
  handsomeTH1(corr->Func[kPhoDecay][kTrkBkg],1);
  handsomeTH1(corr->Func[kPhoDecay][kTrkSig],4);
  corr->Func[kPhoDecay][kTrkRaw]->Draw();
  corr->Func[kPhoDecay][kTrkBkg]->Draw("same hist");
  corr->Func[kPhoDecay][kTrkSig]->Draw("same");
  gPad->SetLogy();

  c1->cd(3);
  corr->Func[kPhoCand][kTrkSig]->Draw();
  corr->Func[kPhoDecay][kTrkSig]->Draw("same");

  c1->cd(4);
  handsomeTH1(corr->Func[kPhoSig][kTrkSig],1);
  corr->Func[kPhoSig][kTrkSig]->Draw();
  c1->SaveAs(Form("gifs/%s_%s.gif",outfName.Data(),c1->GetName()) );

  TFile *outf = TFile::Open(Form("ffFiles/%s",outfName.Data()),"update");
  corr->Func[kPhoCand][kTrkRaw]->Write();
  corr->Func[kPhoCand][kTrkBkg]->Write();
  corr->Func[kPhoCand][kTrkSig]->Write();
  corr->Func[kPhoDecay][kTrkRaw]->Write();
  corr->Func[kPhoDecay][kTrkBkg]->Write();
  corr->Func[kPhoDecay][kTrkSig]->Write();
  corr->Func[kPhoSig][kTrkRaw]->Write();
  corr->Func[kPhoSig][kTrkBkg]->Write();
  corr->Func[kPhoSig][kTrkSig]->Write();

  outf->Close();
}



fitResult getPurity(TString fname, sampleType collision, TCut evtSeltCut, TCut sbEvtCut, TString canvasName, float photonPtThr, float photonPtThrUp) {
  // double purity(0);

  multiTreeUtil* tgj = new multiTreeUtil();
  multiTreeUtil* tgjMC = new multiTreeUtil();
  cout <<" 1 " << endl;
  tgj->addFile(fname,  "tgj",  "",  1);
  cout <<" 2 " << endl;
  tgj->AddFriend("yJet");
  if   (collision==kPPDATA) {
    tgjMC->addFile(fnamePPMC_AllQcdPho30to50,    "tgj", "", wPPMC_AllQcdPho30to50 );
    tgjMC->addFile(fnamePPMC_AllQcdPho50to80,    "tgj", "", wPPMC_AllQcdPho50to80 );
    tgjMC->addFile(fnamePPMC_AllQcdPho80to120,   "tgj", "", wPPMC_AllQcdPho80to120);
    tgjMC->addFile(fnamePPMC_AllQcdPho120to9999, "tgj", "", wPPMC_AllQcdPho120to9999);
  }
  else if (collision==kPADATA)     {
    tgjMC->addFile(fnamePAMC_AllQcdPho30to50,     "tgj","", wPAMC_AllQcdPho30to50);
    tgjMC->addFile(fnamePAMC_AllQcdPho50to80,     "tgj","", wPAMC_AllQcdPho50to80);
    tgjMC->addFile(fnamePAMC_AllQcdPho80to120,    "tgj","", wPAMC_AllQcdPho80to120);
    tgjMC->addFile(fnamePAMC_AllQcdPho120to170,  "tgj","", wPAMC_AllQcdPho120to170);
    tgjMC->addFile(fnamePAMC_AllQcdPho170to9999,  "tgj","", wPAMC_AllQcdPho170to9999);
  }
  else if (collision==kHIDATA)  {
    tgjMC->addFile(fnameHIMC_AllQcdPho30to50,     "tgj", "",wHIMC_AllQcdPho30to50);
    tgjMC->addFile(fnameHIMC_AllQcdPho50to80,     "tgj", "",wHIMC_AllQcdPho50to80);
    tgjMC->addFile(fnameHIMC_AllQcdPho80to9999,   "tgj", "",wHIMC_AllQcdPho80to9999);
  }
  else {
    cout << " Error: getPurity.  check the type of the collision!  " << endl;
    fitResult fitr0;
    return fitr0;
  }

  TH1D* hCand = new TH1D("cand","",25,0,0.025);
  TH1D* hBkg = (TH1D*)hCand->Clone("bkg");  TH1D* hSig = (TH1D*)hCand->Clone("sig");

  tgj->Draw2(   hCand, "sigmaIetaIeta", evtSeltCut , "");
  tgj->Draw2(   hBkg, "sigmaIetaIeta", sbEvtCut , "");
  tgjMC->Draw2( hSig, "sigmaIetaIeta", evtSeltCut && "genIso<5 && abs(genMomId)<=22", "");
  handsomeTH1(hCand,1);
  handsomeTH1(hSig,2);
  handsomeTH1(hBkg,4);


  TCanvas* cPurity = new TCanvas("cpurity","",500,500);
  fitResult  fitr = doFit ( hSig, hBkg, hCand, 0.005, 0.025);
  drawText(Form("Purity : %.2f", (float)fitr.purity010), 0.5680963,0.429118);
  drawText(Form("p_{T}^{#gamma}: %d-%d GeV", (int)photonPtThr, (int)photonPtThrUp), 0.568,0.529118);
  cPurity->SaveAs( Form("%s.pdf",canvasName.Data() ) );
  gPad->SetLogy();
  cPurity->SaveAs( Form("%s_logScale.pdf",canvasName.Data() ) );

  TCanvas* c1 = new TCanvas("c1","",100,100);


  delete tgj;
  delete tgjMC;
  delete hSig;
  delete hBkg;
  delete hCand;

  return fitr;


}

int main(int argc, char *argv[])
{
  if(argc == 6)
  {
    gammaJetHistProducer((sampleType) atoi(argv[1]),
			 atof(argv[2]),
			 atof(argv[3]),
			 atof(argv[4]),
			 atoi(argv[5]));
    return 0;
  } else {
    std::cout << "Wrong number of arguments" << std::endl;
    return 1;
  }
}
