#include <mcl/src/mdsPragma.h>
// RW2STL - inserted:
#include <string>
#include <ostream>
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL - inserted includes.
#include <stdlib.h>
#include <iostream>
#include <float.h>

#include <mcl/src/mcl.h>
#include <scenario/src/calendar.h>
#include <scenario/src/scenDrct.h>
#include <scenario/src/schdDeSt.h>

#include <sce/src/scDeSce.h>
#include <sce/src/difSceF.h>
#include <sce/src/scBdSce.h>
#include <sce/src/scenSce.h>
#include <sce/src/scenSceP.h>
#include <sce/src/mPlantH.h>

#ifdef SCE_ODBC
#include <sce/src/db.h>
#endif

// Uncomment this line if you want to build a version of SCE that
// is ONLY capable of scrubbing
// #define BUILD_AS_SCRUBBER_ONLY

//-----------------------------------------------------------------------
//
// SCE Main program
//
//-----------------------------------------------------------------------
int main(int argc, char * argv[])
{

#ifdef BUILD_AS_SCRUBBER_ONLY
  std::cout << "\nScrub Utility for Supply Capability Engine" << std::endl;
  std::cout << "Version: 7.01,  20120409 " << std::endl;
#else
  std::cout << "\nSupply Capability Engine" << std::endl;
  std::cout << "Version: 7.01,  20120409 " << std::endl;
#endif

  std::string platform("unknown");
#ifdef PLATFORM
  platform = XSTR( PLATFORM );
  if (platform == "1")
    platform = "linux";
#endif

#ifdef WIN32
  platform += " Win32";
#endif

#ifdef CYGWIN
  platform += " Cygwin";
#endif



#ifdef WIN64
  platform += " Win64";
#endif




  std::string exe_type("unknown");
#ifdef EXE_TYPE
  exe_type = XSTR( EXE_TYPE );
#endif

  std::cout << "Platform: " << platform << std::endl;
  std::cout << "Build type: " << exe_type  << std::endl ;
  std::cout << "Build date: " <<  __DATE__  << std::endl << std::endl;

  std::cout << "(C) COPYRIGHT International Business Machines Corp. 1995, 1997, 2011" << std::endl;
  std::cout << "All Rights Reserved" << std::endl;
  std::cout << "Property of IBM\n" << std::endl;


  LgFrCalendar calendar(8, LgFrDatetime("8/01/95"));
  LgFrCalendar & theCal = calendar;

#ifdef SCE_ODBC
  // ==========
  // ODBC Setup: SCE's DB api's need updating.  they are synched to SCE 2.0
  // ==========
  if ( argc != 6 && argc != 3 ) {
    std::cout << "Error.  Usage:  scemain <parameterFileName>  <dbName>" << std::endl;
    std::cout << "     or Usage:  scemain <parameterFileName> <dbName> " << std::endl;
    std::cout << "                       <dbInstance> <userID> <password>" << std::endl;
    exit(1);
  }
  SceDBProfile * pDB;
  if (argc == 6)    {
      pDB = new SceDBProfile(argv[2],argv[3],argv[4],argv[5]);
    }
  else      {
      pDB = new SceDBProfile(argv[2]);
    }
  std::cout << std::endl << "Connecting the database ... " << std::endl;
  if (! pDB -> setup())    {
      std::cout<< "Database connection failed." << std::endl;
      exit(1);
    }
  else
    std::cout<< "Database connection SUCCESSFUL!" <<std::endl<<std::endl;
#else


#ifdef BUILD_AS_SCRUBBER_ONLY
  if ( argc !=4 ) {
      std::cout << "Error.  Usage is: sceScrub <parameterFileName> <T> <scrubDir>" << std::endl;
      exit(8);
  }
#else

  if ( argc != 2 && argc !=4 ) {
    std::cout << "Error.  Usage is: scemain <parameterFileName> " << std::endl;
    exit(8);
  }


#endif

#endif


  std::string parameterFileName (argv[1]);


  LgFrDataInterfaceImplementation * difSceFImpPtr;
  if (argc == 4)
    {
      if (argv[2][0] == 'T')
        difSceFImpPtr = new LgFrDataInterfaceImplementationFromSceFiles
          (parameterFileName, argv[3], true, true);
      else if (argv[2][0] == 'F') {
        difSceFImpPtr = new LgFrDataInterfaceImplementationFromSceFiles
          (parameterFileName, argv[3], true, false);
#ifdef BUILD_AS_SCRUBBER_ONLY
        std::cout << "Error.  <scrubOnly> parameter MUST be \"T\"  " << std::endl;
        std::cout << "SCE terminates" << std::endl;
        exit(8);
#endif
      }
      else
        {
          std::cout << "Error.  <scrubOnly> argument MUST be \"T\" or \"F\" " << std::endl;
          exit(8);
        }
    }
  else
    {
      difSceFImpPtr
        = new LgFrDataInterfaceImplementationFromSceFiles(parameterFileName);
    }

  registerSceFilesDifImpFunctions(difSceFImpPtr);

  LgFrDataInterface dif(difSceFImpPtr);
  delete difSceFImpPtr;

  // Create a scenario using the builder
  LgFrScenarioBuilderForSce builder;
  LgFrScenarioDirector director(&builder);
  // ==================================
  // director oversees the construction of:  witRun, scenario, and 
  // initialProblem.   
  //     construction of initialProblem object does a lot of work.  
  //     It reads all the input files and builds the in-memory wit model).
  // ==================================
  director.construct(dif, calendar);
  LgFrScenarioForSceSmartPointer scenarioSP = builder.getScenario();

  // this is just a test to ensure that purify is working
  float * intentionalMemoryLeak = new float[1];

  LgFrSceSetOfParameters & setOfParameters = scenarioSP->setOfParameters();

  LgFrSceScrubber * myScrubber = scenarioSP -> scrubber();

  // ========================================================
  // === Print any model structure outputs
  // (like echo's, heurWitDataFile, bogons, lowestLevel) etc.
  // ========================================================
  scenarioSP->printModelStructureOutputs();




  // ==========================
  // === Run an MRP engine ===
  // ==========================
  if (! myScrubber-> scrubOnly())
    {
      scenarioSP->runExplosion(scenarioSP);
      scenarioSP->printExplosionResults();
    }

  // ============================
  // === Run IMPLOSION engine ===-
  // ============================
  bool doImplode = setOfParameters.getBool("doImplode");
  
  if (! myScrubber-> scrubOnly())
    {
      if (doImplode)
	scenarioSP->runImplosion(scenarioSP);
      
      // =====================================================================
      // Call another API routine to do all the output stuff
      if (doImplode) {
	
#ifdef SCE_ODBC
	std::cout << "Populate the database with output results ..." << std::endl;
	scenarioSP->analyzeRunAndUpdateDB();
#else
	// don't do this if your in scrub only mode
	if (! myScrubber-> scrubOnly())        {
	  std::cout << "Writing output results ..." << std::endl;
	  scenarioSP->analyzeRunAndPrintResults();
	}
#endif

#ifdef MULTIPLE_IMPLOSION_RUNS
	// testbed for future ability to keep live SCE model.    
	// future methods for changing a demand

	std::string demandUpdateFile("newDemandUpdate.dat");
	scenarioSP->setDemandVols(demandUpdateFile);

	std::cout << "Running a second Implosion Problem Now ..." << std::endl;	
	scenarioSP->runImplosion(scenarioSP);
	std::cout << "second Implosion Complete ... will now analyze and print results ..." << std::endl;	
	scenarioSP->analyzeRunAndPrintResults();
#endif	

      }
    }
  
  // =====================================================================
  // ==================================================
  // ==== Run the FSS Engines =========================
  // ==================================================
  bool fssFlag = setOfParameters.getBool("fssFlag");
  
  if (doImplode && fssFlag)  {
    if (! myScrubber-> scrubOnly())        {
      std::cout << "Running FSS Engines ... " << std::endl;
    }
    
    if (! myScrubber-> scrubOnly())        {
      if (setOfParameters.getBool("doFssSingleton")) {
        std::cout << "Singleton Queries ... "
		  << "(method:" << setOfParameters.getString("fssSingletonMethod") << "    "
		  << "featureMethod:" << setOfParameters.getString("fssFeatureSingletonMethod")
             << ")" << std::endl;
        scenarioSP->singletonFss();
      }
    }
    
    if (setOfParameters.getBool("doFssGeneralSet")) {
      std::cout << "General Set Queries ... "
           << "(method:" << setOfParameters.getString("fssGeneralSetMethod") << "    "
		<< "featureMethod:" << setOfParameters.getString("fssFeatureGeneralSetMethod")
		<< ")" << std::endl;
      scenarioSP->generalSetFss();
    }

    if (setOfParameters.getBool("doFssMachine")) {
      std::cout << "Machine Queries ... "
		<< "(method:" << setOfParameters.getString("fssMachineMethod") << "    "
		<< "featureMethod:" << setOfParameters.getString("fssFeatureMachineMethod")
           << ")" << std::endl;
      scenarioSP->machineFss();
    }
  }

  
  

#ifdef SCE_ODBC

  if (pDB)
    delete pDB;

#endif

  // figure out what return code to pass
  MclFacility * myFacil = scenarioSP->getErrFacility();
  int level = myFacil -> maxSeverityOfMsgsIssued();



  //  delete the guts of scenario
  scenarioSP->deleteAppDataAndScenarioObjects();





  std::cout << "SCE Completed.  Return Code:" << std::flush;

  // max message issued was "info"
  if (level == 0) {
    std::cout << " 0" << std::endl;
    return 0;
  }
  // max message issued was "warning"
  if (level == 1) {
    std::cout << " 4" << std::endl;
    return 4;
  }
  // max message issued was "error" or worse
  if (level > 1) {
    std::cout << " 8" << std::endl;
    return 8;
  }

}
