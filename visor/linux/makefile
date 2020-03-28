all: clean build
clean:
	-rm *.o
	-rm unitTest
	(cd ../../mcl/linux; make clean)
	(cd ../../wit/linux; make clean)

LIBRARY_PATH=-L../../wit/linux/
LIBRARY_PATH+=-L ../../mcl/linux
LIBRARY_PATH+=-L /opt/ibm/ILOG/CPLEX_Studio_Community129/cplex/lib/x86-64_linux/static_pic

C_INCLUDE_PATH=../../wit/src

build: wit mcl
	gcc -c -o scoFile.o     ../src/scoFile.cpp -I$(C_INCLUDE_PATH)
	gcc -c -o unitTest.o    ../src/unitTest.cpp -I$(C_INCLUDE_PATH)
# gcc -o unitTest unitTest.o scoFile.o -lwit -lmcl -pthread -ldl -lstdc++ -lcplex -lm
	gcc -o unitTest unitTest.o scoFile.o  -lmcl -pthread -ldl -lstdc++ -lcplex -lm -I$(C_INCLUDE_PATH) $(LIBRARY_PATH)

mcl:
	(cd ../../mcl/linux; make all exe_type=released)
wit: mcl
	(cd ../../wit/linux; make all exe_type=released)


