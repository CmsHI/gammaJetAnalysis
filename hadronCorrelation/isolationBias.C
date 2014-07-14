///////////////////////////////////////////////////////////////////                                
// isolationBias.C                                               //
// Creator : Yongsun Kim (KU), kimy@cern.ch                      //
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
#include "../commonUtility.h"

void isolationBias() { 
  TH1::SetDefaultSumw2();

  TFile* f = new TFile("forestFiles/pp/skim_collId_kPPMC_jetAlgo_akPu3PF_20mixed_pp2760_pythia_allQCDPhoton30_CMSSW538HIp2.root");
  TTree* tTrkPP = (TTree*)f->Get("yTrk");
  TTree* tgjPP  = (TTree*)f->Get("tgj");
  tTrkPP->AddFriend("tgj");

  TFile* fpbpb = new TFile("forestFiles/pbpb/skim_collId_kHIMC_jetAlgo_akPu3PF_20mixed_PbPb_pythiaHYDJET_forest_AllQCDPhotons30.root");
  TTree* tTrkHI  = (TTree*)fpbpb->Get("yTrk");
  TTree* tChHI  = (TTree*)fpbpb->Get("ch");
  TTree* tgjHI  = (TTree*)fpbpb->Get("tgj");
  tTrkHI->AddFriend("tgj");
  tChHI->AddFriend("tgj");
  

  int kGen = 0;        
  int kTIso=1;
  int kEIso=2;
  int kHIso=3;
  int kAIso=4; 
  int kSumIso=5;
  
  TCut isoCut[10];
  isoCut[kGen] = "genIso<5";
  isoCut[kEIso] = "ecalIso<4.2";
  isoCut[kHIso] = "hcalIso<2.2";
  isoCut[kTIso] = "trackIso<2";
  isoCut[kAIso] =   isoCut[kEIso] &&   isoCut[kHIso] && isoCut[kTIso]; 
  isoCut[kSumIso] = "sumIso<1";

  
  TH1D* hdphiPP[10];     
  TH1D* hdphiHI[10];     
  TH1D* hdphiHIch[10];     
  for ( int iIso=0; iIso<=kSumIso;iIso++) {
    hdphiPP[iIso] = new TH1D(Form("hdphiPP_iso%d",iIso),";dphi; dN^{ch}/dN{photon}",40,0,3.141592);
    hdphiHI[iIso] = (TH1D*) hdphiPP[iIso]->Clone( Form("hdphiHI_iso%d",iIso) ); 
    hdphiHIch[iIso] = (TH1D*) hdphiPP[iIso]->Clone( Form("hdphiHI_chargedGenParticle_iso%d",iIso) ); 
    TCut photonCut  = isoCut[iIso] && " cBin<20 && photonEt>20 && abs(photonEta)<1.44 && hovere<0.1 && sigmaIetaIeta<0.01";
    TCut trackCut = "pt>0";
    TCut genpCut  = "pt>0 && sube==0";
    
    tTrkPP->Draw( Form("dphi>>%s",hdphiPP[iIso]->GetName()), photonCut && trackCut ) ;
    hdphiPP[iIso]->Scale( 1./ (tgjPP->GetEntries(photonCut)) );
    tTrkHI->Draw( Form("dphi>>%s",hdphiHI[iIso]->GetName()), photonCut && trackCut ) ;
    hdphiHI[iIso]->Scale( 1./ (tgjHI->GetEntries(photonCut)) );
    tChHI->Draw( Form("dphi>>%s",hdphiHIch[iIso]->GetName()), photonCut && genpCut ) ;
    hdphiHIch[iIso]->Scale( 1./ (tgjHI->GetEntries(photonCut)) );
  }



  TCanvas* c1 = new TCanvas("c1","",400,400);
  handsomeTH1(  hdphiPP[kGen],1);
  handsomeTH1(  hdphiPP[kAIso],1);
  handsomeTH1(  hdphiPP[kEIso],6);
  handsomeTH1(  hdphiPP[kHIso],4);
  handsomeTH1(  hdphiPP[kTIso],2);
  hdphiPP[kGen]->Draw("hist");
  hdphiPP[kAIso]->Draw("same e");
  hdphiPP[kEIso]->Draw("same e");
  hdphiPP[kHIso]->Draw("same e");
  hdphiPP[kTIso]->Draw("same e");

  TCanvas* c2 = new TCanvas("c2","",400,400);
  handsomeTH1(  hdphiHI[kGen],1);
  handsomeTH1(  hdphiHI[kSumIso],2);
  cleverRange(hdphiHI[kGen],1.5);
  hdphiHI[kGen]->Draw("hist");
  hdphiHI[kSumIso]->Draw("same e");

  TCanvas* c3 = new TCanvas("c3","",400,400);
  handsomeTH1(  hdphiHIch[kGen],1);
  handsomeTH1(  hdphiHIch[kSumIso],2);
  cleverRange(hdphiHIch[kGen],1.5);
  hdphiHIch[kGen]->Draw("hist");
  hdphiHIch[kSumIso]->Draw("same e");
  
  
  
}

