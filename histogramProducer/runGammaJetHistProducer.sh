#enum sampleType { kHIDATA, kHIMC, kPPDATA, kPPMC, kPADATA, kPAMC};
#                  0        1      2        3      4        5
#void gammaJetHistProducer(sampleType collision = kHIDATA,
#float photonPtThr=60, float photonPtThrUp=1000000, float jetPtThr=30, int icent =1)

g++ gammaJetHistProducer.C $(root-config --cflags --libs) -Wall -Wextra -g -o gammaJetHistProducer.exe

# #pp data
# for i in  2 #3
# do
#     ./gammaJetHistProducer.exe $i 40 50 30 7
#     ./gammaJetHistProducer.exe $i 50 60 30 7
#     ./gammaJetHistProducer.exe $i 60 80 30 7
#     ./gammaJetHistProducer.exe $i 80 9999 30 7
#     ./gammaJetHistProducer.exe $i 60 9999 30 7
#     ./gammaJetHistProducer.exe $i 50 9999 30 7
#     ./gammaJetHistProducer.exe $i 40 9999 30 7
# done

# #PbPb data MC
# for i in  0 1
# do
#     for icent in 10030 13099
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



#pPb data and MC
for i in 4 5
do
    for icent in 1 #2 3
    do
	./gammaJetHistProducer.exe $i 40 50 30 $icent
	./gammaJetHistProducer.exe $i 50 60 30 $icent
	./gammaJetHistProducer.exe $i 60 80 30 $icent
	./gammaJetHistProducer.exe $i 80 9999 30 $icent
    done
done
