#include <stdlib.h>
#include <fstream.h>
#include <rw/rstream.h>
#include <scenario/src/calendar.h>
#include <scenario/src/difPWcmr.h>
#include <scenario/src/difCmrpF.h>
#include <scenario/src/difImpl.h>
// #include <difMapic/src/difMapic.h>
#include <scenario/src/scBdCMRP.h>
#include <scenario/src/scenCMRP.h>
#include <scenario/src/scenDrct.h>
#include <scenario/src/ipCMRP.h>
#include <scenario/src/schFCMRP.h>
#include <scenario/src/outInter.h>
#include <scenario/src/schdDeSt.h>
#include <scenario/src/multPSch.h>
#include <scenario/src/multDSch.h>
#include <scenario/src/timeVecs.h>
#include <scenario/src/sOfPaFS.h>
#include <scenario/src/tvElemPC.h>
#include <scenario/src/zeroTvFS.h>
#include <scenario/src/boolFS.h>
#include <scenario/src/scCMRPP.h>
#include <scenario/src/demdSchP.h>
#include <scenario/src/partSchP.h>

  // -----------------------------------------------------------------
  // The PlannerManaged Window (Constrained Parts)
  // - supply                          (yes)
  // - req'd                           (yes)
  // - cum supply                      (yes)
  // - cum req'd                       (yes)
  // - usage (implosion)               (yes)
  // - excess (implosion)              (yes)
  // - stock  (implosion)              (yes)
  // - scrap  (implosion)              (yes)
  // -----------------------------------------------------------------
  // MRPManaged Window (UN-Constrained Parts)
  // - supply                           (yes)
  // - req'd                            (yes)
  // - cum supply                       (yes)
  // - cum req'd                        (yes)
  // - usage (implosion)                (yes)
  // - excess (implosion)               (yes)
  // - stock  (implosion)               (yes)
  // - scrap  (implosion)               (yes)
  // - procurement reciepts (implosion) (yes)
  // - production completion (implosion)(yes)
  // - total shipment (implosion)       (wibni -- needs work)
  // -----------------------------------------------------------------
  // Capacity Multischedule
  // - avail                            (yes)
  // - req'd                            (yes)
  // - usage                            (yes)
  // - utilization%                     (NA)
  // - excess                           (yes)
  // -----------------------------------------------------------------
  // DEMAND Multischedule
  // - demand quantity (editable)       (yes)
  // - shipment        (implosion)      (yes)
  // - cum demand qty  (computed)       (NA)
  // - cum shipment    (computed)       (NA)
  // - backlog         (implosion)      (yes)
  // - Revenue(times demand qty?)       (NA - wibni)
  // - priority        (editable?)      (NA - wibni)


// NOTE:  All the DUMP_DEBUG_SCHEDULES stuff is not necessary
//        for the GUI
// comment out this line to stop the schedule dumping
//
// define DUMP_SCHEDULES_TO_FILE


#ifdef  DUMP_SCHEDULES_TO_FILE
  LgFrSortingPartScheduleFloatSmartPointer PlannerManagedSupplyVol;
  LgFrSortingPartScheduleFloatSmartPointer PlannerManagedRequiredVol;
  LgFrSortingPartScheduleFloatSmartPointer PlannerManagedCumSupplyVol;
  LgFrSortingPartScheduleFloatSmartPointer PlannerManagedCumRequiredVol;
  LgFrSortingPartScheduleFloatSmartPointer PlannerManagedStockVol;
  LgFrSortingPartScheduleFloatSmartPointer PlannerManagedScrapVol;
  LgFrSortingPartScheduleFloatSmartPointer PlannerManagedUsageVol;
  LgFrSortingPartScheduleFloatSmartPointer PlannerManagedExcessVol;

  LgFrSortingPartScheduleFloatSmartPointer MRPManagedSupplyVol;
  LgFrSortingPartScheduleFloatSmartPointer MRPManagedRequiredVol;
  LgFrSortingPartScheduleFloatSmartPointer MRPManagedCumSupplyVol;
  LgFrSortingPartScheduleFloatSmartPointer MRPManagedCumRequiredVol;
  LgFrSortingPartScheduleFloatSmartPointer MRPManagedStockVol;
  LgFrSortingPartScheduleFloatSmartPointer MRPManagedScrapVol;
  LgFrSortingPartScheduleFloatSmartPointer MRPManagedUsageVol;
  LgFrSortingPartScheduleFloatSmartPointer MRPManagedExcessVol;
  LgFrSortingPartScheduleFloatSmartPointer ProductionVolume;
  LgFrSortingPartScheduleFloatSmartPointer ProcurementVol;
  LgFrSortingPartScheduleFloatSmartPointer CycleTime;
  LgFrSortingPartScheduleFloatSmartPointer ProcurementLeadTime;
  LgFrSortingPartScheduleFloatSmartPointer TotalShipmentVol;

  LgFrSortingDemandScheduleFloatSmartPointer DemandVolume;
  LgFrSortingDemandScheduleFloatSmartPointer ShipmentVolume;
  LgFrSortingDemandScheduleFloatSmartPointer BacklogVolume;

  LgFrSortingPartScheduleFloatSmartPointer CapacitySupplyVol;
  LgFrSortingPartScheduleFloatSmartPointer CapacityRequiredVol;
  LgFrSortingPartScheduleFloatSmartPointer CapacityUsageVol;
  LgFrSortingPartScheduleFloatSmartPointer CapacityExcessVol;



  void createAllTheDebugSchedules(const LgFrScheduleFactory * sFactoryPtr);
  void deleteAllTheDebugSchedules();
  void printThePlannerManagedDebugSchedules(
              const LgFrSolutionOutputInterface & soi, RWCString aTitle);
  void printTheMRPManagedDebugSchedules(
              const LgFrSolutionOutputInterface & soi, RWCString aTitle);
  void printTheDemandDebugSchedules(
              const LgFrSolutionOutputInterface & soi, RWCString aTitle);
  void printTheCapacityDebugSchedules(
              const LgFrSolutionOutputInterface & soi, RWCString aTitle);
#endif

inline
void
cmrpWriteSchedule (const LgFrSolutionOutputInterface & soi,
		   LgFrSortingScheduleFloatSmartPointer schedPtr,
		   const RWCString heading)
{
  soi.writeSchedule(*schedPtr, "testCMRP.out", heading, ios::out | ios::app);
}


// Read input from cin until end-of-line is read
void readToEol()
{
  const int len = 1000;
  char buffer[len];
  cin.getline (buffer, len);
}


// Set a demand focus on demand dName, part pName, for period period,
// in scenario *scenarioPtr
void
setFocus (RWCString dName, RWCString pName, int period,
	  LgFrScenarioForCMRPSmartPointer scenarioPtr)
{
    const LgFrDemand* demandPtr
	= scenarioPtr->constSetOfDemands().findValue(dName, pName);

    if (demandPtr == NULL)
	cerr << "INFO: Unable to set demand focus on " << dName
	   << ", " << pName << endl;
    else {
	cout << "INFO: Setting demand focus on " << dName << ", " << pName << endl;
	LgFrDemand demand = *demandPtr;

	// set a focus
	scenarioPtr
	    ->  focussedShortageScheduleControl(). focusHorizon  (demand,
								  period);
    }
}

//-----------------------------------------------------------------------
//
// CMRP Test/Batch Main program
//
// Command Line Format: cmrpTest [filename-prefix] [v] [outfile-name]
//
// All command line parameters are optional, but filename-prefix must be
// used if you want to use the second parameter and likewise for v if
// you want to use the third parameter
//
// If the filename parameter is given, it is used as the beginning part
// of a set of input file in the "CMRP" format documented in difCmrpF.h.
//
// If the second parameter is given, the output will be much more verbose
// than if it is not
//
// If the third parameter is given the financial impact (also known as
// procurement summary) multiSchedule  output will
// be written to the file with that name and the verbose option will
// be ignored
//
//-----------------------------------------------------------------------
int main(int argc, char * * argv)
{
  RWBoolean interactive;
  RWBoolean useFileData;
  RWBoolean verbosePrinting;
  RWBoolean procurementSumToFile;

  interactive = FALSE;
  useFileData = FALSE;
  verbosePrinting = FALSE;
  procurementSumToFile = FALSE;
  if (argc > 1) {
    // interactive = TRUE;	// Old version
    useFileData = TRUE;
  }
  if (argc > 2) {
    verbosePrinting = TRUE;
  }
  if (argc > 3) {
    verbosePrinting = FALSE;
    procurementSumToFile = TRUE;
  }

  const LgFrSchedulePrintStyle printStyle(useFileData  ?  CSV  :  TEXT);

  LgFrCalendar calendar;
  LgFrScenarioForCMRPSmartPointer scenarioPtr;

  // Initialize calendar and create a scenario
  if (useFileData) {
    RWCString baseFileName = *(argv+1);
    RWCString msg
      = "INFO: Using the set of CMRP input files named\n"
      + baseFileName + ".{raw,dem,bom,...}";
    cout << msg << endl;
    cerr << msg << endl;
    calendar =
      LgFrDataInterfaceImplementationFromCmrpFiles::calendar( baseFileName );
    LgFrDataInterfaceImplementationFromCmrpFiles difImp( baseFileName );
    registerCmrpFilesDifImpFunctions(&difImp);
    LgFrDataInterface dif(&difImp);

    // Create a scenario using the builder
    LgFrScenarioBuilderForCMRPwithPrioritiesFromDif builder;
    LgFrScenarioDirector director(&builder);
    director.construct(dif, calendar);

    scenarioPtr = builder.getScenario();
  }
  else {	// Use Perfect World Data
    calendar = LgFrCalendar(8, RWTime(RWDate("8/01/95")));
    LgFrDataInterfaceImplementation* difImpPtr
	= createPWcmrpDifImp();
    registerPWcmrpDifImpFunctions(difImpPtr);
    LgFrDataInterface dif(difImpPtr);
    delete difImpPtr;
    difImpPtr = NULL;

    // Create a scenario using the builder
    LgFrScenarioBuilderForCMRP builder;
    LgFrScenarioDirector director(&builder);
    director.construct(dif, calendar);

    scenarioPtr = builder.getScenario();
  }

  const LgFrSolutionOutputInterface &  soi
      = scenarioPtr->solutionOutputInterface();

#ifdef DUMP_SCHEDULES_TO_FILE

  // write a wit data file for debugging purposes
  //  scenarioPtr->lastProblemSolved().writeWitDataFile(
  //                                      "CMRP.wit.data",
  //                                       scenarioPtr
  //					     ->workingImplosionProblem());

  const LgFrScheduleFactory * sFactoryPtr = scenarioPtr->scheduleFactory();

#endif

  // create the multiSchedules
  LgFrMultiPartScheduleFloat * plannerMSPptr
      =  new LgFrMultiPartScheduleFloat(scenarioPtr);
  LgFrMultiPartScheduleFloat * mrpMSPptr
      = new LgFrMultiPartScheduleFloat(scenarioPtr);
  LgFrMultiPartScheduleFloat * capacityMSPptr
      = new LgFrMultiPartScheduleFloat(scenarioPtr);
  LgFrMultiDemandScheduleFloat * demandMSDptr
      = new LgFrMultiDemandScheduleFloat(scenarioPtr);
  LgFrMultiPartScheduleFloat * procurementSummaryMSPptr
      = new LgFrMultiPartScheduleFloat(scenarioPtr);

  LgFrMultiPartScheduleFloat & plannerMSP = *plannerMSPptr;
  LgFrMultiPartScheduleFloat & mrpMSP = *mrpMSPptr;
  LgFrMultiPartScheduleFloat & capacityMSP = *capacityMSPptr;
  LgFrMultiDemandScheduleFloat & demandMSD = *demandMSDptr;
  LgFrMultiPartScheduleFloat & procurementSummaryMSP
      = *procurementSummaryMSPptr;


  // -----------------------------------------------------------------
  cout << "Running the CMRP script:\n";
  cerr << "Running the CMRP script:\n";
  // -----------------------------------------------------------------

#ifdef DUMP_SCHEDULES_TO_FILE
  createAllTheDebugSchedules(sFactoryPtr);
#endif

#if 0
  witSetMesgTimesPrint( scenarioPtr->lps().witRun(),
     WitFALSE, WitINFORMATIONAL_MESSAGES, UCHAR_MAX );
  witSetMesgFileAccessMode( scenarioPtr->lps().witRun(), WitFALSE,  "w" );
  witWriteData( scenarioPtr->lps().witRun(), "cmrp.wit.data" );
  witWriteShipSched( scenarioPtr->lps().witRun(), WitSTDOUT, WitBSV );
  witDisplayData( scenarioPtr->lps().witRun(), "cmrp.display.data" );
  witSetMesgTimesPrint( scenarioPtr->lps().witRun(),
     WitFALSE, WitINFORMATIONAL_MESSAGES, 0 );
#endif

  // set-up the Planner-Managed MultiSchedule
  plannerMSP.newSchedule(LGFRPlannerManagedSupplyVol);
  plannerMSP.newSchedule(LGFRPlannerManagedRequiredVol);
  plannerMSP.newSchedule(LGFRPlannerManagedCumSupplyVol);
  plannerMSP.newSchedule(LGFRPlannerManagedCumRequiredVol);
  if ( ! useFileData )
    plannerMSP.newSchedule(LGFRPlannerManagedFocussedShortageSchedule);
  plannerMSP.newSchedule(LGFRPlannerManagedStockVol);
  plannerMSP.newSchedule(LGFRPlannerManagedScrapVol);
  plannerMSP.newSchedule(LGFRPlannerManagedUsageVol);
//  plannerMSP.newSchedule(LGFRPlannerManagedExcessVol);

  // -----------------------------------------------------------------
  cout << "Step -1:  Display DemandVolumes (Shipment Vol is zero until Implode happens)\n";
  cerr << "Step -1:  Display DemandVolumes (Shipment Vol is zero until Implode happens)\n";
  // -----------------------------------------------------------------

  if (interactive) {
    RWCString ts;
    cout << "Press enter to continue" << endl;
    readToEol();
  }


  demandMSD.newSchedule(LGFRCMRPDemandVol);
  demandMSD.newSchedule(LGFRCMRPShipmentVol);
  demandMSD.newSchedule(LGFRCMRPBacklogVol);

  cout << "Master Production Schedule\n";

  // Obtain abbreviations of demand multiSchedule window attributes
  LgFrVectorRWCString abbr = demandMSD.schedAbbrevVec();
  for (int i=0; i<abbr.length(); i++ )
     cout << "Abbreviation of attribute " << i << " is " << abbr[i] <<endl;

  // display the demands
  if ( ! useFileData )
    demandMSD.printByItemAndAttribute(printStyle);

  // -----------------------------------------------------------------
  cout << "Step 0:  Display Planner-Managed Parts multi-schedule\n";
  cerr << "Step 0:  Display Planner-Managed Parts multi-schedule\n";
  // -----------------------------------------------------------------

  if (interactive) {
    RWCString ts;
    cout << "Press enter to continue" << endl;
    readToEol();
  }

  // display the plannerMS
  if ( ! useFileData )
    plannerMSP.printByItemAndAttribute(printStyle);

  // NOTE: the Implosion and Explosion buttons should be lit


  // -----------------------------------------------------------------
  cout << "Step 1:  Explode and show req's vs avail for Planner-Managed Parts\n";
  cerr << "Step 1:  Explode and show req's vs avail for Planner-Managed Parts\n";
  // -----------------------------------------------------------------

  // User hits the explode button
  scenarioPtr->newExplosionSolution();
  cerr << "Step 1a" << endl;
  // Explode button should go out, Implosion button still lit
  // ???? Who controls this

  //  plannerMS will update automatically
  if ( ! useFileData )
    plannerMSP.printByItemAndAttribute(printStyle);
cerr << "Step 1b" << endl;
#ifdef DUMP_SCHEDULES_TO_FILE
  printThePlannerManagedDebugSchedules(
                      soi,
                      "Step1:  Explode and show req's vs avail for Planner-Managed Parts");
#endif

  // ------------------------------------------------------------------
  cerr << "Step 2:  Show Availability of Non-constrained parts (MRP-Managed)\n";
  cout << "Step 2:  Show Availability of Non-constrained parts (MRP-Managed)\n";
  // ------------------------------------------------------------------

  // set-up the MRP-Managed MultiSchedule
  mrpMSP.newSchedule(LGFRMRPManagedSupplyVol);
  mrpMSP.newSchedule(LGFRMRPManagedRequiredVol);
  mrpMSP.newSchedule(LGFRRequiredVolumeTimesUnitCost);
  mrpMSP.newSchedule(LGFRMRPManagedCumSupplyVol);
  mrpMSP.newSchedule(LGFRMRPManagedCumRequiredVol);
  if ( ! useFileData )
    mrpMSP.newSchedule(LGFRMRPManagedFocussedShortageSchedule);
  mrpMSP.newSchedule(LGFRMRPManagedStockVol);
  mrpMSP.newSchedule(LGFRMRPManagedScrapVol);
  mrpMSP.newSchedule(LGFRMRPManagedUsageVol);
//  mrpMSP.newSchedule(LGFRMRPManagedExcessVol);
  mrpMSP.newSchedule(LGFRFrozenZone);
  mrpMSP.newSchedule(LGFRProcurementVol);
  mrpMSP.newSchedule(LGFRProcurementVolumeTimesUnitCost);
//  mrpMSP.newSchedule(LGFRProcurementLeadTime);
  mrpMSP.newSchedule(LGFRProductionVolume);


//  mrpMSP.newSchedule(LGFRProductionVolumeTimesUnitCost);


  mrpMSP.newSchedule(LGFRCycleTime);
//  mrpMSP.newSchedule(LGFRTotalShipmentVol);

  // display it
  if ( ! useFileData )
    mrpMSP.printByItemAndAttribute(printStyle);

#ifdef DUMP_SCHEDULES_TO_FILE
  printTheMRPManagedDebugSchedules(
             soi,
             "Step2:  Show Availability of MRP-Managed parts");
#endif





  // ------------------------------------------------------------------
  cout << "Step 3:  IMPLODE and View Demands\n";
  cerr << "Step 3:  IMPLODE and View Demands\n";
  // ------------------------------------------------------------------

#if 0
  witSetMesgTimesPrint( scenarioPtr->lps().witRun(),
     WitFALSE, WitINFORMATIONAL_MESSAGES, UCHAR_MAX );
  witSetMesgFileAccessMode( scenarioPtr->lps().witRun(), WitFALSE,  "w" );
  witWriteData( scenarioPtr->lps().witRun(), "cmrp2.wit.data" );
  witWriteShipSched( scenarioPtr->lps().witRun(), WitSTDOUT, WitBSV );
  witDisplayData( scenarioPtr->lps().witRun(), "cmrp2.display.data" );
  witSetMesgTimesPrint( scenarioPtr->lps().witRun(),
     WitFALSE, WitINFORMATIONAL_MESSAGES, 0 );
#endif


  // user hits the Implode button
  scenarioPtr->newImplosionSolution();

  demandMSD.printByItemAndAttribute(printStyle);

#ifdef DUMP_SCHEDULES_TO_FILE
  printTheDemandDebugSchedules(
             soi,
             "Step3:  IMPLODE and View Demands\n");
#endif



  // ------------------------------------------------------------------
  cout << "Step 4:  View PlannerManaged MS (After implosion)\n";
  cerr << "Step 4:  View PlannerManaged MS (After implosion)\n";
  // ------------------------------------------------------------------

  plannerMSP.printByItemAndAttribute(printStyle);


  // ------------------------------------------------------------------
  cout << "Step 4.1:  Sort Planner Managed MS by criticallity\n";
  cerr << "Step 4.1:  Sort Planner Managed MS by criticallity\n";
  // ------------------------------------------------------------------

  plannerMSP.itemOrderSchedule(
         LGFRPlannerManagedSupplyVol,LGFRSortByCriticality );
  if ( ! useFileData )
    plannerMSP.printByItemAndAttribute(printStyle);


#ifdef DUMP_SCHEDULES_TO_FILE
  printThePlannerManagedDebugSchedules(soi,
                                   "Step4:  View PlannerManaged MS (After implosion)");
#endif


  // ------------------------------------------------------------------
  cout << "Step 5:  View MRP Managed MS (After Implosion)\n";
  cerr << "Step 5:  View MRP Managed MS (After Implosion)\n";
  // ------------------------------------------------------------------

  if ( ! useFileData )
    mrpMSP.printByItemAndAttribute(printStyle);

  // ------------------------------------------------------------------
  cout << "Step 5.1:  Sort MRP Managed MS by criticallity\n";
  cerr << "Step 5.1:  Sort MRP Managed MS by criticallity\n";
  // ------------------------------------------------------------------

  mrpMSP.itemOrderSchedule(
			   LGFRMRPManagedSupplyVol,LGFRSortByCriticality );
  if ( ( ! useFileData ) || verbosePrinting ) {
    // ------------------------------------------------------------------
    cout << "Step 5.2:  Print MRP Managed MS\n";
    cerr << "Step 5.2:  Print MRP Managed MS\n";
    // ------------------------------------------------------------------
    mrpMSP.printByItemAndAttribute(printStyle);
  }

#if 0
  // FOR TESTING, causes wit to write the critical list.
  witSetMesgTimesPrint( scenarioPtr->lps().witRun(),
     WitFALSE, WitINFORMATIONAL_MESSAGES, UCHAR_MAX );
  witSetMesgFileAccessMode( scenarioPtr->lps().witRun(), WitFALSE,  "w" );
  witWriteCriticalList( scenarioPtr->lps().witRun(), WitSTDOUT, WitBSV,99999);
  witSetMesgTimesPrint( scenarioPtr->lps().witRun(),
     WitFALSE, WitINFORMATIONAL_MESSAGES, 0 );
#endif

  // ------------------------------------------------------------------
  cout << "Step 5.5:  View Procurement Summary Window\n";
  cerr << "Step 5.5:  View Procurement Summary Window\n";
  // ------------------------------------------------------------------

  procurementSummaryMSP.newSchedule(LGFRCMRPTotalMrpProcurementCost);
  procurementSummaryMSP.newSchedule(LGFRCMRPTotalWitProcurementCost);
  procurementSummaryMSP.newSchedule(LGFRCMRPCumTotalMrpProcurementCost);
  procurementSummaryMSP.newSchedule(LGFRCMRPCumTotalWitProcurementCost);
  procurementSummaryMSP.newSchedule(LGFRCMRPDeltaCumTotalProcurementCost);
  procurementSummaryMSP.newSchedule(LGFRCostOfCapitalSavings);

  procurementSummaryMSP.newSchedule(LGFRCMRPSummaryOfCumDemandVol);
  procurementSummaryMSP.newSchedule(LGFRCMRPSummaryOfCumDemandVolTimesUnitCost);
  procurementSummaryMSP.newSchedule(LGFRCMRPSummaryOfCumShipmentVol);
  procurementSummaryMSP.newSchedule(
                                   LGFRCMRPSummaryOfCumShipmentVolTimesUnitCost
                                   );
  if (procurementSumToFile) {
    ofstream ofs (*(argv+3), ios::out);
    procurementSummaryMSP.printByItemAndAttribute(printStyle,&ofs);
  }
  else
    procurementSummaryMSP.printByItemAndAttribute(printStyle);




#ifdef DUMP_SCHEDULES_TO_FILE
  printThePlannerManagedDebugSchedules(soi, "Step5: View MRP Managed MS (After Implosion)");
#endif

  // ------------------------------------------------------------------
  cout << "Step 6:  View CAPACITY MS (After Implosion)\n";
  cerr << "Step 6:  View CAPACITY MS (After Implosion)\n";
  // ------------------------------------------------------------------
  // set up the multi sched
  capacityMSP.newSchedule(LGFRCapacitySupplyVol);
  capacityMSP.newSchedule(LGFRCapacityRequiredVol);
  capacityMSP.newSchedule(LGFRCapacityScrapVol);
  capacityMSP.newSchedule(LGFRCapacityUsageVol);
  capacityMSP.newSchedule(LGFRCapacityExcessVol);
  capacityMSP.newSchedule(LGFRCapacityExcessVol);
  capacityMSP.newSchedule(LGFRCapacityUsagePct);

  // display it
  capacityMSP.printByItemAndAttribute(printStyle);

#ifdef DUMP_SCHEDULES_TO_FILE
  printTheCapacityDebugSchedules(soi, "Step6: View Capacity MS (After Implosion)");
#endif


  // ------------------------------------------------------------------
  cout << "Step 7:  Edit supply of a constrained part and Re-implode\n";
  cerr << "Step 7:  Edit supply of a constrained part and Re-implode\n";
  // ------------------------------------------------------------------


  {
     // first make sure that the supply attribute is muteable
     RWBoolean result = plannerMSP.isMutable(LGFRPlannerManagedSupplyVol);
     assert (result == TRUE);

      // define the part of interest -- Screen_S

     const LgFrPart * part1Ptr = scenarioPtr->constSetOfParts().findValue("Screen_S");
     if (part1Ptr != NULL) {
        cout << "\nIncreasing supply on part Screen_S, by 100.0\n";
        LgFrPart     part1(*part1Ptr);

        // get the original  timevec
        const LgFrTimeVecFloat  * originalTVptr =
                      plannerMSP.getTimeVecFloat(part1, LGFRPlannerManagedSupplyVol);

        // add 100.0
        LgFrTimeVecFloat newTV1 = *originalTVptr + 100.0;

        // update the supply and check it
        result = plannerMSP.changeTimeVecInSchedule(
                         part1, LGFRPlannerManagedSupplyVol, newTV1);
        assert(result == TRUE);

      }

      // define the part of interest -- MEM_Module

     const LgFrPart * part2Ptr = scenarioPtr->constSetOfParts().findValue("MEM_Module");
     if (part2Ptr != NULL) {
        cout << "\nChanging supply on part MEM_Module, to  9500.0 (all periods)\n";
        LgFrPart     part2(*part2Ptr);

        // now change the second part's supply
        LgFrTimeVecFloat newTV2(calendar.nPeriods(), 9500.0);

        // update the supply and check it
        result = plannerMSP.changeTimeVecInSchedule(
                         part2, LGFRPlannerManagedSupplyVol, newTV2);
        assert(result == TRUE);
      }

     // re-implode
     scenarioPtr->newImplosionSolution();

     if ( ! useFileData )
       plannerMSP.printByItemAndAttribute(printStyle);
#ifdef DUMP_SCHEDULES_TO_FILE
     printThePlannerManagedDebugSchedules(soi,
                        "Step7:  Update some supplies and re-implode");
#endif
     }


  // ------------------------------------------------------------------
  cout << "Step 8:  Filter MRP Managed MS to RAW parts only (SOP filter)\n";
  cerr << "Step 8:  Filter MRP Managed MS to RAW parts only (SOP filter)\n";
  // ------------------------------------------------------------------
  {
     // create a SOP filter
     LgFrSetOfPartsFilterStrategy rawFS;

     // set it to filter based on LFGR_RAW_SET ofParts
     rawFS.setOfPartsIdentifier(*scenarioPtr, LGFR_RAW_SET);

     // set the supply attribute to be the "selection schedule"  and give
     // it the filterStrategy
     RWBoolean result = mrpMSP.itemSelectionSchedule(LGFRMRPManagedCumRequiredVol, &rawFS);
     assert (result == TRUE);

     if ( ! useFileData )
       mrpMSP.printByItemAndAttribute(printStyle);
  }
  // ------------------------------------------------------------------
  cout << "Step 9:  Sort  MRP-Managed MS based on CumRequired Volume\n";
  cerr << "Step 9:  Sort  MRP-Managed MS based on CumRequired Volume\n";
  // ------------------------------------------------------------------
  {

     // NOTE: it appears that the "selection schedule" must be the same
     // as the "order schedule"

     // create a  sort strategy based on the data in period 7
     LgFrTimeVecElementPairCompareFloat tvePC;
     tvePC.period(7);

     // set the cumrequired attribute/schedule to be the "order schedule"  and give
     // it the sortStrategy
     RWBoolean result = mrpMSP.itemOrderSchedule(LGFRMRPManagedCumRequiredVol, &tvePC);
     assert (result == TRUE);

     // swap attribute priorites (supply and cumReq).  This is not necessary.
     // It switches the order of the two in the MS.
     mrpMSP.prioritySwap(LGFRMRPManagedCumRequiredVol, LGFRMRPManagedSupplyVol);

     if ( ! useFileData )
       mrpMSP.printByItemAndAttribute(printStyle);

  }

  // ------------------------------------------------------------------
  cout << "Step 10:  Set a demand focus and compute FSS.  View in MRP-Managed MS\n";
  cerr << "Step 10:  Set a demand focus and compute FSS.  View in MRP-Managed MS\n";
  // ------------------------------------------------------------------
  LgFrMultiPartScheduleFloat * fssMSPptr = new LgFrMultiPartScheduleFloat(scenarioPtr);
  LgFrMultiPartScheduleFloat & fssMSP = *fssMSPptr;

  if ( useFileData ) {
    plannerMSP.newSchedule(LGFRPlannerManagedFocussedShortageSchedule);
    mrpMSP.newSchedule(LGFRMRPManagedFocussedShortageSchedule);
  }

  fssMSP.newSchedule(LGFRSupplyVolume);
  fssMSP.newSchedule(LGFRFocussedShortageSchedule);

  capacityMSP.newSchedule(LGFRCapacityFocussedShortageSchedule);

  {
      LgFrSortingDemandScheduleFloatSmartPointer ps
	  = scenarioPtr->scheduleFactory()->newDemandSchedule(LGFRPriority);
      LgFrSetOfDemands hotDemands
	  = scenarioPtr->mostUrgentDemands (*ps);
      for (int i = 0; i < hotDemands.entries(); ++i)
	  setFocus (hotDemands[i].name(),
		    hotDemands[i].partPtr()->name(),
		    calendar.nPeriods()-1, scenarioPtr);

      RWCString pName ="Computer_1";
      RWCString dName = "FirmForecast";
      setFocus (dName, pName, calendar.nPeriods()-1, scenarioPtr);
  }
  // implode again
  scenarioPtr->newImplosionSolution();

  if ( ! useFileData )
    mrpMSP.printByItemAndAttribute(printStyle);

  // ------------------------------------------------------------------
  cout << "Step 10a:  Viewing FSS for all materials and capacities \n";
  cerr << "Step 10a:  Viewing FSS for all materials and capacities \n";
  // ------------------------------------------------------------------
  if ( ! useFileData )
    fssMSP.printByItemAndAttribute(printStyle);

  // ------------------------------------------------------------------
  cout << "Step 10b:  Viewing FSS for capacities \n";
  cerr << "Step 10b:  Viewing FSS for capacities \n";
  // ------------------------------------------------------------------
  if ( ! useFileData )
    capacityMSP.printByItemAndAttribute(printStyle);


  // ------------------------------------------------------------------
  cout << "Step 11:  Filter (exclude!) on Non-zero fss  *OR*   PRODUCT parts only\n";
  cerr << "Step 11:  Filter (exclude!) on Non-zero fss  *OR*   PRODUCT parts only\n";
  // ------------------------------------------------------------------
  {
     // create a SOP filter
     LgFrSetOfPartsFilterStrategy prodSopFS;

     // set it to filter based on LFGR_PRODUCEABLE_SET ofParts
     prodSopFS.setOfPartsIdentifier(*scenarioPtr, LGFR_PRODUCEABLE_SET);

     // create a zeroTimeVec filter
     LgFrZeroTimeVecFilterStrategy  ztvFS;

     // create a boolean OR filter
     LgFrOrFilterStrategy  orFS;

     // assign the lhs and rhs to orFS
     orFS.leftHandSideFilterStrategy(ztvFS);
     orFS.rightHandSideFilterStrategy(prodSopFS);

     // set the fss attribute to be the "selection schedule"  and give
     // it the filterStrategy
     RWBoolean result
	 = mrpMSP.itemSelectionSchedule(LGFRMRPManagedFocussedShortageSchedule,
					&orFS);
     assert (result == TRUE);

     if ( ! useFileData )
       mrpMSP.printByItemAndAttribute(printStyle);

  }
  // ------------------------------------------------------------------
  cout << "Step 11a:  Filter (exclude!) on Non-zero fss in the capacity MS\n";
  cerr << "Step 11a:  Filter (exclude!) on Non-zero fss in the capacity MS\n";
  // ------------------------------------------------------------------

  // create a zeroTimeVec filter
  LgFrZeroTimeVecFilterStrategy  ztvFS;

  // set the fss attribute to be the "selection schedule"  and give
  // it the filterStrategy
  RWBoolean result
      = capacityMSP.itemSelectionSchedule(LGFRCapacityFocussedShortageSchedule,
					  &ztvFS);
  assert (result == TRUE);

  if ( ! useFileData )
    capacityMSP.printByItemAndAttribute(printStyle);

  // ------------------------------------------------------------------
  cout << "Script Completed -- Cleaning Up\n";
  cerr << "Script Completed -- Cleaning Up\n";
  // ------------------------------------------------------------------

#ifdef DUMP_SCHEDULES_TO_FILE
  deleteAllTheDebugSchedules();
#endif

  delete plannerMSPptr;
  delete mrpMSPptr;
  delete capacityMSPptr;
  delete demandMSDptr;
  delete procurementSummaryMSPptr;
  delete fssMSPptr;

  cerr << "All tests completed successfully\n";
  cout << "All tests completed successfully\n";
  return 0;
}

// ====================================================================
// ====================================================================
// ====================================================================
#ifdef DUMP_SCHEDULES_TO_FILE

void createAllTheDebugSchedules(const LgFrScheduleFactory * sFactoryPtr)
{
  // Use ScheduleFactory to create all the schedules

  PlannerManagedSupplyVol = sFactoryPtr->newPartSchedule(LGFRPlannerManagedSupplyVol);
  PlannerManagedRequiredVol = sFactoryPtr->newPartSchedule(LGFRPlannerManagedRequiredVol);
  PlannerManagedCumSupplyVol = sFactoryPtr->newPartSchedule(LGFRPlannerManagedCumSupplyVol);
  PlannerManagedCumRequiredVol = sFactoryPtr->newPartSchedule(LGFRPlannerManagedCumRequiredVol);
  PlannerManagedStockVol = sFactoryPtr->newPartSchedule(LGFRPlannerManagedStockVol);
  PlannerManagedScrapVol = sFactoryPtr->newPartSchedule(LGFRPlannerManagedScrapVol);
  PlannerManagedUsageVol = sFactoryPtr->newPartSchedule(LGFRPlannerManagedUsageVol);
  PlannerManagedExcessVol = sFactoryPtr->newPartSchedule(LGFRPlannerManagedExcessVol);

  MRPManagedSupplyVol = sFactoryPtr->newPartSchedule(LGFRMRPManagedSupplyVol);
  MRPManagedRequiredVol = sFactoryPtr->newPartSchedule(LGFRMRPManagedRequiredVol);
  MRPManagedCumSupplyVol = sFactoryPtr->newPartSchedule(LGFRMRPManagedCumSupplyVol);
  MRPManagedCumRequiredVol = sFactoryPtr->newPartSchedule(LGFRMRPManagedCumRequiredVol);
  MRPManagedStockVol = sFactoryPtr->newPartSchedule(LGFRMRPManagedStockVol);
  MRPManagedScrapVol = sFactoryPtr->newPartSchedule(LGFRMRPManagedScrapVol);
  MRPManagedUsageVol = sFactoryPtr->newPartSchedule(LGFRMRPManagedUsageVol);
  MRPManagedExcessVol = sFactoryPtr->newPartSchedule(LGFRMRPManagedExcessVol);
  ProductionVolume = sFactoryPtr->newPartSchedule(LGFRProductionVolume);
  ProcurementVol = sFactoryPtr->newPartSchedule(LGFRProcurementVol);
  CycleTime = sFactoryPtr->newPartSchedule(LGFRCycleTime);
  ProcurementLeadTime = sFactoryPtr->newPartSchedule(LGFRProcurementLeadTime);
  TotalShipmentVol = sFactoryPtr->newPartSchedule(LGFRTotalShipmentVol);

  DemandVolume = sFactoryPtr->newDemandSchedule(LGFRDemandVolume);
  ShipmentVolume = sFactoryPtr->newDemandSchedule(LGFRShipmentVolume);
  BacklogVolume = sFactoryPtr->newDemandSchedule(LGFRBacklogVolume);

  CapacitySupplyVol   = sFactoryPtr->newPartSchedule(LGFRCapacitySupplyVol);
  CapacityRequiredVol = sFactoryPtr->newPartSchedule(LGFRCapacityRequiredVol);
  CapacityUsageVol    = sFactoryPtr->newPartSchedule(LGFRCapacityUsageVol);
  CapacityExcessVol   = sFactoryPtr->newPartSchedule(LGFRCapacityExcessVol);

}

void deleteAllTheDebugSchedules()
{
  delete PlannerManagedSupplyVol;
  delete PlannerManagedRequiredVol;
  delete PlannerManagedCumSupplyVol;
  delete PlannerManagedCumRequiredVol;
  delete PlannerManagedUsageVol;
  delete PlannerManagedStockVol;
  delete PlannerManagedScrapVol;
  delete PlannerManagedExcessVol;

  delete MRPManagedSupplyVol;
  delete MRPManagedRequiredVol;
  delete MRPManagedCumSupplyVol;
  delete MRPManagedCumRequiredVol;
  delete MRPManagedUsageVol;
  delete MRPManagedStockVol;
  delete MRPManagedScrapVol;
  delete MRPManagedExcessVol;
  delete ProcurementVol;
  delete ProductionVolume;
  delete ProcurementLeadTime;
  delete CycleTime;
  delete TotalShipmentVol;


  delete DemandVolume;
  delete ShipmentVolume;
  delete BacklogVolume;

  delete CapacitySupplyVol;
  delete CapacityRequiredVol;
  delete CapacityUsageVol;
  delete CapacityExcessVol;

}

void printThePlannerManagedDebugSchedules(const LgFrSolutionOutputInterface & soi, RWCString aTitle)
{
  RWCString firstSchedTitle("\n\n");
  firstSchedTitle.append(aTitle);
  firstSchedTitle.append("\nPlannerManagedSupplyVol");


  cmrpWriteSchedule(soi,PlannerManagedSupplyVol,
                    firstSchedTitle);
  cmrpWriteSchedule(soi,PlannerManagedRequiredVol,
                    "\nPlannerManagedRequiredVol");
  cmrpWriteSchedule(soi,PlannerManagedCumSupplyVol,
                    "\nPlannerManagedCumSupplyVol");
  cmrpWriteSchedule(soi,PlannerManagedCumRequiredVol,
                    "\nPlannerManagedCumRequiredVol");
  cmrpWriteSchedule(soi,PlannerManagedUsageVol,
                    "\nPlannerManagedUsageVol");
  PlannerManagedExcessVol->populate();
  cmrpWriteSchedule(soi,PlannerManagedExcessVol,
                    "\nPlannerManagedExcessVol");
  cmrpWriteSchedule(soi,PlannerManagedStockVol,
                    "\nPlannerManagedStockVol");
  cmrpWriteSchedule(soi,PlannerManagedScrapVol,
                    "\nPlannerManagedScrapVol");
}

void printTheMRPManagedDebugSchedules(const LgFrSolutionOutputInterface & soi, RWCString aTitle)
{
  RWCString firstSchedTitle("\n\n");
  firstSchedTitle.append(aTitle);
  firstSchedTitle.append("\nMRPManagedSupplyVol");

  cmrpWriteSchedule(soi,MRPManagedSupplyVol,
                    firstSchedTitle);
  cmrpWriteSchedule(soi,MRPManagedRequiredVol,
                    "\nMRPManagedRequiredVol");
  cmrpWriteSchedule(soi,MRPManagedCumSupplyVol,
                    "\nMRPManagedCumSupplyVol");
  cmrpWriteSchedule(soi,MRPManagedCumRequiredVol,
                    "\nMRPManagedCumRequiredVol");
  cmrpWriteSchedule(soi,MRPManagedUsageVol,
                    "\nMRPManagedUsageVol");
  MRPManagedExcessVol->populate();
  cmrpWriteSchedule(soi,MRPManagedExcessVol,
                    "\nMRPManagedExcessVol");
  cmrpWriteSchedule(soi,MRPManagedStockVol,
                    "\nMRPManagedStockVol");
  cmrpWriteSchedule(soi,MRPManagedScrapVol,
                    "\nMRPManagedScrapVol");
  cmrpWriteSchedule(soi,ProcurementVol,
                    "\nProcurementVol");
  cmrpWriteSchedule(soi,ProcurementLeadTime,
                    "\nProcurementLeadTime");
  cmrpWriteSchedule(soi,ProductionVolume,
                    "\nProductionVolume");
  cmrpWriteSchedule(soi,CycleTime,
                    "\nCycleTime");
  cmrpWriteSchedule(soi,TotalShipmentVol,
                    "\nTotalShipmentVol");
}

void printTheDemandDebugSchedules(const LgFrSolutionOutputInterface & soi, RWCString aTitle)
{
  RWCString firstSchedTitle("\n\n");
  firstSchedTitle.append(aTitle);
  firstSchedTitle.append("\nDemandVolume");

  cmrpWriteSchedule(soi,DemandVolume,
                    firstSchedTitle);
  cmrpWriteSchedule(soi,ShipmentVolume,
                    "\nShipmentVolume");
  cmrpWriteSchedule(soi,BacklogVolume,
                    "\nBacklogVolume");
}

void printTheCapacityDebugSchedules(const LgFrSolutionOutputInterface & soi, RWCString aTitle)
{
  RWCString firstSchedTitle("\n\n");
  firstSchedTitle.append(aTitle);
  firstSchedTitle.append("\nCapacitySupplyVol");

  cmrpWriteSchedule(soi,CapacitySupplyVol,
                    firstSchedTitle);
  cmrpWriteSchedule(soi,CapacityRequiredVol,
                    "\nCapacityRequiredVol");
  cmrpWriteSchedule(soi,CapacityUsageVol,
                    "\nCapacityUsageVol");
  cmrpWriteSchedule(soi,CapacityExcessVol,
                    "\nCapacityExcessVol");
}


#endif
