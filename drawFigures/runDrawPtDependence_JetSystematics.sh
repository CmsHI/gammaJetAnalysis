#!/bin/bash

#void drawPtDependence(TString dirName="nominal", int prodDate=20131021, 
# int jetPtCut=30, TString fNameSuffix = "", TString outNameSuffix = "");

g++ drawPtDependence.C $(root-config --cflags --libs) -Wall -Wextra -g -o drawPtDependence.exe
g++ drawPtDependencePA.C $(root-config --cflags --libs) -Wall -Wextra -g -o drawPtDependencePA.exe

dirName="JetSystematics";
prodDate=20150716;
jetPtCut=30;
photonTrackSuffixArr=(
"JetResCorrNO_smearingNO"    # nominal
"JetResCorr_smearingNO"
"JetResCorrNO_smearingNO_JES102"
"JetResCorrNO_smearingNO_JES098"
);

for suffix in "${photonTrackSuffixArr[@]}"
do
  outSuffix=$suffix;
  # output file will be named  "resultHistograms_"$outSuffix".root"
  rm "resultHistograms_"$outSuffix".root"  # delete the output from a previous run
  ./drawPtDependence.exe $dirName $prodDate $jetPtCut $suffix $outSuffix
  ./drawPtDependencePA.exe $dirName $prodDate $jetPtCut $suffix $outSuffix
done

#################################
## for pp DATA there is  additional correction for smearing
photonTrackSuffixArrPP=(
"JetResCorrNO_smearingNO"
"JetResCorrNO_smearing0010"
"JetResCorrNO_smearing1030"
"JetResCorrNO_smearing3050"
"JetResCorrNO_smearing5099"
"JetResCorrNO_smearing0030"
"JetResCorrNO_smearing3099"
"JetResCorr_smearingNO"
"JetResCorrNO_smearingNO_JES102"
"JetResCorrNO_smearingNO_JES098"
);
