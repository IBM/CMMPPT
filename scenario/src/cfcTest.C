/* File fcwit.C created by Jodi Miller on Tue Jan 24 1995.
   Classification:  IBM Confidential
   Copyright IBM Corp 1995
   Licensed material - Program property of IBM
   Refer to copyright instructions form no. G120-2083
*/

/* Change Activity: */
/* End Change Activity */

static const char ibmid[] = "Copyright IBM Corporation 1995 LICENSED MATERIAL - PROGRAM PROPERTY OF IBM";

#define YKT_COMPILE

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream.h>
#include <wit/src/wit.h>
#include <sys/types.h>
#include <sys/wait.h>

#ifndef YKT_COMPILE
#include "geoglimits.H"
#include <periods.H>
#include "deviceksb.H"
#include "devconkss.H"
#include "ltrules.H"
#include "ltkgrules.H"
#include "orderkss.H"
#endif

#include <rw/rstream.h>
#include <scenario/src/calendar.h>
#include <scenario/src/difSFile.h>
#include <scenario/src/scBdDflt.h>
#include <scenario/src/scenario.h>
#include <scenario/src/scenDrct.h>
#include <scenario/src/inPrFile.h>
#include <scenario/src/schFctry.h>
#include <scenario/src/outInter.h>

#include <scenario/src/msItemIt.h>
#include <scenario/src/msAttrIt.h>
#include <scenario/src/timeVecs.h>
#include <scenario/src/sOfDemds.h>
#include <scenario/src/demdNmFS.h>
#include <scenario/src/fssCtrl.h>
#include <scenario/src/demdSchP.h>

/* ******************************************** */
/* global definitions - put here for speed	*/
/* ******************************************** */
 
char STARTDATE[] = "19950201";
char RWSTARTDATE[] = "02/01/95";
size_t NUMBEROFPERIODS = 15;
char *SAVEDFILENAME = "/afs/rchland.ibm.com/usr6/jmiller/combatpp/wit.problem";
extern char *GEOGRAPHYLIMITS;
extern char *DEVICECONSTRAINTS;
extern char *DEVICES;
extern char *ORDERS;
extern char *LEADTIMERULES;
char *ALL = "ALL";
char *GENERIC_MCHT = "****";
char *GENERIC_MODEL = "***";
char *UNITED_STATES = "USA";
char *WORLD_TRADE = "WTD";
char *GENERIC_GEOGRAPHY = "***";

#define NOT_SET "00000000"
#define HIGH_PRIORITY 1
#define MEDIUM_PRIORITY 3
#define LOW_PRIORITY 5

void checkWitRc(char *funcName, int rc)
{
    if (rc >= WitSEVERE_RC)
    {
	printf("Severe problem when calling %s.\n", funcName);
	exit(1);
    }
    else
    {
	if (rc >= WitWARNING_RC)
	{
//	    printf("Warning message condition occurred when calling %s.\n", funcName);
	}
    }
}


int initialize_problem(
   WitRun * theWitRun)
{
#ifndef YKT_COMPILE
    geolimits geol;
    geolimits::Cursor glcrsr(geol);
    periods perclass(STARTDATE, NUMBEROFPERIODS);
    ltrules ltr;
    ltkgrules ltk;
    orderkss orders;
    orderkss::Cursor ordcrsr(orders);
    deviceksb devices;
    deviceksb::Cursor devcrsr(devices);
    deviceConstraintKss devcon;

    // WitRun * theWitRun;
    witReturnCode rc;
    int nPeriods = NUMBEROFPERIODS;
    int i;
    float temptt;
    float temppt;
    char tempind;
    float *cycletime;
    float *demand;
    int *priority;
    int tempint;
    int period;
    unsigned addconstrained;

/* Establish environment for the WIT problem */
    // rc = witNewRun(&theWitRun);
    // checkWitRc("witNewRun", rc);

/* Set up messaging.						*/
/* Send messages to the file wit.out instead of typescript.	*/
    rc = witSetMesgTimesPrint(theWitRun, WitTRUE,
			      WitINFORMATIONAL_MESSAGES, 0);
    rc = witSetMesgTimesPrint(theWitRun, WitTRUE, WitWARNING_MESSAGES, 0);

    witSetMesgFileAccessMode(theWitRun, WitTRUE, "w+");
    witSetMesgFileName(theWitRun, WitTRUE, "wit.out");

/* Set flag so WIT will return to this program after a severe error */
    rc = witSetMesgStopRunning(theWitRun, WitTRUE,
				WitSEVERE_MESSAGES, WitFALSE);
    checkWitRc("witSetMesgStopRunning", rc);

/* Initialize WIT */
    rc = witInitialize(theWitRun);
    checkWitRc("witInitialize", rc);
  
    witSetMesgTimesPrint( theWitRun, WitTRUE, 538, 0 );
    witSetWit34Compatible( theWitRun, WitTRUE );

/* Set the title, number of periods and objective function choice */
    rc = witSetTitle(theWitRun, "CFC Initial Problem");
    checkWitRc("witSetTitle", rc);

    rc = witSetNPeriods(theWitRun, nPeriods);
    checkWitRc("witSetNPeriods", rc);

    rc = witSetObjChoice(theWitRun, 0);
    checkWitRc("witSetObjChoice", rc);

/* Set up the geography limits to be used as constrained parts in WIT*/
   rc = geol.addconstraintPNs(&theWitRun, perclass, nPeriods);
    if (rc != 0)
	return rc;

/* Loop through the orders and set up the parts/demands	*/
    cycletime = (float *)malloc(nPeriods*sizeof(float));
    demand = (float *)malloc(nPeriods*sizeof(float));
    priority = (int *)malloc(nPeriods*sizeof(int));
    IString demandname;
    IString demandpartname;

    forCursor(ordcrsr)
    {
	if (orders.elementAt(ordcrsr).RPSSD() == NOT_SET)
	{   /* calculate the RPSSD date	*/
	    /* set the compare date	*/
	    rc = orders.elementAt(ordcrsr).calc_RPSSD(perclass, ltr, ltk);
            /* get the lead time for the system	*/	   
	    rc = ltk.get_leadtime(ltr, orders.elementAt(ordcrsr).GEOG(), orders.elementAt(ordcrsr).AREA(), orders.elementAt(ordcrsr).MCHT(), orders.elementAt(ordcrsr).MODL(), &temppt, &temptt, &tempind);
	    if (rc == 0)
	    {
		/* note exception	*/
	        cout<<"No leadtime for "<<orders.elementAt(ordcrsr).SYST()<<"  "<<
		  orders.elementAt(ordcrsr).MFGN()<<endl;
	        continue;
	    }
  	    /* set the priority	*/
	    tempint = LOW_PRIORITY;
	    /* set variable to load any constrained parts for this order into WIT */
	    addconstrained = 1;
	}
	else
	{
	    /* lead time is set to 0	*/
	    temppt = 0.;
	    /* set priority to highest */
	    tempint = HIGH_PRIORITY;
	    /* set constrained variable to not add constrained parts	*/
	    addconstrained = 0;
	}

	for (i = 0; i < nPeriods; i++)
	    cycletime[i] = temppt;

	    /* calculate where to put the demand	*/
	for (i = 0; i < nPeriods; i++)
	    demand[i] = 0.;
 	period = perclass.getperiod(orders.elementAt(ordcrsr).RPSSD());
	if (period == -1.)
	{
	    cout<<"No period returned for "<<orders.elementAt(ordcrsr).SYST()<<"  "<<
	      orders.elementAt(ordcrsr).MFGN()<<"  "<<orders.elementAt(ordcrsr).RPSSD()<<endl;
	    continue;
	}
	demand[period] = 1.;
	for (i = 0; i < nPeriods; i++)
	    priority[i] = tempint;

	    /* add the part:	*/
	demandname = orders.elementAt(ordcrsr).SYST();
	demandname += orders.elementAt(ordcrsr).MFGN();
	demandpartname = demandname;
	demandpartname += orders.elementAt(ordcrsr).MCHT();
	demandpartname += orders.elementAt(ordcrsr).MODL();
	rc = witAddPartVa(theWitRun, demandpartname, WitPRODUCT,
			  WitNcycleTime,	cycletime,
			  NULL);
	checkWitRc("witAddPartVa", rc);

	/* add any geography constraints	*/
	rc = geol.addconstraint2BOM(&theWitRun, demandpartname, orders.elementAt(ordcrsr).GEOG(), orders.elementAt(ordcrsr).MCHT(), orders.elementAt(ordcrsr).MODL());

	/* add any device constraints		*/
	if (addconstrained)
	  rc = devices.add_constraints(&theWitRun, demandpartname, devcon, nPeriods);

	/* add the demand	*/
	rc = witAddDemandVa(theWitRun, demandpartname, demandname,
			    WitNdemandVol,	demand,
			    WitNpriority,	priority,
			    NULL);
	checkWitRc("witAddDemandVa", rc);

//	rc = witSetDemandFocusHorizon(theWitRun, demandpartname, demandname, nPeriods-1);
//	checkWitRc("witSetDemandFocusHorizon", rc);
    }
    free(cycletime);
    free(demand);
    free(priority);

    rc = witSetComputeCriticalList(theWitRun, WitTRUE);
    checkWitRc("witSetComputeCriticalList", rc);

    /* write the problem out to a file	*/
    // rc = witWriteData(theWitRun, SAVEDFILENAME);
    // checkWitRc("witWriteData", rc);

    /* delete the problem in memory	*/
    // rc = witDeleteRun(theWitRun);
    // checkWitRc("witDeleteRun", rc);

    return rc;
#else
    // Building in yorktown, so get data from at data file
    witReturnCode rc;
    rc = witReadData( theWitRun, "../../../../wit/wit-data/examples/diner/wit.data");
    checkWitRc("witReadData",rc);
    return rc;
#endif
}


int create_scenario()
{
    int rc = 0;
    int j;
    int requestedPeriod;
    int actualPeriod;
    size_t i;
    LgFrSetOfPartsWithUnmovableParts  parts();

    LgFrCalendar calendar(NUMBEROFPERIODS, RWTime(RWDate(RWSTARTDATE)), LgFrDay);

    // LgFrFileDataInterface dif(SAVEDFILENAME);
    LgFrDataInterfaceSpecialPurposeFromFile
	dif("../../../../wit/wit-data/examples/diner/wit.data");


// Create a scenario using the builder
    LgFrScenarioBuilderForDefault builder;
    LgFrScenarioDirector director(&builder);  
    director.construct(dif, calendar);

    LgFrScenarioSmartPointer scenarioPtr = builder.getScenario();

// Implode
    scenarioPtr->newImplosionSolution ();

// Display the error log -JP's version

    {
    // Get schedules of the backlog, requested shipment, and actual shipment
//    LgFrScheduleFactory *sfPtr = scenarioPtr->scheduleFactory();
    LgFrSortingDemandScheduleFloatSmartPointer backlogSchedPtr = scenarioPtr->scheduleFactory()->newDemandSchedule(LGFRBacklogVolume);
    
    LgFrSortingDemandScheduleFloatSmartPointer reqShipSchedPtr = scenarioPtr->scheduleFactory()->newDemandSchedule(LGFRDemandVolume);
    LgFrSortingDemandScheduleFloatSmartPointer actualShipSchedPtr = scenarioPtr->scheduleFactory()->newDemandSchedule(LGFRShipmentVolume);
    
    const LgFrSortingDemandScheduleFloat reqShipSched( *reqShipSchedPtr );
    const LgFrSortingDemandScheduleFloat actualShipSched( *actualShipSchedPtr );


    // Loop once for each backlogged demand
    LgFrDemand backlogedDemand;
    LgFrTimeVecFloat reqShipTimeVec, actualShipTimeVec;

    for ( size_t i=0; i<backlogSchedPtr->entries(); i++ )
    {

       // Get i'th backloged demand
       backlogedDemand = backlogSchedPtr->demand( i );

       // Get timevecs of requested and actual shipment dates
       reqShipTimeVec    = (reqShipSched[backlogedDemand]).timeVecFloat();
       actualShipTimeVec = (actualShipSched[backlogedDemand]).timeVecFloat();

       // Print out backloged demand name
       cout <<"------" <<endl;
       cout <<"Backlogged demand: " <<backlogedDemand.name() <<endl;

       // Print out backloged part
       cout <<"  backlogged part: " <<backlogedDemand.partPtr()->name() <<endl;

       // Print out requested date
       for ( size_t j=0; j<reqShipTimeVec.length(); j++ )
       {
          if ( reqShipTimeVec[j] != 0 )
	  {
	      requestedPeriod = j;
	      cout <<"  requested period was " << j  <<endl;
	      cout <<"                       " <<calendar.periodStart(j).asString() <<endl;
          } 
       }

       // Print out actual date
       for ( j=0; j<actualShipTimeVec.length(); j++ )
       {
          if ( actualShipTimeVec[j] != 0 )
	  {
	      actualPeriod = j;
              cout <<"  actual shipment period is " << j  <<endl;
              cout <<"                            " <<calendar.periodStart(j).asString() <<endl;
          } 
       }
      // Determine the cause of the error (backlog).
      // This is done by determining the part which is short and causing the backlog.
      {

        // Clear all demands from the focus and add the current backlogged demand
	LgFrFocussedShortageScheduleControl &fssCtrl = scenarioPtr->focussedShortageScheduleControl();   
        fssCtrl.clearFocusHorizons();
	fssCtrl.focusHorizon(backlogedDemand, scenarioPtr->numberOfPeriods() - 1);
                        
        // Get a schedule which contains the short parts (focussedShortageSchedule).
        // It would be nice to use the schedule factory for this, but JP needs to talk to Wally first
        // about the way subscription's are currently working.

        LgFrSortingPartScheduleFloat fssSched =  fssCtrl.latestFocussedShortageSchedule();

        // Now write out the parts which caused the shortage (This is the cause of the error.)
        for ( j=0; j<fssSched.entries(); j++ )
	{

          // Print out the name of the short part.
	  cout <<"  Short part causing error: " <<fssSched.part(j).name() <<endl;
          // Print out the the amount of the shortage and the periods it occurs in as a timeVec
//          cout <<"  Shortage volume is: " <<fssSched[j].timeVecFloat().format() <<endl;

        }  // End of   for ( j=0; j<fssSched.entries(); j++ )

      }  // End of determining cause of error
    }   // End of   for ( size_t i=0; i<backLogSchedPtr.entries(); i++ )
    }  // End of Display error log

    return rc;
}

int main (int argc, char *argv[])
{   
    unsigned rc;
/*    pid_t pid;

    if ((pid = fork()) < 0)
	cout<<"fork error"<<endl;
    else
	if(pid == 0)  */
	{
	    // rc = initialize_problem();
          rc = 0;
	    if (rc == 0)
	    {
		create_scenario();
	    }
	    else
		cout<<"Failed to create the wit run"<<endl;
	}

    return 0;
}

/* Change Log
<@log@>

Tue Jan 24 1995  10:01:10  by Jodi Miller
<reason><version><Brief description and why change was made.>
*/
