#!/bin/bash

#enum sampleType { kHIDATA, kHIMC, kPPDATA, kPPMC, kPADATA, kPAMC};
#                  0        1      2        3      4        5
#void gammaJetHistProducer(sampleType collision = kPADATA, float photonPtThr=60,
# float photonPtThrUp=9999, float jetPtThr=30, int icent =1, TString fname = "",
# TString outNameSuffix = "")

g++ gammaJetHistProducer.C $(root-config --cflags --libs) -Wall -Wextra -g -o gammaJetHistProducer.exe || exit 1

#################################
# pp DATA
# yskim files for Jet Systematics
yskimFilesPP=(
"yskim_pp_Data_localJEC_v3_HiForest_nodupes_JetResCorrNO_smearingNO.root"
"yskim_pp_Data_localJEC_v3_HiForest_nodupes_JetResCorrNO_smearing0010.root"
"yskim_pp_Data_localJEC_v3_HiForest_nodupes_JetResCorrNO_smearing1030.root"
"yskim_pp_Data_localJEC_v3_HiForest_nodupes_JetResCorrNO_smearing3050.root"
"yskim_pp_Data_localJEC_v3_HiForest_nodupes_JetResCorrNO_smearing5099.root"
"yskim_pp_Data_localJEC_v3_HiForest_nodupes_JetResCorrNO_smearing0030.root"
"yskim_pp_Data_localJEC_v3_HiForest_nodupes_JetResCorrNO_smearing3099.root"
"yskim_pp_Data_localJEC_v3_HiForest_nodupes_JetResCorr_smearingNO.root"
"yskim_pp_Data_localJEC_v3_HiForest_nodupes_JetResCorrNO_smearingNO_JES102.root"
"yskim_pp_Data_localJEC_v3_HiForest_nodupes_JetResCorrNO_smearingNO_JES098.root"
);
prefixYskimPP="yskim_pp_Data_localJEC_v3_HiForest_nodupes_";

# pp DATA
for yskimFile in "${yskimFilesPP[@]}"
do
  inFile="yskimmedFiles/"$yskimFile;

  # delete prefix of file name so that we can get the suffix
  suffixTMP=${yskimFile#$prefixYskimPP};
  suffix=${suffixTMP%.root};   # trim ".root"
  for i in 2
  do
    for icent in 10010 11030 13050 15099 10030 13099 7
    do
        ./gammaJetHistProducer.exe $i 40 50 30 $icent $inFile $suffix || exit 1
        ./gammaJetHistProducer.exe $i 50 60 30 $icent $inFile $suffix || exit 1
        ./gammaJetHistProducer.exe $i 60 80 30 $icent $inFile $suffix || exit 1
        ./gammaJetHistProducer.exe $i 80 9999 30 $icent $inFile $suffix || exit 1
    done
  done
done

#################################
# pPb DATA
# yskim files for Jet Systematics
yskimFilesPA=(
"yskim_pPb_Data_all_localJEC_v3_HiForest_nodupes_JetResCorrNO_smearingNO.root"
"yskim_pPb_Data_all_localJEC_v3_HiForest_nodupes_JetResCorr_smearingNO.root"
"yskim_pPb_Data_all_localJEC_v3_HiForest_nodupes_JetResCorrNO_smearingNO_JES102.root"
"yskim_pPb_Data_all_localJEC_v3_HiForest_nodupes_JetResCorrNO_smearingNO_JES098.root"
);
prefixYskimPA="yskim_pPb_Data_all_localJEC_v3_HiForest_nodupes_";

# pPb DATA
for yskimFile in "${yskimFilesPA[@]}"
do
  inFile="yskimmedFiles/"$yskimFile;

  # delete prefix of file name so that we can get the suffix
  suffixTMP=${yskimFile#$prefixYskimPA};
  suffix=${suffixTMP%.root};   # trim ".root"
  for i in 4
  do
    for icent in 1
    do
        ./gammaJetHistProducer.exe $i 40 50 30 $icent $inFile $suffix || exit 1
        ./gammaJetHistProducer.exe $i 50 60 30 $icent $inFile $suffix || exit 1
        ./gammaJetHistProducer.exe $i 60 80 30 $icent $inFile $suffix || exit 1
        ./gammaJetHistProducer.exe $i 80 9999 30 $icent $inFile $suffix || exit 1
    done
  done
done

#################################
# PbPb DATA
# yskim files for Jet Systematics
yskimFilesHI=(
"yskim_HiForestPhoton-v7-noDuplicate_JetResCorrNO_smearingNO.root"
"yskim_HiForestPhoton-v7-noDuplicate_JetResCorr_smearingNO.root"
"yskim_HiForestPhoton-v7-noDuplicate_JetResCorrNO_smearingNO_JES102.root"
"yskim_HiForestPhoton-v7-noDuplicate_JetResCorrNO_smearingNO_JES098.root"
);
prefixYskimHI="yskim_HiForestPhoton-v7-noDuplicate_";

# PbPb DATA
for yskimFile in "${yskimFilesHI[@]}"
do
  inFile="yskimmedFiles/"$yskimFile;

  # delete prefix of file name so that we can get the suffix
  suffixTMP=${yskimFile#$prefixYskimHI};
  suffix=${suffixTMP%.root};   # trim ".root"
  for i in 0
  do
    for icent in 10010 11030 13050 15099 10030 13099
    do
        ./gammaJetHistProducer.exe $i 40 50 30 $icent $inFile $suffix || exit 1
        ./gammaJetHistProducer.exe $i 50 60 30 $icent $inFile $suffix || exit 1
        ./gammaJetHistProducer.exe $i 60 80 30 $icent $inFile $suffix || exit 1
        ./gammaJetHistProducer.exe $i 80 9999 30 $icent $inFile $suffix || exit 1
    done
  done
done

