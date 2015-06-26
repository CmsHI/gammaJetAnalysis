#enum sampleType { kHIDATA, kHIMC, kPPDATA, kPPMC, kPADATA, kPAMC};
#                  0        1      2        3      4        5
#void gammaJetHistProducer(sampleType collision = kHIDATA,
#float photonPtThr=60, float photonPtThrUp=1000000, float jetPtThr=30, int icent =1)

g++ gammaJetHistProducer.C $(root-config --cflags --libs) -Wall -Wextra -g -o gammaJetHistProducer.exe

#pp data and MC
for i in  2 #3
do
    for icent in 10010 11030 13050 15099 10030 13099 7
    do
        ./gammaJetHistProducer.exe $i 40 50 30 $icent
        ./gammaJetHistProducer.exe $i 50 60 30 $icent
        ./gammaJetHistProducer.exe $i 60 80 30 $icent
        ./gammaJetHistProducer.exe $i 80 9999 30 $icent
        ./gammaJetHistProducer.exe $i 60 9999 30 $icent
        ./gammaJetHistProducer.exe $i 50 9999 30 $icent
        ./gammaJetHistProducer.exe $i 40 9999 30 $icent
     done
done

# #pp MC
# for i in 3
# do
#     for icent in 1   # gammaJetHistProducer.C does not care the centrality of pp MC
#     do
#         ./gammaJetHistProducer.exe $i 40 50 30 $icent
#         ./gammaJetHistProducer.exe $i 50 60 30 $icent
#         ./gammaJetHistProducer.exe $i 60 80 30 $icent
#         ./gammaJetHistProducer.exe $i 80 9999 30 $icent
#         ./gammaJetHistProducer.exe $i 60 9999 30 $icent
#         ./gammaJetHistProducer.exe $i 50 9999 30 $icent
#         ./gammaJetHistProducer.exe $i 40 9999 30 $icent
#      done
# done

# #PbPb data and MC
# for i in  0 1
# do
#     for icent in 10010 11030 13050 15099 10030 13099
#     do
#         ./gammaJetHistProducer.exe $i 40 50 30 $icent
#         ./gammaJetHistProducer.exe $i 50 60 30 $icent
#         ./gammaJetHistProducer.exe $i 60 80 30 $icent
#         ./gammaJetHistProducer.exe $i 80 9999 30 $icent
#         ./gammaJetHistProducer.exe $i 60 9999 30 $icent
#         ./gammaJetHistProducer.exe $i 50 9999 30 $icent
#         ./gammaJetHistProducer.exe $i 40 9999 30 $icent
#     done
# done



# #pPb data and MC
# for i in 4 5
# do
#     for icent in 1 #2 3
#     do
# 	./gammaJetHistProducer.exe $i 40 50 30 $icent
# 	./gammaJetHistProducer.exe $i 50 60 30 $icent
# 	./gammaJetHistProducer.exe $i 60 80 30 $icent
# 	./gammaJetHistProducer.exe $i 80 9999 30 $icent
#     done
# done
