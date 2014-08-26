// Author Alex Barbieri
#include "TFile.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TH1D.h"
#include "TProfile.h"
#include "TGraphAsymmErrors.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TString.h"
#include "stdio.h"
#include "TRandom3.h"
#include "../HiForestAnalysis/hiForest.h"
#include "alexGammaSkim.h"
#include "CutAndBinCollection2012.h"

const Double_t gammaEtaCut = 1.44;
const Double_t jetEtaCut = 1.6;

// stuff related to MB mixing
//const int nCentBinSkim = 40;
//const int nVtxBin = 1;
const TString MinbiasFname = "photonSkimProducer/minbiasSkim_HIMinBias_merged_1_1000.root";
const int vzCut = 15;
//const int nMixing1 = 20;

collisionType getCType(sampleType sType);

int makeGammaJetNTuple(const char *inFile, sampleType sType, const char *outName, Float_t mcweight=1)
{
  bool montecarlo = false;
  if(sType == kPPMC || sType == kPAMC || sType == kHIMC)
    montecarlo = true;

  collisionType cType = getCType(sType);

  TFile *outfile = new TFile(outName,"RECREATE");

  // prepares photonTree, jetTree, and mJetTree
  GammaJetSkim s(montecarlo); //see alexGammaSkim.h

  HiForest *c = new HiForest(inFile, "Forest", cType, montecarlo);
  c->InitTree(); //could be redundant, but doesn't hurt
  if( cType == cPbPb )
    c->GetEnergyScaleTable((char*)"photonEnergyScaleTable_lowPt_v6.root");

  c->LoadNoTrees(); // turns off trees I'm not using: loops faster
  c->hasPhotonTree = true;
  c->hasSkimTree = true; // required for selectEvent()
  c->hasEvtTree = true;


  // if(sType == kHIDATA || sType == kHIMC || sType == kPADATA || sType == kPAMC)
  // {
  //   c->hasAkPu3JetTree = true;
  // }
  // else //pp
  // {
  //   c->hasAk3JetTree = true;
  // }
  c->hasAkPu3JetTree = true; //pp currently only has PU

  // mix with minbias jets for PbPb only.
  //////////////////////////////////////
  int seconds = time(NULL);
  TRandom3 rand(seconds%10000);
  Int_t           nJetImb;
  Float_t         jetPtImb[s.MAXJETS];
  Float_t         jetEtaImb[s.MAXJETS];
  Float_t         jetPhiImb[s.MAXJETS];

  int nCentBins =  nCentBinSkim;
  TChain *tjmb[100][nVtxBin+1];
  Long64_t nMB[100][nVtxBin+1] ;
  Long64_t mbItr[100][nVtxBin+1];
  if ( sType==kHIDATA ) {
    for( int icent = 0 ; icent< nCentBins ; icent++) {
      for( int ivz = 1 ; ivz<=nVtxBin ; ivz++) {
  	tjmb[icent][ivz] = new TChain(Form("trkAndJets_first_cBin2icent%d_ivz%d",icent,ivz));
  	tjmb[icent][ivz]->Add(MinbiasFname.Data());
  	tjmb[icent][ivz]->SetBranchAddress("nJet",   &nJetImb);
  	tjmb[icent][ivz]->SetBranchAddress("jetPt",  jetPtImb);
	tjmb[icent][ivz]->SetBranchAddress("jetEta", jetEtaImb);
	tjmb[icent][ivz]->SetBranchAddress("jetPhi", jetPhiImb);

  	nMB[icent][ivz] = tjmb[icent][ivz]->GetEntries();
	mbItr[icent][ivz] = rand.Integer(nMB[icent][ivz]);
      }
    }
  }
  ///////////////////////////////////////////////

  //loop over events in each file
  Long64_t nentries = c->GetEntries();
  for(Long64_t jentry = 0; jentry<nentries; ++jentry)
  {
    if (jentry % 1000 == 0)  {
      printf("%lld / %lld\n",jentry,nentries);
    }

    c->GetEntry(jentry);

    //event selection
    if( !c->selectEvent() )
      continue;

    if( TMath::Abs(c->evt.vz) > vzCut )
      continue;

    if(c->photon.nPhotons == 0)
      continue;

    //loop over photons in the event
    //////////////////////////////////////////
    Float_t leadingCorrectedPt = 0;
    Int_t leadingIndex = -1;
    for(Int_t i = 0; i < c->photon.nPhotons; ++i)
    {
      if(
	(TMath::Abs(c->photon.eta[i]) > gammaEtaCut) ||
	(c->isSpike(i)) ||
	(!(c->isLoosePhoton(i)))
	)
	continue;

      Float_t correctedPt = c->getCorrEt(i);
      if(correctedPt > leadingCorrectedPt)
      {
	leadingCorrectedPt = correctedPt;
	leadingIndex = i;
      }
    }

    if(leadingIndex == -1)
      continue;

    s.run = c->evt.run;
    s.event = c->photon.event;
    s.lumi = c->evt.lumi;
    s.gPt = c->photon.pt[leadingIndex];
    s.corrGPt = leadingCorrectedPt;
    s.gEta = c->photon.eta[leadingIndex];
    s.gPhi = c->photon.phi[leadingIndex];
    s.HF = c->evt.hiHF;
    s.HFplusEta4 = c->evt.hiHFplusEta4;
    s.HFminusEta4 = c->evt.hiHFminusEta4;
    s.cc4 = c->photon.cc4[leadingIndex];
    s.cr4 = c->photon.cr4[leadingIndex];
    s.ct4PtCut20 = c->photon.ct4PtCut20[leadingIndex];
    s.hadronicOverEm = c->photon.hadronicOverEm[leadingIndex];
    s.sigmaIetaIeta = c->photon.sigmaIetaIeta[leadingIndex];
    s.r9 = c->photon.r9[leadingIndex];
    s.ecalRecHitSumEtConeDR04 = c->photon.ecalRecHitSumEtConeDR04[leadingIndex];
    s.hcalTowerSumEtConeDR04 = c->photon.hcalTowerSumEtConeDR04[leadingIndex];
    s.trkSumPtHollowConeDR04 = c->photon.trkSumPtHollowConeDR04[leadingIndex];
    s.hiBin = c->evt.hiBin;
    s.vz = c->evt.vz;

    if(montecarlo)
    {
      s.genMomId = c->photon.genMomId[leadingIndex];
      s.genCalIsoDR04 = c->photon.genCalIsoDR04[leadingIndex];
      s.genTrkIsoDR04 = c->photon.genTrkIsoDR04[leadingIndex];
      s.ptHat = c->photon.ptHat;
      s.genGPt = c->photon.genMatchedPt[leadingIndex];
      s.genGEta = c->photon.genMatchedEta[leadingIndex];
      s.genGPhi = c->photon.genMatchedPhi[leadingIndex];
      s.mcweight = mcweight;
    }
    //////////////////////////////

    //loop over 'away' jets
    ///////////////////////////////////////////
    s.nJets = 0;
    Jets jetCollection;
    // if(sType == kHIDATA || sType == kHIMC || sType == kPADATA || sType == kPAMC)
    // {
    //   jetCollection = c->akPu3PF;
    // }
    // else //pp
    // {
    //   jetCollection = c->ak3PF;
    // }
    jetCollection = c->akPu3PF; //pp mc currently missing ak3PF

    for(Int_t i = 0; i<jetCollection.nref; ++i)
    {
      if( TMath::Abs(jetCollection.jteta[i]) > jetEtaCut)
	continue;

      Float_t deltaPhi = TMath::ACos(TMath::Cos(c->photon.phi[leadingIndex]
						- jetCollection.jtphi[i]));
      Float_t deltaEta = TMath::Abs(c->photon.eta[leadingIndex]
				    - jetCollection.jteta[i]);
      Float_t deltaR = TMath::Sqrt(deltaPhi*deltaPhi + deltaEta*deltaEta);

      if(deltaR < 0.3) continue; // avoid finding the photon itself

      Float_t averageEta = (c->photon.eta[leadingIndex] + jetCollection.jteta[i])/2.0;

      s.jPt[s.nJets] = jetCollection.jtpt[i];
      s.jEta[s.nJets] = jetCollection.jteta[i];
      s.jPhi[s.nJets] = jetCollection.jtphi[i];
      s.avgEta[s.nJets] = averageEta;
      s.dPhi[s.nJets] = deltaPhi;
      s.dR[s.nJets] = deltaR;

      if(montecarlo)
	s.genJPt[i] = jetCollection.refpt[i];

      s.nJets++;
      if(s.nJets > s.MAXJETS-1)
      {
	printf("ERROR: Jet arrays not large enough.\n");
	return(1);
      }
    }
    //////////////////////////////////////////////

    // for kHIDATA, mix with minbias jets
    ///////////////////////////////////////////////
    s.nMjet = 0;
    //if(sType == kHIDATA)
    if(false)
    {
      int nMixing = nMixing1;
      s.nMjet = 0;
      int cBin = c->evt.hiBin;
      int vzBin = 1;

      for(int k = 0; k < nMixing; k++)
      {
    	mbItr[cBin][vzBin] = mbItr[cBin][vzBin] + 1;
    	if ( mbItr[cBin][vzBin] >= nMB[cBin][vzBin] )
	{
	  mbItr[cBin][vzBin] = 0;
	}

    	/// Load the minbias event
	//printf("cBin: %d, mbItr: %lld.\n",cBin,mbItr[cBin][vzBin]);
    	tjmb[cBin][vzBin]->GetEntry(mbItr[cBin][vzBin]);

    	// Jet mixing
    	for (int it = 0 ; it < nJetImb ; it++) {
    	  // if ( jetPtImb[it] < cutjetPtSkim )
    	  //   continue;
    	  if ( TMath::Abs( jetEtaImb[it] ) > jetEtaCut )
    	    continue;

    	  s.mJetPt[s.nMjet]    = jetPtImb[it];
    	  s.mJetEta[s.nMjet]   = jetEtaImb[it];
    	  s.mJetPhi[s.nMjet]   = jetPhiImb[it];
	  s.mJetDphi[s.nMjet]  =  TMath::ACos(TMath::Cos(c->photon.phi[leadingIndex]
						       - jetEtaImb[it]));
    	  s.nMjet++;
	  if(s.nMjet > s.MAXJETS-1)
	  {
	    printf("ERROR: Mixed Jet arrays not large enough.\n");
	    return(1);
	  }
    	}
      }
    }
    //////////////////////////////////////////////

    s.Fill();
  }

  outfile->cd();
  s.Write();
  outfile->Close();

  printf("Done.\n");
  return(0);
}

collisionType getCType(sampleType sType)
{
  switch (sType)
  {
  case kPPDATA:
  case kPPMC:
    return cPP;
  case kPADATA:
  case kPAMC:
    return cPPb;
  case kHIDATA:
  case kHIMC:
    return cPbPb;
  }
  return cPbPb; //probably a bad guess
}

int main(int argc, char *argv[])
{
  if(argc != 4 && argc != 5)
  {
    printf("Usage: makeGammaJetNTuple.exe \"inForest\" sampleType \"outname\" mcweight");
    return(1);
  }
  if(argc == 4)
    return(makeGammaJetNTuple(argv[1], (sampleType)atoi(argv[2]), argv[3]));
  else
    return(makeGammaJetNTuple(argv[1], (sampleType)atoi(argv[2]), argv[3], atof(argv[4])));
}
