// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL -- inserted includes.
/* File yktscen.C
   created by Joe Huizenga
   08:40:12 Wed Jan  5 1994. */

/* IBM CONFIDENTIAL */

/* COPYRIGHT IBM CORP 1994
   LICENSED MATERIAL - PROGRAM PROPERTY OF IBM
   REFER TO COPYRIGHT INSTRUCTIONS FORM NO. G120-2083 */

/* This file provides an implementation of the LgFrYktScen class. */

/* includes */

#include <stdlib.h>
#include <iostream.h>
#include <yktscen.h>
#include <wit/src/wit.h>
#include <scenario/src/scenario.h>
#include <scenario/src/inPrFile.h>
#include <scenario/src/wrkgProb.h>

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// Important: This class was used to diagnose a problem in Rochesters           !!
// scenario but is defunct right now.                                           !!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

LgFrYktScenario::LgFrYktScenario(const LgFrInitialProblemAndParameters& initProb, scheduler& )
:LgFrScenario(initProb)
{

   // Get reference to Scenario's lastSolvedProblem
    LgFrLastProblemSolved & lps = lastProblemSolved();

   // Get reference to Scenario's setOfParts
    LgFrSetOfParts & sop = setOfParts();

   // Get reference to Scenario's setOfDemands
    LgFrSetOfDemands & sod = setOfDemands();

   // Get reference to Scenario's workingImplosionProblem
    LgFrWorkingImplosionProblem & wip = workingImplosionProblem();

   fflush(stdout);
   cout <<"LgFrYktScenario::LgFrYktScenario about to invoke lps.witRun()" <<endl;
   fflush(stdout);

   WitRun *myrun =lps.witRun();

   fflush(stdout);
   cout <<"LgFrYktScenario::LgFrYktScenario returned from lps.witRun()" <<endl;
   fflush(stdout);

    int nP;
    witGetNPeriods(myrun, &nP );
    assert (nP == 8);
    cout << "Problem has " << nP << "periods.";

   // Get number of periods in problem
    int nPeriods = lps.numberOfPeriods();

   // Set heuristic implosion method (could set to optimize).
    wip.implosionMethod(LgFrHEURISTIC);

   // Turn on computing critical list
    lps.computeCriticalList(true);

   // Perform Implosion
    newImplosionSolution();

#if 0
   // Get critical list from lps
    IntGenMat cl(lps.criticalList());

   // Display critical list
    cout <<endl <<"******Critical List*******" <<endl;
    int i = 0; // Pulled out of the for below by RW2STL
    for ( i=0; i<cl.rows(); i++ )
    {
      // Print critical partname and critical period
	cout <<"Part Name: "
	  <<sop[cl(i,0)].name()
	  <<"   period: "
	  <<cl(i,1)
	  <<endl;
    }
#endif

   // Get critical list from lps
   LgFrSortingPartScheduleFloat cl(lps.criticalList());

   // Display critical list
   cout <<endl <<"******Critical List*******" <<endl;
   cout <<cl.format();


   fflush(stdout);
   cout <<"LgFrYktScenario::LgFrYktScenario returned from lps.witRun()" <<endl;
   fflush(stdout);
}

LgFrYktScenario::~LgFrYktScenario()
{
}

#ifdef NDEBUG
#undef NDEBUG
#endif

// Self-test.  This is not a very thourough test.
void
LgFrYktScenario::test()
{
  // Make objects needed by the tests
  LgFrInitialProblemFromFile 
    ip ("../../../../wit/wit-data/standard/obj1.bounds/wit.data");
  int tmpi = 3;

  // Make a LgFrYktScenario
  LgFrYktScenario ys (ip, tmpi);
}
