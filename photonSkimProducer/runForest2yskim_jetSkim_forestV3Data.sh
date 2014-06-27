#void forest2yskim_jetSkim_forestV3(TString inputFile_="forestFiles",
#                                   std::string MinbiasFname = "for"
#                                   float cutphotonPt  = 35, 
#                                   sampleType colli=kHIDATA,
#                                   TString jetAlgo="akPu3PF",
#                                   bool doMix = true,
#                                   TString triggerSelection="",
#                                   bool doJetResCorrection = 0, 
#                                   int smearingCentBin = -1,
#                                   bool useGenJetColl = 0
#                                   )
#

#enum sampleType { kHIDATA, kHIMC, kPPDATA, kPPMC, kPADATA, kPAMC};
#                       0    1      2       3       4        5



# pbpb data


kHIDATA=0
kHIMC=1
kPPDATA=2

for jetAlgo in "akVs3PF"
do
    input="forestFiles/HiForest4/hiForest_Photon40_GR_R_53_LV6_25Feb2014_1530CET_Track8_Jet15.root"
    inputSkim="forestFiles/HiForest4/skim_collId_kHIDATA_jetAlgo_"$jetAlgo"_HIMinBias2011_GR_R_53_LV6_CMSSW_5_3_16_Forest_Track8_Jet21_0.root"
    root -l -q -b 'forest2yskim_jetSkim_forestV3.C+("'$input'", "'$inputSkim'", 35,   '$kHIDATA', "'$jetAlgo'", 1)'
done


root -l -q -b 'forest2yskim_jetSkim_forestV3.C++("forestFiles/pp/pp_photonSKimForest_v85.root", "NoFile", 18, 2, "ak3PF",0, "HLT_PAPhoton20_NoCaloIdVL_v1")'
mv forestFiles/pp/skim_collId_kPPDATA_jetAlgo_ak3PF_20mixed_pp_photonSKimForest_v85.root forestFiles/pp/skim_collId_kPPDATA_jetAlgo_ak3PF_20mixed_pp_photonSKimForest_v85_Photon20_NoCaloIdVL.root

