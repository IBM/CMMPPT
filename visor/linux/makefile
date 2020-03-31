all: unitTest visor
clean:
	-rm *.o
	-rm unitTest
	(cd ../../mcl/linux; make clean)
	(cd ../../wit/linux; make clean)

LIBRARY_PATH=-L../../wit/linux/
LIBRARY_PATH+=-L ../../mcl/linux
LIBRARY_PATH+=-L /opt/ibm/ILOG/CPLEX_Studio_Community129/cplex/lib/x86-64_linux/static_pic

C_INCLUDE_PATH=../../wit/src

OBJECTS=../src/scoFile.o ../src/VisorFinite.o ../src/visorProblem1.o ../src/visorProblem2.o

LIBRARIES=../../wit/linux/libwit.a ../../mcl/linux/libmcl.a 

unitTest: $(OBJECTS) ../src/unitTest.o $(LIBRARIES)
	gcc -o unitTest ../src/unitTest.o $(OBJECTS) -lwit -lmcl -pthread -ldl -lstdc++ -lcplex -lm -I$(C_INCLUDE_PATH) $(LIBRARY_PATH)
visor: $(OBJECTS) ../src/visor.o $(LIBRARIES)
	gcc -o visor ../src/visor.o $(OBJECTS) -lwit -lmcl -pthread -ldl -lstdc++ -lcplex -lm -I$(C_INCLUDE_PATH) $(LIBRARY_PATH)

../../mcl/linux/libmcl.a: 
	(cd ../../mcl/linux; make all exe_type=released)
../../wit/linux/libwit.a: ../../mcl/linux/libmcl.a
	(cd ../../wit/linux; make all exe_type=released)
%.o:	../src/%.cpp
	gcc -c -o $@     ../src/$*.cpp -I$(C_INCLUDE_PATH)
