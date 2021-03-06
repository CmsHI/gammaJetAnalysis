///////////////////////////////////////////////////////////////////
// forest2yskim.C                                                //
// Creator : Yongsun Kim (MIT), jazzitup@mit.edu                 //
// Function : Transform hiForest files into yskim file           //
// yskims for MinBias1, Minbias2 and photon jet skims            //
///////////////////////////////////////////////////////////////////

#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TTree.h>
#include <TFile.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <TMath.h>
#include "../../HiForestAnalysis/hiForest.h"
#include "../CutAndBinCollection2012.h"
#include <time.h>
#include <TRandom3.h>

using namespace std;

static const long MAXTREESIZE = 10000000000;





void forest2yskim_minbias_forestV3(TString inputFile_="mergedFiles/forest_minbias-HYDJET-START44-V12-Aug29th.root",
				   std::string outname = "skim_trackJet_minbiasTrackJet_mc.root",
				   sampleType colli=kPADATA,
				   int maxEvent = -1,
				   bool useGenJetColl = 0
				   )
{

  bool isMC=true;
  if ((colli==kPPDATA)||(colli==kPADATA)||(colli==kHIDATA))
    isMC=false;
  TString jetAlgo="akPu3PF";

  int seconds = time(NULL);         cout << " time = " <<seconds%10000<< endl;
  TRandom3 rand(seconds%10000);
  TString datafname  = "";


  HiForest *c;
  if((colli==kPADATA)||(colli==kPAMC)) {
    c = new HiForest(inputFile_.Data(), "forest", cPPb, isMC );
  }
  else if  ((colli==kPPDATA)||(colli==kPPMC)) {
    c = new HiForest(inputFile_.Data(), "forest", cPP, isMC );
  }
  else if  ((colli==kHIDATA)||(colli==kHIMC)) {
    c = new HiForest(inputFile_.Data(), "forest", cPbPb, isMC );
  }
  else {
    cout << " Error!  No such collision type" << endl;
    return;
  }
  c->LoadNoTrees();
  c->hasEvtTree = true;
  c->hasPhotonTree = true;
  c->hasSkimTree = true;
  c->hasAkPu3JetTree = true;
  c->hasAk3JetTree = true;
  c->InitTree();


  // output file
  TFile* newfile_data = new TFile(outname.data(),"recreate");

  // The track tree removed at the moment

  // Jet tree
  int nJet;
  static const int MAXJET  = 200;   // This must be  enough.
  float jetPt[MAXJET];
  float jetEta[MAXJET];
  float jetPhi[MAXJET];
  int jetSubid[MAXJET];
  float jetRefPt[MAXJET];
  float jetRefEta[MAXJET];
  float jetRefPhi[MAXJET];
  float jetRefPartonPt[MAXJET];
  int  jetRefPartonFlv[MAXJET];

  EvtSel evt;
  TTree* newtreeTrkJet[100][nVtxBin+1];

  int nCentBins =  nCentBinSkim;
  if ((colli==kPADATA)||(colli==kPAMC)) {
    nCentBins = nCentBinSkimPA;
  }


  for( int icent = 0 ; icent< nCentBins ; icent++) {
    for( int ivz = 1 ; ivz<=nVtxBin ; ivz++) {
      newtreeTrkJet[icent][ivz] = new TTree(Form("trkAndJets_first_cBin2icent%d_ivz%d",icent,ivz),"track and jets");
      newtreeTrkJet[icent][ivz]->SetMaxTreeSize(MAXTREESIZE);
      newtreeTrkJet[icent][ivz]->Branch("evt",&evt.run,"run/I:evt:cBin:pBin:trig/O:offlSel:noiseFilt:anaEvtSel:vz/F:reweight/F:hf4Pos:hf4Neg:hf4Sum");

      newtreeTrkJet[icent][ivz]->Branch("nJet",&nJet,"nJet/I");
      newtreeTrkJet[icent][ivz]->Branch("jetPt",jetPt,"jetPt[nJet]/F");
      newtreeTrkJet[icent][ivz]->Branch("jetEta",jetEta,"jetEta[nJet]/F");
      newtreeTrkJet[icent][ivz]->Branch("jetPhi",jetPhi,"jetPhi[nJet]/F");
      if ( isMC )  {
	newtreeTrkJet[icent][ivz]->Branch("subid",jetSubid,"subid[nJet]/I");
	newtreeTrkJet[icent][ivz]->Branch("refPt",jetRefPt,"refPt[nJet]/F");
	newtreeTrkJet[icent][ivz]->Branch("refEta",jetRefEta,"refEta[nJet]/F");
	newtreeTrkJet[icent][ivz]->Branch("refPhi",jetRefPhi,"refPhi[nJet]/F");
	newtreeTrkJet[icent][ivz]->Branch("refPartonPt",jetRefPartonPt,"refPartonPt[nJet]/F");
	newtreeTrkJet[icent][ivz]->Branch("refPartonFlv",jetRefPartonFlv,"refPartonFlv[nJet]/I");
      }
    }
  }

  // vertex histogram
  float vzCut = vtxCutPhotonAna;
  TH1F* hvz = new TH1F("hvz","",nVtxBin,-vzCut,vzCut);
  // event plane hitogram
  TH1F* hEvtPlnBin = new TH1F("hEvtPlnBin", "", nPlnBin, -PI/2., PI/2.);
  // jet algos
  Jets* theJet;
  theJet = &(c->akPu3PF) ;   cout << "Using akPu3PF Jet Algo" << endl<<endl;

  /// LOOP!!
  int nentries = c->GetEntries();
  if ( maxEvent > 0 )
    nentries = maxEvent;
  int nSelEvt = 0;
  cout << "number of entries = " << nentries << endl;

  for (Long64_t jentry=0 ; jentry<nentries;jentry++) {
    if (jentry% 1000 == 0)  {
      cout <<jentry<<" / "<<nentries<<" "<<setprecision(2)<<(double)jentry/nentries*100<<endl;
    }
    c->GetEntry(jentry);
    evt.clear();
    evt.run   = c->evt.run;
    evt.evt = c->evt.evt;
    evt.hf4Pos = c->evt.hiHFplusEta4;
    evt.hf4Neg = c->evt.hiHFminusEta4;
    evt.hf4Sum = evt.hf4Pos + evt.hf4Neg;
    evt.cBin = -99;
    evt.pBin   = -99 ;
    if ((colli==kHIDATA)||(colli==kHIMC))   {
      evt.cBin = c->evt.hiBin;
      evt.pBin   = hEvtPlnBin->FindBin( c->evt.hiEvtPlanes[theEvtPlNumber] ) ;
    }
    else if ((colli==kPADATA)||(colli==kPAMC))   {
      evt.cBin =  getHfBin(evt.hf4Sum);
      if (  ((evt.cBin) < 0) || (evt.cBin) >= nCentBinSkimPA )
	cout << " Check the pA centrality..  cbin = " << evt.cBin << endl;
    }

    evt.vtxCentWeight = 1;
    evt.trig = 0;
    evt.offlSel = (c->skim.pcollisionEventSelection > 0);
    evt.noiseFilt = (c->skim.pHBHENoiseFilter > 0);
    evt.anaEvtSel = c->selectEvent() && evt.trig;
    evt.vz = c->evt.vz;
    if ( c->selectEvent() == 0 )
      continue;
    nSelEvt++;
    // vertex bin and cut!!

    int vzBin = hvz->FindBin(evt.vz)  ;
    hvz->Fill(evt.vz)  ;
    if ( (vzBin<1) || ( vzBin > nVtxBin) )
      continue;

    //////////////////////////////////////////// No track collection at the moment
    /*
      nTrk = 0;
      for (int it=0; it < c->track.nTrk; it++ ) {
      //if ( c->track.trkPt[it] < trkBoundary )   continue;
      if ( c->track.highPurity[it] == 0 )
      continue;   // only high purity high pt tracks and all pixel track
      trkPt[nTrk]  = c->track.trkPt[it];
      trkEta[nTrk] = c->track.trkEta[it];
      trkPhi[nTrk] = c->track.trkPhi[it];
      if ( trkPt[nTrk] < cuttrkPt )
      continue;
      if ( fabs(trkEta[nTrk]) > cuttrkEtaSkim )
      continue;
      trkWeight[nTrk] = c->getTrackCorrection(it);
      nTrk++;
    }
    */

    ///////////// Collection of jets
    nJet = 0 ;

    int jetEntries = 0;
    if (useGenJetColl )    jetEntries = theJet->ngen;
    else                   jetEntries = theJet->nref;

    for (int ij=0; ij< jetEntries ; ij++) {
      if (  useGenJetColl )   {
	jetPt[nJet] = theJet->genpt[ij];
	jetEta[nJet] = theJet->geneta[ij];
	jetPhi[nJet] = theJet->genphi[ij];
      }
      else  {
	jetPt[nJet] = theJet->jtpt[ij];
	jetEta[nJet] = theJet->jteta[ij];
	jetPhi[nJet] = theJet->jtphi[ij];
      }


      if ( jetPt[nJet] < cutjetPtSkim)
	continue;
      if ( fabs( jetEta[nJet] ) > cutjetEtaSkim )
        continue;

      if ( (colli==kPADATA) && ( evt.run > 211256 ) )  {
        jetEta[nJet] = -jetEta[nJet] ;
        //      cout << " reflect eta" << endl;
      }

      if (  useGenJetColl )   {
	jetSubid[nJet] = -9999;
	jetRefPt[nJet] = -9999;
	jetRefEta[nJet] = -9999;
	jetRefPhi[nJet] = -9999;
	jetRefPartonPt[nJet] = -9999;
	jetRefPartonFlv[nJet] = -9999;
      }
      else {
	jetSubid[nJet] = theJet->subid[ij];
	jetRefPt[nJet] = theJet->refpt[ij];
	jetRefEta[nJet] = theJet->refeta[ij];
	jetRefPhi[nJet] = theJet->refphi[ij];
	jetRefPartonPt[nJet] = theJet->refparton_pt[ij];
	jetRefPartonFlv[nJet] = theJet->refparton_flavor[ij];
      }

      nJet++ ;
    }


    /// Gen Particle   commented out at the moment   2013 Aug. 25th
    /*
      nGp = 0;
      if ( isMC ) {
      for ( int ig = 0 ; ig < c->genparticle.mult ; ig++) {
      if ( c->genparticle.pt[ig] < cuttrkPt )
      continue;
      if ( fabs( c->genparticle.eta[ig] ) > cuttrkEtaSkim )
      continue;
      if ( c->genparticle.chg[ig] == 0 )
      continue;
      gpChg[nGp] =  c->genparticle.chg[ig] ;
      gpPt[nGp]  = c->genparticle.pt[ig];
      gpEta[nGp] = c->genparticle.eta[ig];
      gpPhi[nGp] = c->genparticle.phi[ig];
      nGp++;
      }
      }
    */

    newtreeTrkJet[evt.cBin][vzBin]->Fill();


  }





  newfile_data->Write();
  cout << " Done! "<< endl;
  cout << nSelEvt << " out of " << nentries << " events were selected " << endl;
}

int main(int argc, char *argv[])
{
  if(argc == 6)
  {
    forest2yskim_minbias_forestV3(argv[1],
				  argv[2],
				  (sampleType) atoi(argv[3]),
				  atoi(argv[4]),
				  (bool) atoi(argv[5])
      );
    return 0;
  } else if (argc == 5)
  {
    forest2yskim_minbias_forestV3(argv[1],
				  argv[2],
				  (sampleType) atoi(argv[3]),
				  atoi(argv[4])
      );
    return 0;
  } else {
    return 1;
  }
}
