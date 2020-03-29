all: clean unitTest
clean:
	-rm *.o
	-rm unitTest
	(cd ../../mcl/linux; make clean)
	(cd ../../wit/linux; make clean)

LIBRARY_PATH=-L../../wit/linux/
LIBRARY_PATH+=-L ../../mcl/linux
LIBRARY_PATH+=-L /opt/ibm/ILOG/CPLEX_Studio_Community129/cplex/lib/x86-64_linux/static_pic

C_INCLUDE_PATH=../../wit/src

OBJECTS=../src/scoFile.o ../src/unitTest.o ../src/CoinFinite.o ../src/visorProblem.o
libraries: wit mcl 
unitTest: $(OBJECTS)
	gcc -o unitTest $(OBJECTS) -lwit -lmcl -pthread -ldl -lstdc++ -lcplex -lm -I$(C_INCLUDE_PATH) $(LIBRARY_PATH)

mcl: 
	(cd ../../mcl/linux; make all exe_type=released)
wit: mcl 
	(cd ../../wit/linux; make all exe_type=released)
%.o:	../src/%.cpp
	gcc -c -o $@     ../src/$*.cpp -I$(C_INCLUDE_PATH)
