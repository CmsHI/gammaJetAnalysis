#void forest2yskim_jetSkim_forestV3(TString inputFile_="forestFiles/pA/pA_photonSkimForest_v85_skimPhotonPt50_eta1.5.root",
#                                   std::string MinbiasFname = "skim_trackJet_minbiasTrackJet_mc.root",
#                                   float cutphotonPt  = 50,
#                                   std::string outname = "testPhotonSkim.root",
#                                   sampleType colli=kPADATA,
     #                              bool doMix = false,
    #                               bool doJetResCorrection = 0,
   #                                float addJetEnergyRes = 0,
  #                                 float addFlatJetEnergyRes = 0,
 #                                  float jetEnergyScale = 1,
#                                   bool useGenJetColl = 0
#                                   )
#enum sampleType { kHIDATA, kHIMC, kPPDATA, kPPMC, kPADATA, kPAMC};
#                       0    1      2       3       4        5 


# pbpb data

root -l -q -b 'forest2yskim_jetSkim_forestV3_akpu5.C+("forestFiles/pbpb/HiForestPhoton-v7-noDuplicate.root",       "yskimmedFiles/minbiasSkim_HIMinBias_merged_1_1000.root",    40,  "yskim_HiForestPhoton-v7-noDuplicate_akpu5.root",  0, 1)'


# pp data 
root -l -q -b 'forest2yskim_jetSkim_forestV3_akpu5.C+("forestFiles/pp/pp_photonSKimForest_v85.root",       "no minbias", 40,   "yskim_pp_photonSKimForest_v85_jetNoSmearing_akpu5.root",2,0,0)'

# pp MC. photon hiForest
root -l -q -b 'forest2yskim_jetSkim_forestV3_akpu5.C+("forestFiles/pp/merged_allQCDPhoton30to50_genPhotonPtCut30_CMSSW538HIp2.root",       "minbiasSkim_H02_merged_hiforest_v68_prod12.root", 40,  "yskim_merged_allQCDPhoton30to50_genPhotonPtCut30_CMSSW538HIp2_akpu5.root",3,0)'
root -l -q -b 'forest2yskim_jetSkim_forestV3_akpu5.C+("forestFiles/pp/merged_allQCDPhoton50to80_genPhotonPtCut30_CMSSW538HIp2.root",       "minbiasSkim_H02_merged_hiforest_v68_prod12.root", 40,  "yskim_merged_allQCDPhoton50to80_genPhotonPtCut30_CMSSW538HIp2_akpu5.root",3,0)'
root -l -q -b 'forest2yskim_jetSkim_forestV3_akpu5.C+("forestFiles/pp/merged_allQCDPhoton80to120_genPhotonPtCut30_CMSSW538HIp2.root",       "minbiasSkim_H02_merged_hiforest_v68_prod12.root", 40,  "yskim_merged_allQCDPhoton80to120_genPhotonPtCut30_CMSSW538HIp2_akpu5.root",3,0)'
root -l -q -b 'forest2yskim_jetSkim_forestV3_akpu5.C+("forestFiles/pp/merged_allQCDPhoton120to9999_genPhotonPtCut30_CMSSW538HIp2.root",       "minbiasSkim_H02_merged_hiforest_v68_prod12.root", 40,  "yskim_merged_allQCDPhoton120to9999_genPhotonPtCut30_CMSSW538HIp2_akpu5.root",3,0)'


                          
#  pA data, photon hiForest 
root -l -q -b 'forest2yskim_jetSkim_forestV3_akpu5.C++("forestFiles/pA/pA_photonSkimForest_v85_HLT_PAPhoton30_NoCaloIdVL_v1_highPtPhoton40.root", "", 40,  "yskim_pA_photonSkimForest_v85_HLT_PAPhoton30_NoCaloIdVL_v1_highPtPhoton40_akpu5.root",4,0)'
root -l -q -b 'forest2yskim_jetSkim_forestV3_akpu5.C+("forestFiles/pA/PA2013_pyquen_allQCDPhoton30to50_forestv85.root",       "minbiasSkim_H02_merged_hiforest_v68_prod12.root", 40,  "yskim_PA2013_pyquen_allQCDPhoton30to50_forestv85_akpu5.root", 5,0)'
root -l -q -b 'forest2yskim_jetSkim_forestV3_akpu5.C+("forestFiles/pA/PA2013_pyquen_allQCDPhoton50to80_forestv85.root",       "minbiasSkim_H02_merged_hiforest_v68_prod12.root", 40,  "yskim_PA2013_pyquen_allQCDPhoton50to80_forestv85_akpu5.root", 5,0)'
root -l -q -b 'forest2yskim_jetSkim_forestV3_akpu5.C+("forestFiles/pA/PA2013_pyquen_allQCDPhoton80to120_forestv85.root",       "minbiasSkim_H02_merged_hiforest_v68_prod12.root", 40,  "yskim_PA2013_pyquen_allQCDPhoton80to120_forestv85_akpu5.root", 5,0)'
root -l -q -b 'forest2yskim_jetSkim_forestV3_akpu5.C+("forestFiles/pA/PA2013_pyquen_allQCDPhoton120to9999_forestv85.root",       "minbiasSkim_H02_merged_hiforest_v68_prod12.root", 40,  "yskim_PA2013_pyquen_allQCDPhoton120to9999_forestv85_akpu5.root", 5,0)'

