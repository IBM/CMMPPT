#!/bin/bash

rm *.o
rm unitTest

export LIBRARY_PATH=../../wit/linux/:../../mcl/linux:/opt/ibm/ILOG/CPLEX_Studio_Community129/cplex/lib/x86-64_linux/static_pic
export C_INCLUDE_PATH="../../wit/src"


gcc -I$C_INCLUDE_PATH -c -o visor.o           ../src/visor.cpp
gcc -I$C_INCLUDE_PATH -c -o scoFile.o         ../src/scoFile.cpp
gcc -I$C_INCLUDE_PATH -c -o unitTest.o        ../src/unitTest.cpp
gcc -I$C_INCLUDE_PATH -c -o VisorFinite.o      ../src/VisorFinite.cpp
gcc -I$C_INCLUDE_PATH -c -o visorProblem.o    ../src/visorProblem.cpp

gcc -o visor visor.o scoFile.o visorProblem.o VisorFinite.o -lwit -lmcl -pthread -ldl -lstdc++ -lcplex -lm
gcc -o unitTest unitTest.o scoFile.o visorProblem.o VisorFinite.o -lwit -lmcl -pthread -ldl -lstdc++ -lcplex -lm
