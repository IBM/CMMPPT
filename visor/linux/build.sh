#!/bin/bash

rm *.o
rm unitTest

export LIBRARY_PATH=~/witFromGithub/CMMPPT/wit/linux/:~/witFromGithub/CMMPPT/mcl/linux:/opt/ibm/ILOG/CPLEX_Studio_Community129/cplex/lib/x86-64_linux/static_pic
export C_INCLUDE_PATH=~/witFromGithub/CMMPPT/wit/src


gcc -c -o scoFile.o     ../src/scoFile.cpp
gcc -c -o unitTest.o    ../src/unitTest.cpp

gcc -o unitTest unitTest.o scoFile.o -lwit -lmcl -pthread -ldl -lstdc++ -lcplex -lm
