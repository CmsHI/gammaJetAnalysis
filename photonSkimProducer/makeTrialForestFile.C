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


using namespace std;

static const long MAXTREESIZE = 10000000000;





void makeTrialForestFile(int nMax= 1000){ 
  //collisionType
  TString inputFile_="/home/jazzitup/forestFiles/pp/merged_allQCDPhoton30_50_80_120_CMSSW538HIp2.root";
  TString outname = Form("merged_allQCDPhoton%dto%d_genPtCut%d_CMSSW538HIp2.root",ptHatCut1,ptHatCut2,genPhotonPtCut);
  
  
  // start from here
  // path length histogram
  
  HiForest * t;
  t = new HiForest(inputFile_.Data(),"",colli);
   
  t->SetOutputFile(outname.Data());
  // LOOP!!
  t->InitTree();

  for (Long64_t jentry = 0 ; jentry < mMax ; jentry++) {
    if (jentry% 10000 == 0)  {
      cout <<jentry<<" / "<<nentries<<" "<<setprecision(2)<<(double)jentry/nentries*100<<endl;
    }
    t->GetEntry(jentry);
    t->FillOutput();
  }
  delete t;
}

