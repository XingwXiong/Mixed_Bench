#!/bin/bash


./single_thread ../dataset/part-00000 ../dataset/part-00000 ./output/part-00000.out.s.mtx

./multi_thread ../dataset/part-00000 ../dataset/part-00000 ./output/part-00000.out.m.mtx 4

