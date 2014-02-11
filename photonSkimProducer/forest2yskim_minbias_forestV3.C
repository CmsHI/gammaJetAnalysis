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
#include "../../HiForestAnalysisPostHP/hiForest.h"
#include "../CutAndBinCollection2012.h"
#include <time.h>
#include <TRandom3.h>

using namespace std;

static const long MAXTREESIZE = 10000000000;





void forest2yskim_minbias_forestV3(TString inputFile_="forestFiles/HiForest4/HiForest_PbPb_MinBias_Track8_Jet5_GR_R_53_LV2B_merged_forest_0.root",
				   sampleType colli=kHIDATA,
				   int maxEvent = -1,
				   bool useGenJetColl = 0,
				   TString jetAlgo="akPu3PF",
				   float trkJetRadius = 0.3
				   )
{ 
  
  bool isMC=true;
  if ((colli==kPPDATA)||(colli==kPADATA)||(colli==kHIDATA))
    isMC=false;
  
   int seconds = time(NULL);  
  cout << " time = " <<seconds%10000<< endl;
  TRandom3 rand(seconds%10000);

  TString sampleString = "kPPDATA";
  if (colli==kPADATA) sampleString = "kPADATA";
  if (colli==kHIDATA) sampleString = "kHIDATA";
  if (colli==kPPMC) sampleString = "kPPMC";
  if (colli==kPAMC) sampleString = "kPAMC";
  if (colli==kHIMC) sampleString = "kHIMC";


  TString outname =  inputFile_(0, inputFile_.Last('/')+1) +  "skim_collId_"+ sampleString + "_jetAlgo_"+jetAlgo+"_"+inputFile_(inputFile_.Last('/')+1,200);

  
  
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

  c->InitTree();

  
  // output file
  TFile* newfile_data = new TFile(outname,"recreate");
   
  // Track tree retrieved on Feb 11 2014
  int nTrk;
  static const int MAXTRK  = 5000;   // This must be  enough.
  float trkPt[MAXTRK];
  float trkEta[MAXTRK];
  float trkPhi[MAXTRK];
  float trkAsJetPt[MAXTRK];  // associated Jet pT 
  float trkAsJetEta[MAXTRK];  // associated Jet pT 
  float trkAsJetPhi[MAXTRK];  // associated Jet pT 
  float trkAsJetDR[MAXTRK];  // associated Jet pT 

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
      newtreeTrkJet[icent][ivz] = new TTree(Form("trkAndJets_first_icent%d_ivz%d",icent,ivz),"track and jets");
      newtreeTrkJet[icent][ivz]->SetMaxTreeSize(MAXTREESIZE);
      newtreeTrkJet[icent][ivz]->Branch("evt",&evt.run,"run/I:evt:cBin:pBin:vz/F:vtxCentWeight/F:hf4Pos:hf4Neg:hf4Sum");

      newtreeTrkJet[icent][ivz]->Branch("nTrk",&nTrk,"nTrk/I");
      newtreeTrkJet[icent][ivz]->Branch("trkPt",trkPt,"trkPt[nTrk]/F");
      newtreeTrkJet[icent][ivz]->Branch("trkEta",trkEta,"trkEta[nTrk]/F");
      newtreeTrkJet[icent][ivz]->Branch("trkPhi",trkPhi,"trkPhi[nTrk]/F");
      newtreeTrkJet[icent][ivz]->Branch("trkAsJetPt",trkAsJetPt,"trkAsJetPt[nTrk]/F");
      newtreeTrkJet[icent][ivz]->Branch("trkAsJetEta",trkAsJetEta,"trkAsJetEta[nTrk]/F");
      newtreeTrkJet[icent][ivz]->Branch("trkAsJetPhi",trkAsJetPhi,"trkAsJetPhi[nTrk]/F");
      newtreeTrkJet[icent][ivz]->Branch("trkAsJetDR",trkAsJetDR,"trkAsJetDR[nTrk]/F");

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
  if ( jetAlgo == "akPu3PF")  {
    theJet = &(c->akPu3PF) ;   cout << "Using akPu3PF Jet Algo" << endl<<endl;
  }
  else if ( jetAlgo == "akVs3PF") {
    theJet = &(c->akVs3PF) ;   cout << "Using ak3PF Jet Algo, Voronoi Subtraction method" << endl<<endl;
  } 

  
  /// LOOP!!
  int nentries = c->GetEntries();
  if ( maxEvent > 0 ) 
    nentries = maxEvent;
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
    //   evt.trig = 0; // dummy variables are removed 
    //   evt.offlSel = (c->skim.pcollisionEventSelection > 0);
    //   evt.noiseFilt = (c->skim.pHBHENoiseFilter > 0);
    //   evt.anaEvtSel = c->selectEvent() && evt.trig;
    evt.vz = c->evt.vz;
    
    //   if ( c->selectEvent() == 0 )
    //   continue;
    
    // vertex bin and cut!! 
    
    int vzBin = hvz->FindBin(evt.vz)  ;
    hvz->Fill(evt.vz)  ;
    if ( (vzBin<1) || ( vzBin > nVtxBin) ) 
      continue;
    
    //////////////////////////////////////////// No track collection at the moment 
    nTrk = 0; 
    for (int it=0; it < c->track.nTrk; it++ ) { 
      if ( c->track.trkPt[it] < cuttrkPtSkim )   continue;
      if (  fabs(c->track.trkEta[it]) > cuttrkEtaSkim ) continue;
      //      if ( c->track.highPurity[it] == 0 )   continue;  // No id cut yet
      trkPt[nTrk]  = c->track.trkPt[it];
      trkEta[nTrk] = c->track.trkEta[it];
      trkPhi[nTrk] = c->track.trkPhi[it]; 
      //  trkWeight[nTrk] = c->getTrackCorrection(it);
      int assocJetId = matchedJetFinder( theJet, trkEta[nTrk], trkPhi[nTrk], trkJetRadius); // Works only for reco jets
      if ( assocJetId < 0 )  {
	trkAsJetPt[nTrk] = -1; 
	trkAsJetEta[nTrk] = -1; 
	trkAsJetPhi[nTrk] = -1; 
	trkAsJetDR[nTrk] = 100; 
      } 
      else { 
	trkAsJetPt[nTrk] = theJet->jtpt[assocJetId];
	trkAsJetEta[nTrk] = theJet->jteta[assocJetId];
	trkAsJetPhi[nTrk] = theJet->jtphi[assocJetId];
	trkAsJetDR[nTrk] =getDR( trkEta[nTrk], trkPhi[nTrk], theJet->jteta[assocJetId], theJet->jtphi[assocJetId]) ;
      }
      
      nTrk++;
    }
    

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
}



