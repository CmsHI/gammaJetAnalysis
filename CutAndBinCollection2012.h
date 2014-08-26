#ifndef CutAndBinCollection_C
#define CutAndBinCollection_C

#include <TF1.h>
#include <TCut.h>
#include <TChain.h>
#include <TGraphAsymmErrors.h>
#include <TCanvas.h>
#include <TNtuple.h>
#include <iostream>
#include <TLine.h>
#include <TMath.h>
#include <math.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include <TROOT.h>
#include <TClonesArray.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <TLegend.h>

#define PI 3.141592653589

// Updated on Oct 20th.   Jet energy residual correction is done by default.
///////////// pp Data

TString fnamePPDATA                  =   "yskimmedFiles/yskim_pp_photonSKimForest_v85_jetNoSmearing.root";
//no Residual correction
TString fnamePPDATA_noJetResCorr    = "yskimmedFiles/yskim_pp_photonSKimForest_v85_jetNoSmearing_noJetResCorr.root";
/// jet energy smeared  // Now these are smeared and thn corr by default
TString fnamePPDATA0010                  =   "yskimmedFiles/yskim_pp_photonSKimForest_v85_jetSmearingCent0010.root";
TString fnamePPDATA1030                  =   "yskimmedFiles/yskim_pp_photonSKimForest_v85_jetSmearingCent1030.root";
TString fnamePPDATA3050                  =   "yskimmedFiles/yskim_pp_photonSKimForest_v85_jetSmearingCent3050.root";
TString fnamePPDATA5099                 =   "yskimmedFiles/yskim_pp_photonSKimForest_v85_jetSmearingCent5099.root";
TString fnamePPDATA30100                =   "yskimmedFiles/yskim_pp_photonSKimForest_v85_jetSmearingCent3099.root";
TString fnamePPDATA0030                =   "yskimmedFiles/yskim_pp_photonSKimForest_v85_jetSmearingCent0030.root";

// pp MC
TString fnamePPMC_AllQcdPho30to50    = "yskimmedFiles/yskim_merged_allQCDPhoton30to50_genPhotonPtCut30_CMSSW538HIp2.root";
TString fnamePPMC_AllQcdPho50to80    = "yskimmedFiles/yskim_merged_allQCDPhoton50to80_genPhotonPtCut30_CMSSW538HIp2.root";
TString fnamePPMC_AllQcdPho80to120   = "yskimmedFiles/yskim_merged_allQCDPhoton80to120_genPhotonPtCut30_CMSSW538HIp2.root";
TString fnamePPMC_AllQcdPho120to9999 = "yskimmedFiles/yskim_merged_allQCDPhoton120to9999_genPhotonPtCut30_CMSSW538HIp2.root";
double  wPPMC_AllQcdPho30to50        = 29329./ 29329.;
double  wPPMC_AllQcdPho50to80        = 8098. / 87988.;
double  wPPMC_AllQcdPho80to120       = 1680. / 96756.;
double  wPPMC_AllQcdPho120to9999     = 438.  / 90972.;

// pp MC smeared for closure test
TString fnamePPMC_AllQcdPho30to50_smeared0030    = "yskimmedFiles/yskim_merged_allQCDPhoton30to50_genPhotonPtCut30_CMSSW538HIp2_smeared0030.root";
TString fnamePPMC_AllQcdPho50to80_smeared0030    = "yskimmedFiles/yskim_merged_allQCDPhoton50to80_genPhotonPtCut30_CMSSW538HIp2_smeared0030.root";
TString fnamePPMC_AllQcdPho80to120_smeared0030   = "yskimmedFiles/yskim_merged_allQCDPhoton80to120_genPhotonPtCut30_CMSSW538HIp2_smeared0030.root";
TString fnamePPMC_AllQcdPho120to9999_smeared0030 = "yskimmedFiles/yskim_merged_allQCDPhoton120to9999_genPhotonPtCut30_CMSSW538HIp2_smeared0030.root";

TString fnamePPMC_AllQcdPho30to50_smeared3099    = "yskimmedFiles/yskim_merged_allQCDPhoton30to50_genPhotonPtCut30_CMSSW538HIp2_smeared3099.root";
TString fnamePPMC_AllQcdPho50to80_smeared3099    = "yskimmedFiles/yskim_merged_allQCDPhoton50to80_genPhotonPtCut30_CMSSW538HIp2_smeared3099.root";
TString fnamePPMC_AllQcdPho80to120_smeared3099   = "yskimmedFiles/yskim_merged_allQCDPhoton80to120_genPhotonPtCut30_CMSSW538HIp2_smeared3099.root";
TString fnamePPMC_AllQcdPho120to9999_smeared3099 = "yskimmedFiles/yskim_merged_allQCDPhoton120to9999_genPhotonPtCut30_CMSSW538HIp2_smeared3099.root";


TString fnamePPMC_AllQcdPho30to50_smeared0010    = "yskimmedFiles/yskim_merged_allQCDPhoton30to50_genPhotonPtCut30_CMSSW538HIp2_smeared0010.root";
TString fnamePPMC_AllQcdPho50to80_smeared0010    = "yskimmedFiles/yskim_merged_allQCDPhoton50to80_genPhotonPtCut30_CMSSW538HIp2_smeared0010.root";
TString fnamePPMC_AllQcdPho80to120_smeared0010   = "yskimmedFiles/yskim_merged_allQCDPhoton80to120_genPhotonPtCut30_CMSSW538HIp2_smeared0010.root";
TString fnamePPMC_AllQcdPho120to9999_smeared0010 = "yskimmedFiles/yskim_merged_allQCDPhoton120to9999_genPhotonPtCut30_CMSSW538HIp2_smeared0010.root";
TString fnamePPMC_AllQcdPho30to50_smeared1030    = "yskimmedFiles/yskim_merged_allQCDPhoton30to50_genPhotonPtCut30_CMSSW538HIp2_smeared1030.root";
TString fnamePPMC_AllQcdPho50to80_smeared1030    = "yskimmedFiles/yskim_merged_allQCDPhoton50to80_genPhotonPtCut30_CMSSW538HIp2_smeared1030.root";
TString fnamePPMC_AllQcdPho80to120_smeared1030   = "yskimmedFiles/yskim_merged_allQCDPhoton80to120_genPhotonPtCut30_CMSSW538HIp2_smeared1030.root";
TString fnamePPMC_AllQcdPho120to9999_smeared1030 = "yskimmedFiles/yskim_merged_allQCDPhoton120to9999_genPhotonPtCut30_CMSSW538HIp2_smeared1030.root";
TString fnamePPMC_AllQcdPho30to50_smeared3050    = "yskimmedFiles/yskim_merged_allQCDPhoton30to50_genPhotonPtCut30_CMSSW538HIp2_smeared3050.root";
TString fnamePPMC_AllQcdPho50to80_smeared3050    = "yskimmedFiles/yskim_merged_allQCDPhoton50to80_genPhotonPtCut30_CMSSW538HIp2_smeared3050.root";
TString fnamePPMC_AllQcdPho80to120_smeared3050   = "yskimmedFiles/yskim_merged_allQCDPhoton80to120_genPhotonPtCut30_CMSSW538HIp2_smeared3050.root";
TString fnamePPMC_AllQcdPho120to9999_smeared3050 = "yskimmedFiles/yskim_merged_allQCDPhoton120to9999_genPhotonPtCut30_CMSSW538HIp2_smeared3050.root";
TString fnamePPMC_AllQcdPho30to50_smeared5099    = "yskimmedFiles/yskim_merged_allQCDPhoton30to50_genPhotonPtCut30_CMSSW538HIp2_smeared5099.root";
TString fnamePPMC_AllQcdPho50to80_smeared5099    = "yskimmedFiles/yskim_merged_allQCDPhoton50to80_genPhotonPtCut30_CMSSW538HIp2_smeared5099.root";
TString fnamePPMC_AllQcdPho80to120_smeared5099   = "yskimmedFiles/yskim_merged_allQCDPhoton80to120_genPhotonPtCut30_CMSSW538HIp2_smeared5099.root";
TString fnamePPMC_AllQcdPho120to9999_smeared5099 = "yskimmedFiles/yskim_merged_allQCDPhoton120to9999_genPhotonPtCut30_CMSSW538HIp2_smeared5099.root";




// pp MC smearing factors
double c_pp = 0.056762;
double s_pp = 0.808114;
double n_pp = 0.000244992;
// 0-10%,  10-30%,  30-50%,  50-100%,   0-30%,  30-100%
// old ones
/* double c_pbpb[] = {0.0560882, 0.0256674, 0.00644361, 0.0170442, 0.0419, 0.0101  }; */
/* double s_pbpb[] = {1.47838, 1.40659, 1.31446, 1.22837, 1.441,  1.294, }; */
/* double n_pbpb[] = {0.115188, -0.0341018, -0.0721935, 0.0511498, 0.0807,  0.0676}; */

// new ones, from official MC
// currently 0-30% and 30-100% are weighted averages of other 4 bins - should be corrected
double c_pbpb[] = {0.0363269, 0.0147739, 0.0264179, 0.0608379, 0.021958233333333334, 0.05100361428571429 };
double s_pbpb[] = {1.3291, 1.33037, 1.27536, 0.911933, 1.3299466666666668, 1.0157692857142857 };
double n_pbpb[] = {6.49405, 5.00827, 2.70035, 5.70016, 5.5035300000000005, 4.843071428571429};

// pp MC phi smearing factors
double cphi_pp = 0.024497;
double sphi_pp = -0.170472;
double nphi_pp = -0.000188492;
// 0-10%,  10-30%,  30-50%,  50-100%,   0-30%,  30-100%
double cphi_pbpb[] = {0.0216325, 0.0168838,  -0.0160583,   -0.0271079,   0.0215035,   -0.0223996};
double sphi_pbpb[] = {0.342842,  0.328665,   -0.301696,    -0.227269,    0.32505,     -0.26666};
double nphi_pbpb[] = {0.0002195, 3.79342e-05, 3.46104e-06, -1.51886e-06, 9.56424e-05 , 2.77627e-06};


// not important at the moment
TString fnamePPDATA_Cone05                  =   "yskimmedFiles/yskim_pp_photonSKimForest_v85_jetNoSmearing_akpu5.root";
TString fnamePPMC_AllQcdPho30to50_Cone05    = "yskimmedFiles/yskim_merged_allQCDPhoton30to50_genPhotonPtCut30_CMSSW538HIp2_akpu5.root";
TString fnamePPMC_AllQcdPho50to80_Cone05    = "yskimmedFiles/yskim_merged_allQCDPhoton50to80_genPhotonPtCut30_CMSSW538HIp2_akpu5.root";
TString fnamePPMC_AllQcdPho80to120_Cone05   = "yskimmedFiles/yskim_merged_allQCDPhoton80to120_genPhotonPtCut30_CMSSW538HIp2_akpu5.root";
TString fnamePPMC_AllQcdPho120to9999_Cone05 = "yskimmedFiles/yskim_merged_allQCDPhoton120to9999_genPhotonPtCut30_CMSSW538HIp2_akpu5.root";


TString fnamePPMC_emDijet30to50    = "yskimmedFiles/yskim_emDijet30to50_CMSSW538HIp2.root";
TString fnamePPMC_emDijet50to80    = "yskimmedFiles/yskim_emDijet50to80_CMSSW538HIp2.root";
TString fnamePPMC_emDijet80to120   = "yskimmedFiles/yskim_emDijet80to120_CMSSW538HIp2.root";
TString fnamePPMC_emDijet120to9999 = "yskimmedFiles/yskim_emDijet120to9999_CMSSW538HIp2.root";
double wPPMC_emDijet30to50         = 31957./31957.;
double wPPMC_emDijet50to80         = 46494./114811.;
double wPPMC_emDijet80to120        = 17265./121988.;
double wPPMC_emDijet120to9999      = 4284./130444.;


///////////// pA Data  // File name checked on Oct 20th
TString fnamePADATA                  = "yskimmedFiles/yskim_pA_photonSkimForest_v85_HLT_PAPhoton30_NoCaloIdVL_v1_highPtPhoton40.root";
TString fnamePADATA_noJetResCorr     = "yskimmedFiles/yskim_pA_photonSkimForest_v85_HLT_PAPhoton30_NoCaloIdVL_v1_highPtPhoton40_noJetResCorr.root";
//            pA MC   // File name checked on Oct 20th
TString fnamePAMC_AllQcdPho30to50    = "yskimmedFiles/yskim_PA2013_pyquen_allQCDPhoton30to50_forestv85.root";
TString fnamePAMC_AllQcdPho50to80    = "yskimmedFiles/yskim_PA2013_pyquen_allQCDPhoton50to80_forestv85.root";
TString fnamePAMC_AllQcdPho80to120   = "yskimmedFiles/yskim_PA2013_pyquen_allQCDPhoton80to120_forestv85.root";
TString fnamePAMC_AllQcdPho120to9999 = "yskimmedFiles/yskim_PA2013_pyquen_allQCDPhoton120to9999_forestv85.root";
//
TString fnamePAMC_AllQcdPho30to50_noJetResCorr    = "yskimmedFiles/yskim_PA2013_pyquen_allQCDPhoton30to50_forestv85_noJetResCorr.root";
TString fnamePAMC_AllQcdPho50to80_noJetResCorr    = "yskimmedFiles/yskim_PA2013_pyquen_allQCDPhoton50to80_forestv85_noJetResCorr.root";
TString fnamePAMC_AllQcdPho80to120_noJetResCorr   = "yskimmedFiles/yskim_PA2013_pyquen_allQCDPhoton80to120_forestv85_noJetResCorr.root";
TString fnamePAMC_AllQcdPho120to9999_noJetResCorr = "yskimmedFiles/yskim_PA2013_pyquen_allQCDPhoton120to9999_forestv85_noJetResCorr.root";

// Not important
TString fnamePADATA_Cone05                  = "yskimmedFiles/yskim_pA_photonSkimForest_v85_HLT_PAPhoton30_NoCaloIdVL_v1_highPtPhoton40_akpu5.root";
//            pA MC
TString fnamePAMC_AllQcdPho30to50_Cone05    = "yskimmedFiles/yskim_PA2013_pyquen_allQCDPhoton30to50_forestv85_akpu5.root";
TString fnamePAMC_AllQcdPho50to80_Cone05    = "yskimmedFiles/yskim_PA2013_pyquen_allQCDPhoton50to80_forestv85_akpu5.root";
TString fnamePAMC_AllQcdPho80to120_Cone05   = "yskimmedFiles/yskim_PA2013_pyquen_allQCDPhoton80to120_forestv85_akpu5.root";
TString fnamePAMC_AllQcdPho120to9999_Cone05 = "yskimmedFiles/yskim_PA2013_pyquen_allQCDPhoton120to9999_forestv85_akpu5.root";

double  wPAMC_AllQcdPho30to50        = 56669./ 50385.;
double  wPAMC_AllQcdPho50to80        = 41906./ 114136.;
double  wPAMC_AllQcdPho80to120       = 12044./ 103562.;
double  wPAMC_AllQcdPho120to9999     = 4481. / 151511.;



///////////// PbPb Data // File name checked on Oct 20th
TString fnameHIDATA                  = "yskimmedFiles/yskim_HiForestPhoton-v7-noDuplicate.root";
TString fnameHIDATA_noJetResCorr= "yskimmedFiles/yskim_HiForestPhoton-v7-noDuplicate_noJetResCorr.root";
// PbPb MC  // File name checked on Oct 20th
TString fnameHIMC_AllQcdPho30to50    =  "yskimmedFiles/yskim_qcdAllPhoton30to50_genPhotonPtCut40_allCent.root";
TString fnameHIMC_AllQcdPho50to80    =  "yskimmedFiles/yskim_qcdAllPhoton50to80_genPhotonPtCut40_allCent.root";
TString fnameHIMC_AllQcdPho80to9999  =  "yskimmedFiles/yskim_qcdAllPhoton80to9999_genPhotonPtCut40_allCent.root";
double  wHIMC_AllQcdPho30to50        = 32796./ 32796.;
double  wHIMC_AllQcdPho50to80        = 21470./ 53876.;
double  wHIMC_AllQcdPho80to9999       = 6462. / 58781.;

/// Jet energy is smeared and/or scaled in histogram step
/////// Data with jet energy smeared by 10%
TString fnameHIDATA_10percentSmeared  = "yskimmedFiles/yskim_HiForestPhoton-v7-noDuplicate_smearedBy10Flat.root"  ;
TString fnamePADATA_10percentSmeared  = "yskimmedFiles/yskim_pA_photonSkimForest_v85_HLT_PAPhoton30_NoCaloIdVL_v1_highPtPhoton40_smearedBy10Flat.root";
TString fnamePPDATA_10percentSmeared  = "yskimmedFiles/yskim_pp_photonSKimForest_v85_jetNoSmearing_smearedBy10Flat.root"; // no smearing means no relative jet energy smearing
/////// Data with jet energy scaled by +2%
//TString fnameHIDATA_Plus2percentScaled  = "yskimmedFiles/yskim_HiForestPhoton-v7-noDuplicate_jetScaleBy2percent.root";
//TString fnamePADATA_Plus2percentScaled  = "yskimmedFiles/yskim_pA_photonSkimForest_v85_HLT_PAPhoton30_NoCaloIdVL_v1_highPtPhoton40_jetScaleBy2percent.root";
//TString fnamePPDATA_Plus2percentScaled  = "yskimmedFiles/yskim_pp_photonSKimForest_v85_jetScaleBy2percent.root";
/////// Data with jet energy scaled by -2%
//TString fnameHIDATA_Minus2percentScaled  = "yskimmedFiles/yskim_HiForestPhoton-v7-noDuplicate_jetScaleBy-2percent.root";
//TString fnamePADATA_Minus2percentScaled  = "yskimmedFiles/yskim_pA_photonSkimForest_v85_HLT_PAPhoton30_NoCaloIdVL_v1_highPtPhoton40_jetScaleBy-2percent.root";
//TString fnamePPDATA_Minus2percentScaled  = "yskimmedFiles/yskim_pp_photonSKimForest_v85_jetScaleBy-2percent.root";



/////// Data without electron rejection // File name checked on Oct 20th
TString fnameHIDATA_noElectronRejection  = "yskimmedFiles/yskim_HiForestPhoton-v7-noDuplicate_noEleRejection.root";
TString fnamePADATA_noElectronRejection  = "yskimmedFiles/yskim_pA_photonSkimForest_v85_HLT_PAPhoton30_NoCaloIdVL_v1_highPtPhoton40_noEleRejection.root";
TString fnamePPDATA_noElectronRejection  = "yskimmedFiles/yskim_pp_photonSKimForest_v85_jetNoSmearing_noEleRejection.root";

// DON'T FORGET TO APPLY HOE CUT SEPARATELY
// Convinient Output Classes

// sample typle
enum sampleType { kHIDATA, kHIMC, kPPDATA, kPPMC, kPADATA, kPAMC};
TString getSampleName ( sampleType colli) {
  if (colli == kHIDATA) return "pbpbDATA";
  if (colli == kHIMC) return "pbpbMC";
  if (colli == kPPDATA) return "ppDATA";
  if (colli == kPPMC) return "ppMC";
  if (colli == kPADATA) return "ppbDATA";
  if (colli == kPAMC) return "ppbMC";
  return "NULL";
}
TString getSampleName ( int colli) {
  if (colli == kHIDATA) return "pbpbDATA";
  if (colli == kHIMC) return "pbpbMC";
  if (colli == kPPDATA) return "ppDATA";
  if (colli == kPPMC) return "ppMC";
  if (colli == kPADATA) return "ppbDATA";
  if (colli == kPAMC) return "ppbMC";
  return "NULL";
}

const int nMixing1 = 20;

const int kHIMCNoJetWeight = 2001;

TCut isoSumCut  = "(cc4+cr4+ct4PtCut20)/0.9 <1";

TCut sbIsoCut =" (cc4+cr4+ct4PtCut20)/0.9>10 && (cc4+cr4+ct4PtCut20)/0.9 < 20 ";
TCut sbIsoPPCut = sbIsoCut;

float isolationCut = 5.0;

TCut isFragment = "abs(genMomId)<22";
TCut isPrompt = "abs(genMomId)==22";

TCut genMatchCut0      = "isGenMatched && abs(genMomId)<=22";
TCut genMatchCut1      = Form("isGenMatched && genMomId==22 && genCalIsoDR04 < %.1f",isolationCut);
TCut genMatchCut      = Form("(isGenMatched && abs(genMatchedEta)<1.44 && abs(etCorrected/genMatchedPt-1)<.3 && abs(genMomId) <= 22 && genCalIsoDR04 < %.1f)",isolationCut);
TCut genMatchCutBkg      = "(isGenMatched && abs(genMatchedEta)<1.44 && abs(etCorrected/genMatchedPt-1)<.6)  &&  ( (abs(genMomId) > 22) || (genCalIsoDR04 > 5.0) ) ";


TCut genPhotonCut     = Form("( abs(gpEta) < 1.44 && abs(gpId)==22 && abs(gpMomId) <= 22 && gpCollId ==0  && gpIsoDR04 < %.3f)",isolationCut);



TString swissCrx      = "(1 - (eRight+eLeft+eTop+eBottom)/eMax)";
TCut hiSpikeCutMC     = Form("(  %s < 0.90 && sigmaIetaIeta>0.002 && sigmaIphiIphi>0.002)",swissCrx.Data());
TCut ppSpikeCutMC     = Form("(  %s < 0.95 && sigmaIetaIeta>0.002 && sigmaIphiIphi>0.002)",swissCrx.Data());

//TCut hiSpikeCutNoPhi  = Form("( ( %s < 0.90 && sigmaIetaIeta>0.002) ",swissCrx.Data());

//TCut hiSpikeCutNoPhi  = Form("( ( %s < 0.90 && sigmaIetaIeta>0.002) ",swissCrx.Data());


TCut seedTimeCut      = "abs(seedTime)<3";
TCut hiSpikeCutData   = hiSpikeCutMC && seedTimeCut;
TCut ppSpikeCutData   = ppSpikeCutMC && seedTimeCut;

TCut etaCut       = "abs(eta)<1.44 && abs(scEta)<1.479";

//TCut etaCut           = " (abs(scEta) < 1.479 && abs(eta)<1.44) && (rawEnergy/energy > 0.5)";// && (!isEBGap&&!isEEGap&&!isEBEEGap)";

TCut genEtaCut  =       "(abs(eta) < 1.44)";
TCut finalCutSigHI  = genMatchCut     &&  hiSpikeCutMC && etaCut ;
TCut finalCutBkgHI  = !genMatchCut  &&  hiSpikeCutMC && etaCut ;
TCut finalCutDataHI =                   hiSpikeCutData && etaCut;

TCut finalCutSigPP  = genMatchCut     &&  ppSpikeCutMC && etaCut;
TCut finalCutBkgPP  = !genMatchCut  &&  ppSpikeCutMC && etaCut;
TCut finalCutDataPP =                     ppSpikeCutData && etaCut;



TCut finalCutGen  =  genPhotonCut ;




// Cuts for track and jets
double cutjetPtSkim = 15;
double cuttrkEtaSkim = 2.0;

double cutjetEta = 1.6;
double cutjetEtaSkim = 3.0;

double cuttrkPt =  10;
double cuttrkEta  = 2.0;
double drCutTJ = 0.3 ;  // cut for tracks in jets


int ycolor[9] =  {0,1,2,4,8,20,1,1,1};  // the for centrality bins...
int ycolorEt[9]= {0,1,2,4,8,20,1,1,1};

// temporar.y!!!

const int theEvtPlNumber = 21;


int nCentBinSkim = 40;

int nCentBinSkimPA = 16;
int getHfBin( float hf4Sum=-1 ) {
  if ( hf4Sum < -1 )   return -1;
  else if ( hf4Sum < 5 )   return 0;
  else if ( hf4Sum < 10)   return 1;
  else if ( hf4Sum < 15)   return 2;
  else if ( hf4Sum < 20 )   return 3;
  else if ( hf4Sum < 25 )   return 4;
  else if ( hf4Sum < 30 )   return 5;
  else if ( hf4Sum < 35 )   return 6;
  else if ( hf4Sum < 40 )   return 7;
  else if ( hf4Sum < 45 )   return 8;
  else if ( hf4Sum < 50 )   return 9;
  else if ( hf4Sum < 55 )   return 10;
  else if ( hf4Sum < 60 )   return 11;
  else if ( hf4Sum < 65 )   return 12;
  else if ( hf4Sum < 70 )   return 13;
  else if ( hf4Sum < 75 )   return 14;
  else return 15;

}



const int nCentBin1 = 4;
double centBin1[nCentBin1+1] = {0,4,12,20,40};


const int nCentBinPa = 1;
//double centBinPa[nCentBinPa+1] = {0,20,30,100};
double centBinPa[nCentBinPa+1] = {0,100};
//double centBinPa[nCentBinPa+1] = {0,20,25,30,40,100};

const int nPtBinPa = 4;
double ptBinPa[nPtBinPa+1]    = {40,50,60,80,9999};
double ptBinPaDraw[nPtBinPa+1]    = {45,55,70,100,200};


float vtxCutPhotonAna = 15;
float jetDPhiCut = PI * 7./8.;

const int nVtxBin =1;
const int nPlnBin = 1;

void mcStyle(TH1* h=0) {
   h->SetLineColor(kPink);
   h->SetFillColor(kOrange+7);
   h->SetFillStyle(3004);
}

void sbStyle(TH1* h=0) {
   h->SetLineColor(kGreen+4);
   h->SetFillColor(kGreen+1);
   h->SetFillStyle(3001);
}

void dijetStyle(TH1* h=0) {
   h->SetLineColor(kBlue);
   h->SetFillColor(kAzure-8);
   h->SetFillStyle(3001);
}

void getNColl( float* ncoll) {

ncoll[0] = 1747.86 ;
ncoll[1] = 1567.53 ;
ncoll[2] = 1388.39 ;
ncoll[3] = 1231.77 ;
ncoll[4] = 1098.2 ;
ncoll[5] = 980.439 ;
ncoll[6] = 861.609 ;
ncoll[7] = 766.042 ;
ncoll[8] = 676.515 ;
ncoll[9] = 593.473 ;
ncoll[10] = 521.912 ;
ncoll[11] = 456.542 ;
ncoll[12] = 398.546 ;
ncoll[13] = 346.647 ;
ncoll[14] = 299.305 ;
ncoll[15] = 258.344 ;
ncoll[16] = 221.216 ;
ncoll[17] = 188.677 ;
ncoll[18] = 158.986 ;
ncoll[19] = 134.7 ;
ncoll[20] = 112.547 ;
ncoll[21] = 93.4537 ;
ncoll[22] = 77.9314 ;
ncoll[23] = 63.5031 ;
ncoll[24] = 52.0469 ;
ncoll[25] = 42.3542 ;
ncoll[26] = 33.9204 ;
ncoll[27] = 27.3163 ;
ncoll[28] = 21.8028 ;
ncoll[29] = 17.2037 ;
ncoll[30] = 13.5881 ;
ncoll[31] = 10.6538 ;
ncoll[32] = 8.35553 ;
ncoll[33] = 6.40891 ;
ncoll[34] = 5.13343 ;
ncoll[35] = 3.73215 ;
ncoll[36] = 3.06627 ;
ncoll[37] = 2.41926 ;
ncoll[38] = 2.11898 ;
 ncoll[39] = 1.76953 ;

}

#endif
