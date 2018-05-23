#!/bin/bash


./single_thread ../dataset/test.data.01/test.data.01.txt ../dataset/test.data.02/test.data.01.txt ./output/s.f
mkdir ./output/s.d
./single_thread ../dataset/test.data.01/ ../dataset/test.data.02/ ./output/s.d

./multi_thread ../dataset/test.data.01/test.data.01.txt ../dataset/test.data.02/test.data.01.txt ./output/m.f 3
mkdir ./output/m.d
./multi_thread ../dataset/test.data.01/ ../dataset/test.data.02/ ./output/m.d 3

#./single_thread ../dataset/gen_text_20_15000_80_wiki_noSW_90_Sampling_all ../dataset/gen_text_25_1500_1000_wiki_noSW_90_Sampling_all ./output/20.25.s.f
#./single_thread ../dataset/gen_text_20_15000_80_head_0x4000 ../dataset/gen_text_25_1500_1000_head_0x10002 ./output/20.25.head.s.f
#./single_thread ../dataset/gen_text_25_1500_1000_wiki_noSW_90_Sampling_all ../dataset/gen_text_25_1500_1000_wiki_noSW_90_Sampling_all ./output/25.25.s.f
#mkdir ./output/25.25.s.d
#./single_thread ../dataset/gen_text_25_1500_1000 ../dataset/gen_text_25_1500_1000 ./output/25.25.s.d
#
#./multi_thread ../dataset/gen_text_25_1500_1000_wiki_noSW_90_Sampling_all ../dataset/gen_text_25_1500_1000_wiki_noSW_90_Sampling_all ./output/25.25.m.f 3
#mkdir ./output/25.25.m.d
#./multi_thread ../dataset/gen_text_25_1500_1000 ../dataset/gen_text_25_1500_1000 ./output/25.25.m.d 3
