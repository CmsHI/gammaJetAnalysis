# shell script to skim all of the required files for HIN-13-006, using
# the nominal settings (not for systematic studies).

#enum sampleType { kHIDATA, kHIMC, kPPDATA, kPPMC, kPADATA, kPAMC};
#                       0    1      2       3       4        5

g++ forest2yskim_minbias_forestV3.C $(root-config --cflags --libs) -Wall -Wextra -g -o forest2yskim_minbias_forestV3.exe
g++ forest2yskim_jetSkim_forestV3.C $(root-config --cflags --libs) -Wall -Wextra -g -o forest2yskim_jetSkim_forestV3.exe

# 07/14/2015
# for systematic uncertainties we need to compare results with corrections on and off.
# pbpb DATA
# Jet Residual correction OFF, JES = 1
./forest2yskim_jetSkim_forestV3.exe "/mnt/hadoop/cms/store/user/luck/PbPb2011_photons_Data/HiForestPhoton-v7-noDuplicate.root" "yskimmedFiles/minbiasSkim_HIMinBias_merged_1_1000.root" 35 "yskimmedFiles/yskim_HiForestPhoton-v7-noDuplicate_JetResCorrNO_smearingNO.root" 0 1 0 -1 1 0 0
# Jet Residual correction ON, JES = 1
./forest2yskim_jetSkim_forestV3.exe "/mnt/hadoop/cms/store/user/luck/PbPb2011_photons_Data/HiForestPhoton-v7-noDuplicate.root" "yskimmedFiles/minbiasSkim_HIMinBias_merged_1_1000.root" 35 "yskimmedFiles/yskim_HiForestPhoton-v7-noDuplicate_JetResCorr_smearingNO.root" 0 1 1 -1 1 0 0    # will be same as "yskimmedFiles/yskim_HiForestPhoton-v7-noDuplicate.root"
# Jet Residual correction OFF, JES = 1.02
./forest2yskim_jetSkim_forestV3.exe "/mnt/hadoop/cms/store/user/luck/PbPb2011_photons_Data/HiForestPhoton-v7-noDuplicate.root" "yskimmedFiles/minbiasSkim_HIMinBias_merged_1_1000.root" 35 "yskimmedFiles/yskim_HiForestPhoton-v7-noDuplicate_JetResCorrNO_smearingNO_JES102.root" 0 1 0 -1 1.02 0 0
# Jet Residual correction OFF, JES = 0.98
./forest2yskim_jetSkim_forestV3.exe "/mnt/hadoop/cms/store/user/luck/PbPb2011_photons_Data/HiForestPhoton-v7-noDuplicate.root" "yskimmedFiles/minbiasSkim_HIMinBias_merged_1_1000.root" 35 "yskimmedFiles/yskim_HiForestPhoton-v7-noDuplicate_JetResCorrNO_smearingNO_JES098.root" 0 1 0 -1 0.98 0 0

# 07/13/2015
# for systematic uncertainties we need to compare results with corrections on and off.
# pp DATA
# Jet Residual correction OFF, smearing OFF, JES = 1
# ./forest2yskim_jetSkim_forestV3.exe "/mnt/hadoop/cms/store/user/luck/2014-photon-forests/pp_Data_localJEC_v3_HiForest_nodupes.root" "" 35 "/export/d00/scratch/luck/yskimmedFiles/yskim_pp_Data_localJEC_v3_HiForest_nodupes_JetResCorrNO_smearingNO.root" 2 0 0 -1 1 0 0
# # Jet Residual correction OFF, smearing ON, JES = 1
# ./forest2yskim_jetSkim_forestV3.exe "/mnt/hadoop/cms/store/user/luck/2014-photon-forests/pp_Data_localJEC_v3_HiForest_nodupes.root" "" 35 "/export/d00/scratch/luck/yskimmedFiles/yskim_pp_Data_localJEC_v3_HiForest_nodupes_JetResCorrNO_smearing0010.root" 2 0 0 0 1 0 0
# ./forest2yskim_jetSkim_forestV3.exe "/mnt/hadoop/cms/store/user/luck/2014-photon-forests/pp_Data_localJEC_v3_HiForest_nodupes.root" "" 35 "/export/d00/scratch/luck/yskimmedFiles/yskim_pp_Data_localJEC_v3_HiForest_nodupes_JetResCorrNO_smearing1030.root" 2 0 0 1 1 0 0
# ./forest2yskim_jetSkim_forestV3.exe "/mnt/hadoop/cms/store/user/luck/2014-photon-forests/pp_Data_localJEC_v3_HiForest_nodupes.root" "" 35 "/export/d00/scratch/luck/yskimmedFiles/yskim_pp_Data_localJEC_v3_HiForest_nodupes_JetResCorrNO_smearing3050.root" 2 0 0 2 1 0 0
# ./forest2yskim_jetSkim_forestV3.exe "/mnt/hadoop/cms/store/user/luck/2014-photon-forests/pp_Data_localJEC_v3_HiForest_nodupes.root" "" 35 "/export/d00/scratch/luck/yskimmedFiles/yskim_pp_Data_localJEC_v3_HiForest_nodupes_JetResCorrNO_smearing5099.root" 2 0 0 3 1 0 0
# ./forest2yskim_jetSkim_forestV3.exe "/mnt/hadoop/cms/store/user/luck/2014-photon-forests/pp_Data_localJEC_v3_HiForest_nodupes.root" "" 35 "/export/d00/scratch/luck/yskimmedFiles/yskim_pp_Data_localJEC_v3_HiForest_nodupes_JetResCorrNO_smearing0030.root" 2 0 0 4 1 0 0
# ./forest2yskim_jetSkim_forestV3.exe "/mnt/hadoop/cms/store/user/luck/2014-photon-forests/pp_Data_localJEC_v3_HiForest_nodupes.root" "" 35 "/export/d00/scratch/luck/yskimmedFiles/yskim_pp_Data_localJEC_v3_HiForest_nodupes_JetResCorrNO_smearing3099.root" 2 0 0 5 1 0 0
# # Jet Residual correction ON, smearing OFF, JES = 1
# ./forest2yskim_jetSkim_forestV3.exe "/mnt/hadoop/cms/store/user/luck/2014-photon-forests/pp_Data_localJEC_v3_HiForest_nodupes.root" "" 35 "/export/d00/scratch/luck/yskimmedFiles/yskim_pp_Data_localJEC_v3_HiForest_nodupes_JetResCorr_smearingNO.root" 2 0 1 -1 1 0 0
# # Jet Residual correction OFF, smearing OFF, JES = 1.02
# ./forest2yskim_jetSkim_forestV3.exe "/mnt/hadoop/cms/store/user/luck/2014-photon-forests/pp_Data_localJEC_v3_HiForest_nodupes.root" "" 35 "/export/d00/scratch/luck/yskimmedFiles/yskim_pp_Data_localJEC_v3_HiForest_nodupes_JetResCorrNO_smearingNO_JES102.root" 2 0 0 -1 1.02 0 0
# # Jet Residual correction OFF, smearing OFF, JES = 0.98
# ./forest2yskim_jetSkim_forestV3.exe "/mnt/hadoop/cms/store/user/luck/2014-photon-forests/pp_Data_localJEC_v3_HiForest_nodupes.root" "" 35 "/export/d00/scratch/luck/yskimmedFiles/yskim_pp_Data_localJEC_v3_HiForest_nodupes_JetResCorrNO_smearingNO_JES098.root" 2 0 0 -1 0.98 0 0

# # for systematic uncertainties we need to compare results with corrections on and off.
# # pA DATA 
# # Jet Residual correction OFF, smearing OFF, JES = 1
# ./forest2yskim_jetSkim_forestV3.exe "/mnt/hadoop/cms/store/user/luck/2014-photon-forests/pPb_Data_all_localJEC_v3_HiForest_nodupes.root" "" 35 "/export/d00/scratch/luck/yskimmedFiles/yskim_pPb_Data_all_localJEC_v3_HiForest_nodupes_JetResCorrNO_smearingNO.root" 4 0 0 -1 1 0 0
# # Jet Residual correction ON, smearing OFF, JES = 1
# ./forest2yskim_jetSkim_forestV3.exe "/mnt/hadoop/cms/store/user/luck/2014-photon-forests/pPb_Data_all_localJEC_v3_HiForest_nodupes.root" "" 35 "/export/d00/scratch/luck/yskimmedFiles/yskim_pPb_Data_all_localJEC_v3_HiForest_nodupes_JetResCorr_smearingNO.root" 4 0 1 -1 1 0 0
# # Jet Residual correction OFF, smearing OFF, JES = 1.02
# ./forest2yskim_jetSkim_forestV3.exe "/mnt/hadoop/cms/store/user/luck/2014-photon-forests/pPb_Data_all_localJEC_v3_HiForest_nodupes.root" "" 35 "/export/d00/scratch/luck/yskimmedFiles/yskim_pPb_Data_all_localJEC_v3_HiForest_nodupes_JetResCorrNO_smearingNO_JES102.root" 4 0 0 -1 1.02 0 0
# # Jet Residual correction OFF, smearing OFF, JES = 0.98
# ./forest2yskim_jetSkim_forestV3.exe "/mnt/hadoop/cms/store/user/luck/2014-photon-forests/pPb_Data_all_localJEC_v3_HiForest_nodupes.root" "" 35 "/export/d00/scratch/luck/yskimmedFiles/yskim_pPb_Data_all_localJEC_v3_HiForest_nodupes_JetResCorrNO_smearingNO_JES098.root" 4 0 0 -1 0.98 0 0
