///////////////////////////////////////////////////////////////////                                
// forest2yskim.C                                                //                                                 
// Creator : Yongsun Kim (MIT), jazzitup@mit.edu                 //                                                 
// Function : Transform hiForest files into yskim file           //
// yskims for MinBias1, Minbias2 and photon jet skims            //
///////////////////////////////////////////////////////////////////         
//d
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
  if  ((colli==kPADATA)||(colli==kPAMC))      c = new HiForest(inputFile_.Data(), "forest", cPPb, isMC);
  else if  ((colli==kPPDATA)||(colli==kPPMC)) c = new HiForest(inputFile_.Data(), "forest", cPP, isMC);
  else if  ((colli==kHIDATA)||(colli==kHIMC)) c = new HiForest(inputFile_.Data(), "forest", cPbPb, isMC);
  else {
    cout << " Error!  No such collision type" << endl;
    return;
  }
  c->InitTree();
  
  // Second forest files for track mixing 
  HiForest *cMix;   
  if ((colli==kHIDATA)||(colli==kHIMC))   
    cMix = new HiForest(inputFile_.Data(), "forest", cPbPb, isMC);
  cMix->InitTree();
  
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

  int nmTrk;
  float mtrkPt[MAXTRK];
  float mtrkEta[MAXTRK];
  float mtrkPhi[MAXTRK];
  float mtrkAsJetPt[MAXTRK];  // associated Jet pT 
  float mtrkAsJetEta[MAXTRK];  // associated Jet pT 
  float mtrkAsJetPhi[MAXTRK];  // associated Jet pT 
  float mtrkAsJetDR[MAXTRK];  // associated Jet pT 

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

      newtreeTrkJet[icent][ivz]->Branch("nTrk",&nTrk,"nTrk/I");
      newtreeTrkJet[icent][ivz]->Branch("trkPt",trkPt,"trkPt[nTrk]/F");
      newtreeTrkJet[icent][ivz]->Branch("trkEta",trkEta,"trkEta[nTrk]/F");
      newtreeTrkJet[icent][ivz]->Branch("trkPhi",trkPhi,"trkPhi[nTrk]/F");
      newtreeTrkJet[icent][ivz]->Branch("trkAsJetPt",trkAsJetPt,"trkAsJetPt[nTrk]/F");
      newtreeTrkJet[icent][ivz]->Branch("trkAsJetEta",trkAsJetEta,"trkAsJetEta[nTrk]/F");
      newtreeTrkJet[icent][ivz]->Branch("trkAsJetPhi",trkAsJetPhi,"trkAsJetPhi[nTrk]/F");
      newtreeTrkJet[icent][ivz]->Branch("trkAsJetDR",trkAsJetDR,"trkAsJetDR[nTrk]/F");

      newtreeTrkJet[icent][ivz]->Branch("nmTrk",&nmTrk,"nmTrk/I");
      newtreeTrkJet[icent][ivz]->Branch("mtrkPt",mtrkPt,"mtrkPt[nmTrk]/F");
      newtreeTrkJet[icent][ivz]->Branch("mtrkEta",mtrkEta,"mtrkEta[nmTrk]/F");
      newtreeTrkJet[icent][ivz]->Branch("mtrkPhi",mtrkPhi,"mtrkPhi[nmTrk]/F");
      newtreeTrkJet[icent][ivz]->Branch("mtrkAsJetPt",mtrkAsJetPt,"mtrkAsJetPt[nmTrk]/F");
      newtreeTrkJet[icent][ivz]->Branch("mtrkAsJetEta",mtrkAsJetEta,"mtrkAsJetEta[nmTrk]/F");
      newtreeTrkJet[icent][ivz]->Branch("mtrkAsJetPhi",mtrkAsJetPhi,"mtrkAsJetPhi[nmTrk]/F");
      newtreeTrkJet[icent][ivz]->Branch("mtrkAsJetDR",mtrkAsJetDR,"mtrkAsJetDR[nmTrk]/F");

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
  //  int nentries = 300;
  if ( maxEvent > 0 ) 
    nentries = maxEvent;
  cout << "number of entries = " << nentries << endl;
    
  for (Long64_t jentry=0 ; jentry<nentries;jentry++) {
    if (jentry% 100 == 0)  {
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
    evt.vz = c->evt.vz;
    
    // vertex bin and cut!! 
    int vzBin = hvz->FindBin(evt.vz)  ;
    hvz->Fill(evt.vz)  ;
    if ( (vzBin<1) || ( vzBin > nVtxBin) ) 
      continue;

    ///////////// Collection of jets  /////////////////////////////////////////////
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
    
    
    ///////////////////////////// Tracks //////////////////////////////
    nTrk = 0; 
    for (int it=0; it < c->track.nTrk; it++ ) { 
      if ( c->track.trkPt[it] < cuttrkPtSkim )   continue;
      if (  fabs(c->track.trkEta[it]) > cuttrkEtaSkim ) continue;
      if ( c->track.highPurity[it] == 0 )   continue;  // highpurity cut by default
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

    ///////////////////////////// background tracks //////////////////////////////
    // Step 1.  Find the centrality matched events 
    int loopCounter = 0;
    for ( int offset=1; offset<= nentries ; offset++) {
      loopCounter++;   //if ( loopCounter >100 ) break;
      
      int i2 = jentry+offset;
      if ( i2>= nentries) 
	i2 = i2 - nentries;
      cMix->GetEntry(i2);
      
      if ( c->evt.hiBin == cMix->evt.hiBin ) 
       	break;   
    }
    if ( (c->evt.evt) == ( cMix->evt.evt ) ) {
      cout << " WARNING!!! Could not find the centrlaity matched events" << endl;
      return;
    }
    
    //    cout << "loop counts = " << loopCounter << endl;
    //    cout << "c1 = " << c->evt.hiBin << " c2 = " << cMix->evt.hiBin << endl;
    
    nmTrk = 0; 
    for (int it=0; it < cMix->track.nTrk; it++ ) { 
      if ( cMix->track.trkPt[it] < cuttrkPtSkim )   continue;
      if (  fabs(cMix->track.trkEta[it]) > cuttrkEtaSkim ) continue;
      if ( cMix->track.highPurity[it] == 0 )   continue;  // highpurity cut by default
      mtrkPt[nmTrk]  = cMix->track.trkPt[it];
      mtrkEta[nmTrk] = cMix->track.trkEta[it];
      mtrkPhi[nmTrk] = cMix->track.trkPhi[it]; 
      int assocJetId = matchedJetFinder( theJet, mtrkEta[nmTrk], mtrkPhi[nmTrk], trkJetRadius); // Works only for reco jets
      if ( assocJetId < 0 )  {
	mtrkAsJetPt[nmTrk] = -1; 
	mtrkAsJetEta[nmTrk] = -1; 
	mtrkAsJetPhi[nmTrk] = -1; 
	mtrkAsJetDR[nmTrk] = 100; 
      } 
      else { 
	mtrkAsJetPt[nmTrk] = theJet->jtpt[assocJetId];
	mtrkAsJetEta[nmTrk] = theJet->jteta[assocJetId];
	mtrkAsJetPhi[nmTrk] = theJet->jtphi[assocJetId];
	mtrkAsJetDR[nmTrk] =getDR( mtrkEta[nmTrk], mtrkPhi[nmTrk], theJet->jteta[assocJetId], theJet->jtphi[assocJetId]) ;
      }
      nmTrk++;
    }
    
    
    
    newtreeTrkJet[evt.cBin][vzBin]->Fill();
  }
  newfile_data->Write();
  cout << " Done! "<< endl;
}



