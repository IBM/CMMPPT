// ---------------------------------------------------------------|
//                                                                |
//                                                                |
// Usage: aMrpMain <wit_data_file_name>                           |
//                 <----mandatory----->                           |
//                                                                |
//----------------------------------------------------------------|
 
 
 
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <wit.h>
#include <allocMrp.h>
#include <witutil/src/machdep.h> 

#include <iostream>
#include <fstream>

int main (int argc, char * argv[]) 

{
    WitRun * theWitRun;
    int version;
    
    // Mandatory Argument: wit_data_file_name    
   if ( argc < 2 || argc > 3 ) {
      printf("Error.  Usage is %s wit_data_file_name  <version>\n", argv[0]);
      exit(1);
      }

    if (argc==2) 
       version = 1;
    else
       version = atoi(argv[2]);
    
    witNewRun(&theWitRun);

    witSetMesgTimesPrint(theWitRun, WitTRUE, WitINFORMATIONAL_MESSAGES, 0);

    witInitialize(theWitRun);

    // read the wit data file
    witReadData(theWitRun, argv[1]);

    // construct an allocMpr
    WUtlAllocatedMrp * myAllocMrp = new WUtlAllocatedMrp(theWitRun, '#');

    // compute the allocMrp results
    std::cout << "Computing allocatedMrp ... " << std::endl;
    int rc = myAllocMrp->computeAndPopulate(version);

    if (rc == 4) {
      std::cout << "Warnings happened, continuing to print" << std::endl;
    }

    if (rc == 8) {
      std::cout << "Errors happened, terminating program" << std::endl;
      exit(rc);
    }

    // Print the results
    std::cout << "Printing results ... " << std::endl;
    rc = myAllocMrp->print("allocMrp.output");
    if (rc == 8) {
      std::cout << "Errors happened, terminating program" << std::endl;
      exit(rc);
    }

    // delete the object
    delete myAllocMrp;

    // delete the original witRun
    witDeleteRun(theWitRun);

    exit(0);
}





