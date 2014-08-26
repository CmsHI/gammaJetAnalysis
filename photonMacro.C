#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TProfile.h"
#include "CutAndBinCollection2012.h"
#include "alexGammaSkim.h"
#include "TGraphErrors.h"

void photonMacro()
{
  TH1::SetDefaultSumw2();

  TFile *infile = TFile::Open("gammaJets_PbPb_Data.root");
  GammaJetSkim s(infile);

  Long64_t entries = s.photonTree->GetEntries();

  // int bins = 4;
  // // 0 43.63075 116.3597 235.7747 359.017
  // double xbins = {};
  TProfile *xjg = new TProfile("xjg",";centrality;x_{J#gamma}",nCentBin1,centBin1);

  for(Long64_t i = 0; i < entries; ++i)
  {
    s.GetEntry(i);

    if(s.corrGPt < 60) continue;
    if(s.cr4 + s.cc4 + s.ct4PtCut20 > 1.0) continue;
    if(s.hadronicOverEm > 0.1) continue;
    if(s.sigmaIetaIeta > 0.01) continue;

    for(int j = 0; j < s.nJets; ++j)
    {
      if(s.dPhi[j] < 7*PI/8) continue;
      if(s.jPt[j] < 30) continue;

      double xjg_i = s.jPt[j]/s.corrGPt;

      // double point = 0;
      // if(hiBin_ < 4) point = 359.017;
      // else if( hiBin_ < 12) point = 235.7747;
      // else if( hiBin_ < 20) point = 116.3597;
      // else point = 43.63075;
      xjg->Fill(s.hiBin, xjg_i);
    }
  }
  double xpoints[] = {43.63075, 116.3597, 235.7747, 359.017};
  double ypoints[] = {xjg->GetBinContent(4), xjg->GetBinContent(3), xjg->GetBinContent(2), xjg->GetBinContent(1)};
  double yepoints[] = {xjg->GetBinError(4), xjg->GetBinError(3), xjg->GetBinError(2), xjg->GetBinError(1)};

  TGraphErrors *xjg_g = new TGraphErrors(4, xpoints, ypoints,0,yepoints);

  //handsomeTH1(xjg, 1);
  xjg_g->GetYaxis()->SetTitle("<x_{J#gamma}>  (>30GeV)");
  //xjg->SetMarkerStyle(24);
  xjg_g->GetXaxis()->SetRangeUser(0,400);
  xjg_g->GetYaxis()->SetRangeUser(0.6,1.1);
  xjg_g->Draw("A E p");

}
