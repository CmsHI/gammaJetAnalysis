####void forest2yskim_minbias_forestV3(TString inputFile_="forestFiles/HiForest4/HiForest_PbPb_MinBias_Track8_Jet5_GR_R_53_LV2B_merged_forest_0.root",
    ####                               sampleType colli=kHIDATA,
        #                           int maxEvent = -1,
         #                          bool useGenJetColl = 0,
          #                          TString jetAlgo="akPu3PF"
            #                        )
                                                                                                               
#enum sampleType { kHIDATA, kHIMC, kPPDATA, kPPMC, kPADATA, kPAMC};                                                          
#                   0 (X)    1 (x)  2       3       4        5                                                  


# PbPb
root -l -q -b 'forest2yskim_minbias_tracks.C+("forestFiles/0.root", 0,  -1)'
#root -l -q -b 'forest2yskim_minbias_forestV3.C++("forestFiles/0.root", 0,  -1, 0, "akPu3PF")'
