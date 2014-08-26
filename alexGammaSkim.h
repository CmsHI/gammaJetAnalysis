// Author: Alex Barbieri
#ifndef GammaJetSkim_h
#define GammaJetSkim_h

#include "TTree.h"
#include "TFile.h"

class GammaJetSkim {
public:
  TTree *photonTree;
  TTree *jetTree;
  TTree *mJetTree;

// photon tree
  Int_t run;
  Int_t event;
  Int_t lumi;
  Float_t gPt;
  Float_t corrGPt;
  Float_t gEta;
  Float_t gPhi;
  Float_t HF;
  Float_t HFplusEta4;
  Float_t HFminusEta4;
  Float_t cc4;
  Float_t cr4;
  Float_t ct4PtCut20;
  Float_t hadronicOverEm;
  Float_t sigmaIetaIeta;
  Float_t r9;
  Float_t ecalRecHitSumEtConeDR04;
  Float_t hcalTowerSumEtConeDR04;
  Float_t trkSumPtHollowConeDR04;
  Int_t hiBin;
  Float_t vz;


//jet tree
  Int_t nJets;
  static const int MAXJETS = 504; //got from SetupJetTree.h
  Float_t jPt[MAXJETS];
  Float_t jEta[MAXJETS];
  Float_t jPhi[MAXJETS];
  Float_t avgEta[MAXJETS];
  Float_t dPhi[MAXJETS];
  Float_t dR[MAXJETS];

// MC elements
  Int_t genMomId;
  Float_t genCalIsoDR04;
  Float_t genTrkIsoDR04;
  Float_t ptHat;
  Float_t genGPt;
  Float_t genGEta;
  Float_t genGPhi;
  Float_t mcweight;

  Float_t genJPt[MAXJETS];

// minbias mix elements
  int nMjet;
  float mJetPt[MAXJETS];
  float mJetEta[MAXJETS];
  float mJetPhi[MAXJETS];
  float mJetDphi[MAXJETS];

private:
  void setBranches(bool montecarlo)
  {
    photonTree->Branch("run",&run,"run/I");
    photonTree->Branch("event",&event,"event/I");
    photonTree->Branch("lumi",&lumi,"lumi/I");
    photonTree->Branch("pt",&gPt,"pt/F");
    photonTree->Branch("corrPt",&corrGPt,"corrPt/F");
    photonTree->Branch("eta",&gEta,"eta/F");
    photonTree->Branch("phi",&gPhi,"phi/F");
    photonTree->Branch("HF",&HF,"HF/F");
    photonTree->Branch("HFplusEta4",&HFplusEta4,"HFplusEta4/F");
    photonTree->Branch("HFminusEta4",&HFminusEta4,"HFminusEta4/F");
    photonTree->Branch("cc4",&cc4,"cc4/F");
    photonTree->Branch("cr4",&cr4,"cr4/F");
    photonTree->Branch("ct4PtCut20",&ct4PtCut20,"ct4PtCut20/F");
    photonTree->Branch("hadronicOverEm",&hadronicOverEm,"hadronicOverEm/F");
    photonTree->Branch("sigmaIetaIeta",&sigmaIetaIeta,"sigmaIetaIeta/F");
    photonTree->Branch("r9",&r9,"r9/F");
    photonTree->Branch("ecalRecHitSumEtConeDR04",&ecalRecHitSumEtConeDR04,"ecalRecHitSumEtConeDR04/F");
    photonTree->Branch("hcalTowerSumEtConeDR04",&hcalTowerSumEtConeDR04,"hcalTowerSumEtConeDR04/F");
    photonTree->Branch("trkSumPtHollowConeDR04",&trkSumPtHollowConeDR04,"trkSumPtHollowConeDR04/F");
    photonTree->Branch("hiBin",&hiBin,"hiBin/I");
    photonTree->Branch("vz",&vz,"vz/F");

    jetTree->Branch("nJets",&nJets,"nJets/I");
    jetTree->Branch("pt",jPt,"jPt[nJets]/F");
    jetTree->Branch("eta",jEta,"eta[nJets]/F");
    jetTree->Branch("phi",jPhi,"phi[nJets]/F");
    jetTree->Branch("avgEta",avgEta,"avgEta[nJets]/F");
    jetTree->Branch("dPhi",dPhi,"dPhi[nJets]/F");
    jetTree->Branch("dR",dR,"dR[nJets]/F");

    if(montecarlo)
    {
      photonTree->Branch("genMomId",&genMomId,"genMomId/I");
      photonTree->Branch("genCalIsoDR04",&genCalIsoDR04,"genCalIsoDR04/F");
      photonTree->Branch("genTrkIsoDR04",&genTrkIsoDR04,"genTrkIsoDR04/F");
      photonTree->Branch("ptHat",&ptHat,"ptHat/F");
      photonTree->Branch("genPt",&genGPt,"genPt/F");
      photonTree->Branch("genEta",&genGEta,"genEta/F");
      photonTree->Branch("genPhi",&genGPhi,"genPhi/F");
      photonTree->Branch("mcweight",&mcweight,"mcweight/F");

      jetTree->Branch("genPt",genJPt,"genJPt[nJets]/F");
    }

    mJetTree->Branch("nJet",&nMjet,"nJet/I");
    mJetTree->Branch("pt",mJetPt,"pt[nJet]/F");
    mJetTree->Branch("eta",mJetEta,"eta[nJet]/F");
    mJetTree->Branch("phi",mJetPhi,"phi[nJet]/F");
    mJetTree->Branch("dPhi", mJetDphi, "dphi[nJet]/F");
  }

  void getBranches(bool montecarlo)
  {
    photonTree->SetBranchAddress("run",&run);
    photonTree->SetBranchAddress("event",&event);
    photonTree->SetBranchAddress("lumi",&lumi);
    photonTree->SetBranchAddress("pt",&gPt);
    photonTree->SetBranchAddress("corrPt",&corrGPt);
    photonTree->SetBranchAddress("eta",&gEta);
    photonTree->SetBranchAddress("phi",&gPhi);
    photonTree->SetBranchAddress("HF",&HF);
    photonTree->SetBranchAddress("HFplusEta4",&HFplusEta4);
    photonTree->SetBranchAddress("HFminusEta4",&HFminusEta4);
    photonTree->SetBranchAddress("cc4",&cc4);
    photonTree->SetBranchAddress("cr4",&cr4);
    photonTree->SetBranchAddress("ct4PtCut20",&ct4PtCut20);
    photonTree->SetBranchAddress("hadronicOverEm",&hadronicOverEm);
    photonTree->SetBranchAddress("sigmaIetaIeta",&sigmaIetaIeta);
    photonTree->SetBranchAddress("r9",&r9);
    photonTree->SetBranchAddress("ecalRecHitSumEtConeDR04",&ecalRecHitSumEtConeDR04);
    photonTree->SetBranchAddress("hcalTowerSumEtConeDR04",&hcalTowerSumEtConeDR04);
    photonTree->SetBranchAddress("trkSumPtHollowConeDR04",&trkSumPtHollowConeDR04);
    photonTree->SetBranchAddress("hiBin",&hiBin);
    photonTree->SetBranchAddress("vz",&vz);

    jetTree->SetBranchAddress("nJets",&nJets);
    jetTree->SetBranchAddress("pt",jPt);
    jetTree->SetBranchAddress("eta",jEta);
    jetTree->SetBranchAddress("phi",jPhi);
    jetTree->SetBranchAddress("avgEta",avgEta);
    jetTree->SetBranchAddress("dPhi",dPhi);
    jetTree->SetBranchAddress("dR",dR);

    if(montecarlo)
    {
      photonTree->SetBranchAddress("genMomId",&genMomId);
      photonTree->SetBranchAddress("genCalIsoDR04",&genCalIsoDR04);
      photonTree->SetBranchAddress("genTrkIsoDR04",&genTrkIsoDR04);
      photonTree->SetBranchAddress("ptHat",&ptHat);
      photonTree->SetBranchAddress("genPt",&genGPt);
      photonTree->SetBranchAddress("genEta",&genGEta);
      photonTree->SetBranchAddress("genPhi",&genGPhi);
      photonTree->SetBranchAddress("mcweight",&mcweight);

      jetTree->SetBranchAddress("genPt",genJPt);
    }

    mJetTree->SetBranchAddress("nJet",&nMjet);
    mJetTree->SetBranchAddress("pt",mJetPt);
    mJetTree->SetBranchAddress("eta",mJetEta);
    mJetTree->SetBranchAddress("phi",mJetPhi);
    mJetTree->SetBranchAddress("dPhi", mJetDphi);
  }

  void readGammaSkim(TFile *file, bool montecarlo = false)
  {
    photonTree = (TTree*)file->Get("photonTree");
    jetTree = (TTree*)file->Get("jetTree");
    mJetTree = (TTree*)file->Get("mixedJetTree");

    getBranches(montecarlo);
  }

  void initGammaSkim(bool montecarlo = false)
  {
    photonTree = new TTree("photonTree","photonTree");
    jetTree = new TTree("jetTree","jetTree");
    mJetTree = new TTree("mixedJetTree","mixedJetTree");

    setBranches(montecarlo);
  }

public:
  GammaJetSkim(bool mc=false)
  {
    initGammaSkim(mc);
  }
  GammaJetSkim(TFile* infile, bool mc=false)
  {
    readGammaSkim(infile, mc);
  }
  void GetEntry(Long64_t entry)
  {
    photonTree->GetEntry(entry);
    jetTree->GetEntry(entry);
    mJetTree->GetEntry(entry);
  }
  void Fill()
  {
    photonTree->Fill();
    jetTree->Fill();
    mJetTree->Fill();
  }
  void Write()
  {
    photonTree->Write();
    jetTree->Write();
    mJetTree->Write();
  }
};

#endif
