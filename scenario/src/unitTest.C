// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL -- inserted includes.
// Test individual classes or groups of classes

#include <assert.h>


#include <scenario/src/machdep.h>
//#include <scenario/src/util.h>
#include <scenario/src/collflt.h>
//#include <scenario/src/sortVecE.h>
#include <scenario/src/calendar.h>
//#include <scenario/src/difWitD.h>
#include <scenario/src/difWitR.h>
#include <scenario/src/difCmrpF.h>
#include <scenario/src/timeVecs.h>
//#include <scenario/src/timVComp.h>
#include <scenario/src/sortSchd.h>
#include <scenario/src/partSchd.h>
#include <scenario/src/demdSchd.h>
#include <scenario/src/pairComp.h>
//#include <scenario/src/tvElemPC.h>
#include <scenario/src/scBdDflt.h>
//#include <scenario/src/scBdDifT.h>
#include <scenario/src/scBdTest.h>
//#include <scenario/src/pcatPC.h>
//#include <scenario/src/schdSnPC.h>
//#include <scenario/src/sopSnPC.h>
#include <scenario/src/demdPnPC.h>
//#include <scenario/src/demdPrPC.h>
//#include <scenario/src/demdVgPC.h>
//#include <scenario/src/demdBgPC.h>
#include <scenario/src/dif.h>
#include <scenario/src/difSFile.h>
#include <scenario/src/difPWcmr.h>
//#include <scenario/src/difPWatp.h>
//#include <scenario/src/difVWcmr.h>
#include <scenario/src/difTest.h>
#include <scenario/src/supplySS.h>
#include <scenario/src/shipSS.h>
//#include <scenario/src/shpBndSS.h>
#include <scenario/src/excessSS.h>
#include <scenario/src/demVolSS.h>
#include <scenario/src/prodSS.h>
#include <scenario/src/reqSS.h>
#include <scenario/src/cycleSS.h>
#include <scenario/src/scrapSS.h>
#include <scenario/src/stockSS.h>
#include <scenario/src/critPaSS.h>
#include <scenario/src/fssSS.h>
#include <scenario/src/fssPrdSS.h>
#include <scenario/src/schdSnSS.h>
#include <scenario/src/cumSS.h>
#include <scenario/src/consSS.h>
#include <scenario/src/costSS.h>
#include <scenario/src/deltaSS.h>
#include <scenario/src/offsetSS.h>
#include <scenario/src/usageSS.h>
#include <scenario/src/priorSS.h>
#include <scenario/src/shpRewSS.h>

#include <scenario/src/filtStrt.h>
#include <scenario/src/zeroTvFS.h>
//#include <scenario/src/sOfPaFS.h>
#include <scenario/src/schdSnFS.h>
#include <scenario/src/boolFS.h>
#include <scenario/src/model.h>

#include <scenario/src/demdPerd.h>
#include <scenario/src/lastPrbS.h>

#include <scenario/src/witRunVn.h>
#include <scenario/src/inPrFile.h>
#include <scenario/src/sOfDemds.h>
#include <scenario/src/sOfParts.h>

#include <scenario/src/wrkgProb.h>
#include <scenario/src/scenario.h>

#include <scenario/src/testScen.h>
#include <scenario/src/outInter.h>
#include <scenario/src/yktscen.h>
#include <scenario/src/fssCtrl.h>
#include <scenario/src/implode.h>
#include <scenario/src/explode.h>

#include <scenario/src/usrInMod.h>


#include <scenario/src/demdNmFS.h>
//#include <scenario/src/partNmFS.h>
#include <scenario/src/partCtFS.h>

#include <scenario/src/scenCMRP.h>
#include <scenario/src/ipCMRP.h>
#include <scenario/src/scenBld.h>
#include <scenario/src/scBdCMRP.h>
#include <scenario/src/scenDrct.h>
#include <scenario/src/schFctry.h>
#include <scenario/src/schFCMRP.h>
//#include <scenario/src/schFcATP.h>
#include <scenario/src/schdDeSt.h>
//#include <scenario/src/descSchd.h>

//#include <scenario/src/multiSch.h>
//#include <scenario/src/multPSch.h>
//#include <scenario/src/multDSch.h>
//#include <scenario/src/scBdATP.h>

//#include <scenario/src/scenFac.h>


// LGFR_CAPACITY_SET is defined in scenCMRP.h and scenATP.h
#undef LGFR_CAPACITY_SET

//#include <scenario/src/scenATP.h>
#include <scenario/src/demSSATP.h>
//#include <scenario/src/shpSSATP.h>
//#include <scenario/src/bacSSATP.h>
#include <scenario/src/supSSATP.h>
//#include <scenario/src/fssSSATP.h>


// smart pointer stuff
#include <scenario/src/scCMRPP.h>
#include <scenario/src/scATPP.h>
#include <scenario/src/sortSchP.h>
#include <scenario/src/partSchP.h>
#include <scenario/src/demdSchP.h>

// Function Prototypes. Function definitions is in this file.
void testingMessage( const char * const msg );
void pairCompareStrategiesTest();
void attributeScheduleStrategiesTest();
void filterStrategiesTest();
void dataInterfaceTest();

// The following macro is provided instead of a function in order to
// avoid class conversions.
// Release smart pointers to schedules in the schedule factory so
// that the scenario can be deleted when the current block exists.
#define CLEAR_AND_DESTROY_SF_SCHEDULE_PTRS(ptr)	\
  ( (LgFrScheduleFactory *)    /* Cast away const */	\
    (ptr->scheduleFactory()) )			\
    -> clearAndDestroy()

int
main ()
{

  testingMessage( "Testing LgFrMachdepTest\n" );
  LgFrMachdepTest();

  testingMessage( "Testing LgFrUtilTest\n" );
  //LgFrUtilTest();

  testingMessage( "Testing LgFrCollectableFloat\n" );
  LgFrCollectableFloat::test();

  testingMessage( "Testing LgFrSortedVectorEnhanced\n" );
  //LgFrSortedVectorEnhanced::test();

  testingMessage( "Testing LgFrCalendar\n" );
  LgFrCalendar::test();

  testingMessage( "Testing LgFrPart\n" );
  LgFrPart::test();

  testingMessage( "Testing LgFrDemand\n" );
  LgFrDemand::test();	

  testingMessage( "Testing LgFrTimeVecFloat\n" );
  LgFrTimeVecFloat::test();

  testingMessage( "Testing LgFrTimeVecCompareFloat\n" );
//  LgFrTimeVecCompareFloat::test();

  testingMessage( "Testing LgFrItemTimeVecPairFloat\n" );
  LgFrItemTimeVecPairFloat::test();

  testingMessage( "Testing LgFrSchedule\n" );
  LgFrScheduleFloat::test();

  testingMessage( "Testing LgFrDemandPeriodPair\n" );
  LgFrDemandPeriodPair::test();

  testingMessage( "Testing LgFrSetOfPartsWithMovableParts\n" );
  LgFrSetOfPartsWithMovableParts::test();

  testingMessage( "Testing LgFrSetOfPartsWithUnmovableParts\n" );
  LgFrSetOfPartsWithUnmovableParts::test();

  testingMessage( "Testing LgFrSetOfDemands\n" );
  LgFrSetOfDemands::test();

  testingMessage( "Testing LgFrWitRunVeneer\n" );
  LgFrWitRunVeneer::test();

  testingMessage( "Testing LgFrInitialProblemFromFile\n" );
  LgFrInitialProblemFromFile::test();

  testingMessage( "Testing LgFrScenarioBuilderForDefault\n" );
  LgFrScenarioBuilderForDefault::test();
  
  testingMessage( "Testing LgFrScenarioBuilderForTest\n" );
  LgFrScenarioBuilderForTest::test();

  testingMessage( "Testing LgFrScenarioBuilderForCMRP\n" );
  LgFrScenarioBuilderForCMRP::test();

#ifndef _MSC_VER
  //Crashing on MS VC++ Ver 5
  testingMessage( "Testing LgFrScenarioBuilderForCMRPwithPrioritiesFromDif\n" );
  LgFrScenarioBuilderForCMRPwithPrioritiesFromDif::test();
#endif

  testingMessage( "Testing LgFrScenarioDirector\n" );
  LgFrScenarioDirector::test();

  testingMessage( "Testing LgFrLastProblemSolved\n" );
  LgFrLastProblemSolved::test();
  
  testingMessage( "Testing LgFrWorkingImplosionProblem\n" );
  LgFrWorkingImplosionProblem::test();

  // Repeat the LgFrWitRunVeneer test in case the bug that occurs after
  // calling LgFrLastProblemSolved::test() comes back.
  testingMessage( "Testing LgFrWitRunVeneer \n" );
  LgFrWitRunVeneer::test();

  // This also calls LgFrSolutionOutputInterface::contextTest()
  testingMessage( "Testing LgFrScenario\n" );
  LgFrScenario::test();

  testingMessage( "Testing LgFrScenarioForCMRP\n" );
  LgFrScenarioForCMRP::test();
  
  testingMessage( "Testing LgFrScenarioForATP\n" );
//  LgFrScenarioForATP::test();  
  
  testingMessage( "Testing LgFrTestScenario\n" );
  LgFrTestScenario::test();

  testingMessage( "Testing LgFrSortingSchedule\n" );
  LgFrSortingScheduleFloat::test();

  testingMessage( "Testing LgFrSortingPartSchedule\n" );
  LgFrSortingPartScheduleFloat::test();

  testingMessage( "Testing LgFrSortingDemandSchedule\n" );
  LgFrSortingDemandScheduleFloat::test();

  testingMessage( "Testing LgFrFocussedShortageScheduleControl\n" );
  LgFrFocussedShortageScheduleControl::test();

  testingMessage( "Testing LgFrImplode\n" );
  LgFrImplode::test();

  testingMessage( "Testing LgFrExplode\n" );
  LgFrExplode::test();
  
  testingMessage( "Testing LgFrUserInputModification\n" );
  LgFrUserInputModification::test();

  testingMessage( "Testing LgFrSubscription\n" );
  LgFrSubscription::test();

  testingMessage( "Testing LgFrSolutionOutputInterface\n" );
  LgFrSolutionOutputInterface::test();

  // LgFrYktScenario is not being used right now --- WCD, 9/6/94
  // testingMessage( "Testing LgFrYktScenario\n" );
  // LgFrYktScenario::test();

  dataInterfaceTest();

  filterStrategiesTest();

  attributeScheduleStrategiesTest();

  pairCompareStrategiesTest();

  {
    // Make objects for scenario construction for more complete tests
    LgFrDataInterfaceSpecialPurposeFromFile
      diner12Prob ("../../../../wit/wit-data/examples/diner12/wit.data");
    LgFrCalendar calInst;
    LgFrScenarioBuilderForDefault builder;
    LgFrScenarioDirector director(&builder);

    testingMessage( "Testing LgFrScheduleFactory\n" );
    {
      director.construct(diner12Prob, calInst);
      LgFrScenarioSmartPointer  dInst1P = builder.getScenario();
      LgFrScheduleFactory::contextTest(*(dInst1P->scheduleFactory()),
				       dInst1P);
    }

    testingMessage( "Testing LgFrScheduleFactoryForCMRP\n" );
    {
	LgFrDataInterfaceImplementation * difPWcmrpImpPtr
	    = new LgFrDataInterfaceImplementation("internal data set pw1");
        registerPWcmrpDifImpFunctions(difPWcmrpImpPtr);

	LgFrDataInterface dif(difPWcmrpImpPtr);
	delete difPWcmrpImpPtr;
	difPWcmrpImpPtr = 0;

	LgFrCalendar calInst;
	LgFrScenarioBuilderForCMRP builder;
	LgFrScenarioDirector director(&builder);

	// diner12Prob can not work with LgFrScenarioBuilderForCMRP, JMT 8/14/95
	// director.construct(diner12Prob, calInst);
	director.construct(dif, calInst);
	LgFrScenarioForCMRPSmartPointer dInst1P = builder.getScenario();
	LgFrScheduleFactoryForCMRP::contextTest(*(dInst1P->scheduleFactory()));

	CLEAR_AND_DESTROY_SF_SCHEDULE_PTRS(dInst1P);
    }

/*    testingMessage( "Testing LgFrScheduleFactoryForATP\n" );
    {
	LgFrScheduleFactoryForATP::test();

	LgFrDataInterfaceImplementation * difPWatpImpPtr
	    = new LgFrDataInterfaceImplementation("internal data set pw1");
        registerPWatpDifImpFunctions(difPWatpImpPtr);

	LgFrDataInterface dif(difPWatpImpPtr);
	delete difPWatpImpPtr;
	difPWatpImpPtr = 0;

	LgFrCalendar calInst;
	LgFrScenarioBuilderForATP builder;
	LgFrScenarioDirector director(&builder);

	// diner12Prob can not work with LgFrScenarioBuilderForATP, use difPWatp instead JMT 9/15/95
	// director.construct(diner12Prob, calInst);
	director.construct(dif, calInst);
	LgFrScenarioForATPSmartPointer dInst1P = builder.getScenario();
	LgFrScheduleFactoryForATP::contextTest(*(dInst1P->scheduleFactory()));

	CLEAR_AND_DESTROY_SF_SCHEDULE_PTRS(dInst1P);
    }*/


    /*testingMessage( "Testing LgFrDescriptorSchedulePairFloat\n" );
    {
      director.construct(diner12Prob, calInst);
      LgFrScenarioSmartPointer  dInst1P = builder.getScenario();
      LgFrScenarioSmartPointer dInst1Pdup  =  dInst1P.deepCopy();

      const LgFrScheduleDescriptor supplyD (LGFRSupplyVolume);
      const LgFrScheduleDescriptor productD (LGFRProductionVolume);

      LgFrSortingPartScheduleFloatSmartPointer supplySchedule
        = dInst1P->scheduleFactory() -> newPartSchedule (supplyD);
      LgFrSortingPartScheduleFloatSmartPointer productionSchedule
        = dInst1P->scheduleFactory() -> newPartSchedule (productD);

      
      LgFrDescriptorSchedulePairFloat::contextTest(*dInst1P,
                                                   supplyD, supplySchedule,
                                                   productD, productionSchedule,
                                                   *dInst1Pdup);
      CLEAR_AND_DESTROY_SF_SCHEDULE_PTRS(dInst1P);
    }*/



    // Testing multi-schedules

    /*testingMessage( "Testing LgFrMultiPartScheduleFloat\n" );
    {
      director.construct(diner12Prob, calInst);
      LgFrScenarioSmartPointer dInst1P = builder.getScenario();
      LgFrScenarioSmartPointer dInst1Pdup =  dInst1P.deepCopy();
      
      LgFrMultiPartScheduleFloat::contextTest(dInst1P, dInst1Pdup);

      CLEAR_AND_DESTROY_SF_SCHEDULE_PTRS(dInst1Pdup);
    }


    testingMessage( "Testing LgFrMultiDemandScheduleFloat\n" );
    {
      director.construct(diner12Prob, calInst);
      LgFrScenarioSmartPointer  dInst1P = builder.getScenario();
      LgFrScenarioSmartPointer dInst1Pdup  =  dInst1P.deepCopy();
      
      LgFrMultiDemandScheduleFloat::contextTest(dInst1P, dInst1Pdup);

      CLEAR_AND_DESTROY_SF_SCHEDULE_PTRS(dInst1P);
      CLEAR_AND_DESTROY_SF_SCHEDULE_PTRS(dInst1Pdup);
    }*/

  }

  // smart pointer stuff

  testingMessage("Testing LgFrScenarioSmartPointerToConst\n");
  LgFrScenarioSmartPointerToConst::test();

  testingMessage("Testing LgFrScenarioSmartPointer\n");
  LgFrScenarioSmartPointer::test();

  testingMessage("Testing LgFrScenarioForCMRPSmartPointerToConst\n");
  LgFrScenarioForCMRPSmartPointerToConst::test();

  testingMessage("Testing LgFrScenarioForCMRPSmartPointer\n");
  LgFrScenarioForCMRPSmartPointer::test();

  testingMessage("Testing LgFrScenarioForATPSmartPointerToConst\n");
  //LgFrScenarioForATPSmartPointerToConst::test();

  testingMessage("Testing LgFrScenarioForATPSmartPointer\n");
  //LgFrScenarioForATPSmartPointer::test();

  testingMessage("Testing LgFrSortingScheduleFloatSmartPointerToConst\n");
  LgFrSortingScheduleFloatSmartPointerToConst::test();

  testingMessage("Testing LgFrSortingScheduleFloatSmartPointer\n");
  LgFrSortingScheduleFloatSmartPointer::test();

  testingMessage("Testing LgFrSortingDemandScheduleFloatSmartPointerToConst\n");
  LgFrSortingDemandScheduleFloatSmartPointerToConst::test();

  testingMessage("Testing LgFrSortingDemandScheduleFloatSmartPointer\n");
  LgFrSortingDemandScheduleFloatSmartPointer::test();

  testingMessage("Testing LgFrSortingPartScheduleFloatSmartPointerToConst\n");
  LgFrSortingPartScheduleFloatSmartPointerToConst::test();

  testingMessage("Testing LgFrSortingPartScheduleFloatSmartPointer\n");
  LgFrSortingPartScheduleFloatSmartPointer::test();

  testingMessage("Testing LgFrCmrpScenarioFacade\n");
  //LgFrCmrpScenarioFacade::test();

  testingMessage( "All tests completed successfully\n" );
#if defined(_MSC_VER)
  getchar();
#endif

  return 0;
}

 
// Display message on stdout and stderr
void testingMessage( const char * const msg )
{
   std::cerr <<msg;
   std::cout <<std::endl <<"*****************************************"
        <<std::endl <<msg <<std::endl;
}


void
pairCompareStrategiesTest ()
{
  // Make objects for scenario construction for more complete test of PCs
  LgFrDataInterfaceSpecialPurposeFromFile
    diner12Prob ("../../../../wit/wit-data/examples/diner12/wit.data");
  LgFrDataInterfaceSpecialPurposeFromFile
    deniseProb ("../../../../wit/wit-data/brenda/denise/wit.data");
  LgFrCalendar calInst;
  LgFrScenarioBuilderForDefault builder;
  LgFrScenarioDirector director(&builder);

  testingMessage( "Testing LgFrPairCompareFloat\n" );
  LgFrPairCompareFloat::test();

  testingMessage( "Testing LgFrTimeVecElementPairCompareFloat\n" );
  //LgFrTimeVecElementPairCompareFloat::test();
  
  testingMessage( "Testing LgFrSetOfPartsSensitivePairCompareFloat\n" );
  //LgFrSetOfPartsSensitivePairCompareFloat::test();

  /*testingMessage( "Testing LgFrPartCategoryPairCompareFloat\n" );
  {
    LgFrPartCategoryPairCompareFloat::test();

    director.construct (diner12Prob, calInst);
    LgFrScenarioSmartPointer  dInst1P = builder.getScenario();

    LgFrPartCategoryPairCompareFloat::contextTest
      (*dInst1P, dInst1P->lps_, dInst1P->soParts_);

  }

  testingMessage( "Testing LgFrScheduleSensitivePairCompareFloat\n" );
  {
    LgFrScheduleSensitivePairCompareFloat::test();

    // Make scenarios for more complete test of LgFrScheduleSensitiveFilterStrategy
    director.construct(deniseProb, calInst);
    LgFrScenarioSmartPointer dp = builder.getScenario();
    director.construct(deniseProb, calInst);
    LgFrScenarioSmartPointer d2p = builder.getScenario();
    // Make schedules for more ...
    LgFrSortingPartScheduleFloatSmartPointer criticalSchedule
      = dp->scheduleFactory() -> newPartSchedule (LGFRCriticalPartRankingList);
    LgFrSortingPartScheduleFloatSmartPointer supplySchedule
      = dp->scheduleFactory() -> newPartSchedule (LGFRSupplyVolume);
    LgFrSortingPartScheduleFloatSmartPointer productionSchedule
      = dp->scheduleFactory() -> newPartSchedule (LGFRProductionVolume);
    
    LgFrScheduleSensitivePairCompareFloat::contextTest
      (*dp, dp->lps_, criticalSchedule, supplySchedule, productionSchedule,
       *d2p, d2p->lps_);

    CLEAR_AND_DESTROY_SF_SCHEDULE_PTRS(dp);

    // supplySchedule & productionSchedule must be deleted before criticalSchedule
    // because when they are deleted, their subscription to 
    // criticalSchedule is removed.  criticalSchedule must exist when this is
    // done.
  }*/

  testingMessage( "Testing LgFrDemandPartNamePairCompare\n" );
  LgFrDemandPartNamePairCompare::test();

  /*testingMessage( "Testing LgFrDemandPriorityPairCompare\n" );
  {

    LgFrDemandPriorityPairCompare::test();
    
    director.construct(diner12Prob, calInst);
    LgFrScenarioSmartPointer dInst1P = builder.getScenario();
    LgFrSortingDemandScheduleFloatSmartPointer demandSchedule
      = dInst1P->scheduleFactory() -> newDemandSchedule (LGFRDemandVolume);
    
    LgFrDemandPriorityPairCompare::contextTest
      (*dInst1P, dInst1P->lps_, dInst1P->soDemands_,
       demandSchedule);

    CLEAR_AND_DESTROY_SF_SCHEDULE_PTRS(dInst1P);
  }

  testingMessage( "Testing LgFrDemandVolumeTimesGrossRevenuePairCompare\n" );
  {
    LgFrDemandVolumeTimesGrossRevenuePairCompare::test();
    
    director.construct(diner12Prob, calInst);
    LgFrScenarioSmartPointer dInst1P = builder.getScenario();
    LgFrSortingDemandScheduleFloatSmartPointer demandSchedule
      = dInst1P->scheduleFactory() -> newDemandSchedule (LGFRDemandVolume);
    
    LgFrDemandVolumeTimesGrossRevenuePairCompare::contextTest
      (*dInst1P, dInst1P->lps_, dInst1P->soDemands_,
       demandSchedule);

    CLEAR_AND_DESTROY_SF_SCHEDULE_PTRS(dInst1P);
  }

  testingMessage( "Testing LgFrDemandBacklogTimesGrossRevenuePairCompare\n" );
  {
    LgFrDemandBacklogTimesGrossRevenuePairCompare::test();
    
    director.construct(diner12Prob, calInst);
    LgFrScenarioSmartPointer dInst1P = builder.getScenario();

    LgFrDemandBacklogTimesGrossRevenuePairCompare::contextTest
      (*dInst1P, dInst1P->wip_, dInst1P->lps_, dInst1P->soDemands_);

  }*/
}

void
attributeScheduleStrategiesTest()
{
  // Make objects for scenario construction for more complete test of ASSs
  LgFrDataInterfaceSpecialPurposeFromFile
    diner12Prob ("../../../../wit/wit-data/examples/diner12/wit.data"),
    deniseProb ("../../../../wit/wit-data/brenda/denise/wit.data");
  LgFrCalendar calInst;
  LgFrCalendar calInst94 (6, LgFrDatetime("7/15/94"), LgFrMonth, 2);
  LgFrScenarioBuilderForDefault builder;
  LgFrScenarioDirector director(&builder);


  testingMessage( "Testing LgFrSortingPartScheduleFloat\n" );
  {
    director.construct(diner12Prob, calInst);
    LgFrScenarioSmartPointer dInst1P = builder.getScenario();

    LgFrSortingPartScheduleFloat::contextTest
      (*dInst1P, dInst1P->wip_, dInst1P->soParts_);

  }



  testingMessage( "Testing LgFrSupplyScheduleStrategy\n" );
  {
    LgFrSupplyScheduleStrategy::test();
  
    director.construct(diner12Prob, calInst);
    LgFrScenarioSmartPointer dInst1P = builder.getScenario();

    director.construct(diner12Prob, calInst);
    LgFrScenarioSmartPointer dInst2P = builder.getScenario();

    LgFrSupplyScheduleStrategy::contextTest
      (*dInst1P, dInst1P->wip_, dInst1P->soParts_, *dInst2P);

    std::cout << "Done testing LgFrSupplyScheduleStrategy\n";
  }

  
  /*testingMessage( "Testing LgFrSupplyScheduleStrategyForATP\n" );
  {
    LgFrDataInterfaceImplementation* difPWatpImpPtr
      = createPWatpDifImp();
    difPWatpImpPtr->title("internal data set pw2");
    registerPWatpDifImpFunctions(difPWatpImpPtr);
    LgFrDataInterface difPWatp(difPWatpImpPtr);
    delete difPWatpImpPtr;
    difPWatpImpPtr = 0;

    LgFrScenarioBuilderForATP builder1;
    LgFrScenarioDirector director1(&builder1);

    LgFrSupplyScheduleStrategyForATP::test();
  
    // need to get a ATP Master Schedule created
    // Can not use diner12Prob(LgFrDataInterfaceSpecialPurposeFromFile) 
    // for LgFrScenarioBuilderForATP because
    // LgFrDataInterfaceSpecialPurposeFromFile can only be used by 
    // LgFrScenarioBuilderForDefault which uses LgFrInitialProblemFromFile 
    // to read data directly from LgFrDataInterfaceSpecialPurposeFromFile.
    // Therefore, we use difPWatp to do this test here. JMT 8/14/95

    director1.construct(difPWatp, calInst);
    LgFrScenarioForATPSmartPointer dInst1P = builder1.getScenario();

    director1.construct(difPWatp, calInst);
    LgFrScenarioForATPSmartPointer dInst2P = builder1.getScenario();

    LgFrSupplyScheduleStrategyForATP::contextTest
      (dInst1P, dInst1P->wip_, dInst1P->soParts_, dInst2P);

    std::cout << "Done testing LgFrSupplyScheduleStrategyForATP\n";
  }
  
  testingMessage( "Testing LgFrBacklogScheduleStrategyForATP\n" );
  {
    LgFrDataInterfaceImplementation* difPWatpImpPtr
      = createPWatpDifImp();
    difPWatpImpPtr->title("internal data set pw2");
    registerPWatpDifImpFunctions(difPWatpImpPtr);
    LgFrDataInterface difPWatp(difPWatpImpPtr);
    delete difPWatpImpPtr;
    difPWatpImpPtr = 0;

    LgFrScenarioBuilderForATP builder1;
    LgFrScenarioDirector director1(&builder1);

    LgFrBacklogScheduleStrategyForATP::test();
  
    // need to get a ATP Master Schedule created
    director1.construct(difPWatp, calInst);
    LgFrScenarioForATPSmartPointer dInst1P = builder1.getScenario();

    LgFrBacklogScheduleStrategyForATP::contextTest
      (dInst1P, dInst1P->lps_, dInst1P->soDemands_ );

    std::cout << "Done testing LgFrBacklogScheduleStrategyForATP\n";
  }*/
  
  testingMessage( "Testing LgFrProductionScheduleStrategy\n" );
  {
    LgFrProductionScheduleStrategy::test();
  
    director.construct(diner12Prob, calInst);
    LgFrScenarioSmartPointer dInst1P = builder.getScenario();
    director.construct(diner12Prob, calInst);
    LgFrScenarioSmartPointer dInst2P = builder.getScenario();

    LgFrProductionScheduleStrategy::contextTest
      (*dInst1P, dInst1P->lps_, dInst1P->soParts_,
       *dInst2P, dInst2P->lps_);
    
  }


  testingMessage( "Testing LgFrRequirementScheduleStrategy\n" );
  {
    LgFrRequirementScheduleStrategy::test();

    director.construct (diner12Prob, calInst94);
    LgFrScenarioSmartPointer  dInst1P = builder.getScenario();

    LgFrRequirementScheduleStrategy::contextTest
      (*dInst1P, dInst1P->wip_, dInst1P->lps_, dInst1P->soParts_);

  }

  testingMessage( "Testing LgFrCmrpRequirementScheduleStrategy\n" );
  {
    LgFrCmrpRequirementScheduleStrategy::test();

    director.construct (diner12Prob, calInst94);
    LgFrScenarioSmartPointer dInst1P = builder.getScenario();

    LgFrCmrpRequirementScheduleStrategy::contextTest
      (*dInst1P, dInst1P->wip_, dInst1P->lps_, dInst1P->soParts_);

  }

  testingMessage( "Testing LgFrMrpConsVolScheduleStrategy\n" );
  {
    LgFrMrpConsVolScheduleStrategy::test();

    director.construct (diner12Prob, calInst94);
    LgFrScenarioSmartPointer dInst1P = builder.getScenario();

    LgFrMrpConsVolScheduleStrategy::contextTest
      (*dInst1P, dInst1P->wip_, dInst1P->lps_, dInst1P->soParts_);

  }


  testingMessage( "Testing LgFrFullyExplodedMrpConsVolScheduleStrategy\n" );
  {
    LgFrFullyExplodedMrpConsVolScheduleStrategy::test();

    director.construct (diner12Prob, calInst94);
    LgFrScenarioSmartPointer dInst1P = builder.getScenario();

    LgFrFullyExplodedMrpConsVolScheduleStrategy::contextTest
      (*dInst1P, dInst1P->wip_, dInst1P->lps_, dInst1P->soParts_);

  }
  
  
  testingMessage( "Testing LgFrCycleTimeScheduleStrategy\n" );
  {
    LgFrCycleTimeScheduleStrategy::test();

    director.construct (diner12Prob, calInst94);
    LgFrScenarioSmartPointer  dInst1P = builder.getScenario();

    LgFrCycleTimeScheduleStrategy::contextTest
      (*dInst1P, dInst1P->wip_, dInst1P->lps_, dInst1P->soParts_);

  }
   
  testingMessage( "Testing LgFrFrozenZoneScheduleStrategy\n" );
  {
    LgFrFrozenZoneScheduleStrategy::test();

    director.construct (diner12Prob, calInst94);
    LgFrScenarioSmartPointer  dInst1P = builder.getScenario();

    LgFrFrozenZoneScheduleStrategy::contextTest
      (*dInst1P, dInst1P->wip_, dInst1P->soParts_);

  }
   

  testingMessage( "Testing LgFrCriticalPartScheduleStrategy\n" );
  {
    LgFrCriticalPartScheduleStrategy::test();
    
    director.construct(diner12Prob, calInst);
    LgFrScenarioSmartPointer dInst1P = builder.getScenario();

    LgFrCriticalPartScheduleStrategy::contextTest
      (*dInst1P, dInst1P->lps_);

  }
  
  testingMessage( "Testing LgFrFocussedShortageScheduleScheduleStrategy\n" );
  {
    LgFrFocussedShortageScheduleScheduleStrategy::test();
    
    director.construct(diner12Prob, calInst);
    LgFrScenarioSmartPointer dInst1P = builder.getScenario();

    LgFrFocussedShortageScheduleScheduleStrategy::contextTest
      (*dInst1P, dInst1P->lps_, dInst1P->soParts_, dInst1P->soDemands_);

  }

  /*testingMessage( "Testing LgFrFocussedShortageScheduleScheduleStrategyForATP\n" );
  {
    LgFrFocussedShortageScheduleScheduleStrategyForATP::test();
    
    LgFrScenarioBuilderForATP builder1;
    LgFrScenarioDirector director1(&builder1);

    LgFrDataInterfaceImplementation* difPWatpImpPtr
      = createPWatpDifImp();
    difPWatpImpPtr->title("internal data set pw2");
    registerPWatpDifImpFunctions(difPWatpImpPtr);
    LgFrDataInterface difPWatp(difPWatpImpPtr);
    delete difPWatpImpPtr;
    difPWatpImpPtr = 0;

    director1.construct(difPWatp, calInst);
    LgFrScenarioForATPSmartPointer dInst1P = builder1.getScenario();
    LgFrFocussedShortageScheduleScheduleStrategyForATP::contextTest
      (dInst1P, dInst1P->lps_, dInst1P->soParts_, dInst1P->soDemands_);

    std::cout << "Done Testing LgFrFocussedShortageScheduleScheduleStrategyForATP"
      << std::endl;
  }*/
  
  testingMessage( "Testing LgFrFocussedShortageScheduleProductionScheduleStrategy\n" );
  {
    director.construct(diner12Prob, calInst);
    LgFrScenarioSmartPointer dInst1P = builder.getScenario();

    LgFrFocussedShortageScheduleProductionScheduleStrategy::contextTest
      (*dInst1P, dInst1P->lps_, dInst1P->soParts_, dInst1P->soDemands_);

  }

  testingMessage( "Testing LgFrExcessScheduleStrategy\n" );
  {
    LgFrExcessScheduleStrategy::test();
    
    director.construct(diner12Prob, calInst);
    LgFrScenarioSmartPointer dInst1P = builder.getScenario();

    LgFrExcessScheduleStrategy::contextTest
      (*dInst1P, dInst1P->lps_, dInst1P->soParts_);

  }

  testingMessage( "Testing LgFrDemandVolumeScheduleStrategy\n" );
  {
    LgFrDemandVolumeScheduleStrategy::test();
    
    director.construct(diner12Prob, calInst);
    LgFrScenarioSmartPointer dInst1P = builder.getScenario();

    LgFrDemandVolumeScheduleStrategy::contextTest
      (*dInst1P, dInst1P->wip_, dInst1P->soDemands_);

  }

  
  /*testingMessage( "Testing LgFrDemandVolumeScheduleStrategyForATP\n" );
  {

    LgFrScenarioBuilderForATP builder1;
    LgFrScenarioDirector director1(&builder1);

    LgFrDemandVolumeScheduleStrategyForATP::test();
      LgFrDataInterfaceImplementation* difPWatpImpPtr
      = createPWatpDifImp();
    difPWatpImpPtr->title("internal data set pw2");
    registerPWatpDifImpFunctions(difPWatpImpPtr);
    LgFrDataInterface difPWatp(difPWatpImpPtr);
    delete difPWatpImpPtr;
    difPWatpImpPtr = 0;

    // need to get a ATP Master Schedule created
    //   director1.construct(diner12Prob, calInst);
    // Can not use diner12Prob(LgFrDataInterfaceSpecialPurposeFromFile) 
    // for LgFrScenarioBuilderForATP because
    // LgFrDataInterfaceSpecialPurposeFromFile can only be used by 
    // LgFrScenarioBuilderForDefault which uses LgFrInitialProblemFromFile 
    // to read data directly from LgFrDataInterfaceSpecialPurposeFromFile.
    // Therefore, we use difPWatp to do this test here.  JMT 8/14/95
 
    director1.construct(difPWatp, calInst);
    LgFrScenarioForATPSmartPointer dInst1P = builder1.getScenario();

    director1.construct(difPWatp, calInst);
    LgFrScenarioForATPSmartPointer dInst2P = builder1.getScenario();

    LgFrDemandVolumeScheduleStrategyForATP::contextTest
      (dInst1P, dInst1P->wip_, dInst1P->soDemands_, dInst2P);

    std::cout << "Done testing LgFrDemandVolumeScheduleStrategyForATP\n";
  }*/

  testingMessage( "Testing LgFrScrapScheduleStrategy\n" );
  {
    LgFrScrapScheduleStrategy::test();
    
    director.construct(diner12Prob, calInst);
    LgFrScenarioSmartPointer dInst1P = builder.getScenario();

    LgFrScrapScheduleStrategy::contextTest
      (*dInst1P, dInst1P->lps_, dInst1P->soParts_);

  }

  testingMessage( "Testing LgFrStockScheduleStrategy\n" );
  {
    LgFrStockScheduleStrategy::test();
    
    director.construct(diner12Prob, calInst);
    LgFrScenarioSmartPointer dInst1P = builder.getScenario();

    LgFrStockScheduleStrategy::contextTest
      (*dInst1P, dInst1P->lps_, dInst1P->soParts_);

  }

  testingMessage( "Testing LgFrShipmentSchuduleStrategy\n" );
  {
    LgFrShipmentScheduleStrategy::test();
    
    director.construct(diner12Prob, calInst);
    LgFrScenarioSmartPointer dInst1P = builder.getScenario();
    director.construct(diner12Prob, calInst);
    LgFrScenarioSmartPointer dInst2P = builder.getScenario();

    LgFrShipmentScheduleStrategy::contextTest
      (*dInst1P, *dInst2P, dInst1P->lps_, dInst1P->soDemands_);
  }

  // DOESN'T WORK - DIDN'T WORK BEFORE EITHER
  // JMT 8/14/95, there are two problems here why it did not work before:
  // (1) diner12Prob is a LgFrDataInterfaceSpecialPurposeFromFile which can only
  //     work with LgFrInitialProblemFromFile (used by LgFrScenarioBuilderForDefault).
  // (2) In the contextTest code,    
  //     constSchedule only contains committed demands 
  //     and sod has both committed and firmForcast demands.
  //     In general, sod.size() != constSchedule.size() and I believe
  //     this is the reason why this test did not work before.
  // After making proper changes, this part now works.

  /*testingMessage( "Testing LgFrShipmentScheduleStrategyForATP\n" );
  {
    LgFrScenarioBuilderForATP builder1;
    LgFrScenarioDirector director1(&builder1);

    LgFrSupplyScheduleStrategyForATP::test();

    LgFrDataInterfaceImplementation* difPWatpImpPtr
      = createPWatpDifImp();
    difPWatpImpPtr->title("internal data set pw2");
    registerPWatpDifImpFunctions(difPWatpImpPtr);
    LgFrDataInterface difPWatp(difPWatpImpPtr);
    delete difPWatpImpPtr; 
    difPWatpImpPtr = 0;
  
    // need to get a ATP Master Schedule created
    // Can not use diner12Prob(LgFrDataInterfaceSpecialPurposeFromFile) 
    // for LgFrScenarioBuilderForATP because
    // LgFrDataInterfaceSpecialPurposeFromFile can only be used by 
    // LgFrScenarioBuilderForDefault which uses LgFrInitialProblemFromFile 
    // to read data directly from LgFrDataInterfaceSpecialPurposeFromFile.
    // Therefore, we use difPWatp to do this test here. JMT 8/14/95 

    director1.construct(difPWatp, calInst);
    LgFrScenarioForATPSmartPointer dInst1P = builder1.getScenario();

    
    LgFrShipmentScheduleStrategyForATP::test();
    
    LgFrShipmentScheduleStrategyForATP::contextTest
      (dInst1P, dInst1P->lps_, dInst1P->soDemands_);

  }

  testingMessage( "Testing LgFrBacklogScheduleStrategy\n" );
  {
     LgFrBacklogScheduleStrategy::test();
    
    director.construct(diner12Prob, calInst);
    LgFrScenarioSmartPointer dInst1P = builder.getScenario();

    LgFrBacklogScheduleStrategy::contextTest
      (*dInst1P, dInst1P->lps_, dInst1P->soDemands_);

  }*/



  testingMessage( "Testing LgFrPartScheduleSensitiveScheduleStrategy\n" );
  {
    LgFrPartScheduleSensitiveScheduleStrategy::test();
    
    director.construct(diner12Prob, calInst);
    LgFrScenarioSmartPointer dInst1P = builder.getScenario();

    LgFrSortingPartScheduleFloatSmartPointer supplySchedule
      = dInst1P->scheduleFactory() -> newPartSchedule (LGFRSupplyVolume);

    LgFrPartScheduleSensitiveScheduleStrategy::contextTest
      (*dInst1P, supplySchedule );

    CLEAR_AND_DESTROY_SF_SCHEDULE_PTRS(dInst1P);
  }

  testingMessage( "Testing LgFrDemandScheduleSensitiveScheduleStrategy\n" );
  {
    LgFrDemandScheduleSensitiveScheduleStrategy::test();
    
    director.construct(diner12Prob, calInst);
    LgFrScenarioSmartPointer dInst1P = builder.getScenario();

    LgFrSortingDemandScheduleFloatSmartPointer demandVolSchedule
      = dInst1P->scheduleFactory() -> newDemandSchedule (LGFRDemandVolume);

    LgFrDemandScheduleSensitiveScheduleStrategy::contextTest
      (*dInst1P, demandVolSchedule );

    CLEAR_AND_DESTROY_SF_SCHEDULE_PTRS(dInst1P);
  }

  testingMessage( "Testing LgFrCumTimeVecPartScheduleStrategy\n" );
  {
    LgFrCumTimeVecPartScheduleStrategy::test();
    
    director.construct(diner12Prob, calInst);
    LgFrScenarioSmartPointer dInst1P = builder.getScenario();

    LgFrSortingPartScheduleFloatSmartPointer supplySchedule
      = dInst1P->scheduleFactory() -> newPartSchedule (LGFRSupplyVolume);

    LgFrCumTimeVecPartScheduleStrategy::contextTest
      (*dInst1P, supplySchedule);

    CLEAR_AND_DESTROY_SF_SCHEDULE_PTRS(dInst1P);
  }

  testingMessage( "Testing LgFrCumTimeVecDemandScheduleStrategy\n" );
  {
    LgFrCumTimeVecDemandScheduleStrategy::test();
    
    director.construct(diner12Prob, calInst);
    LgFrScenarioSmartPointer dInst1P = builder.getScenario();

    LgFrSortingDemandScheduleFloatSmartPointer demandVolSchedule
      = dInst1P->scheduleFactory() -> newDemandSchedule (LGFRDemandVolume);

    LgFrCumTimeVecDemandScheduleStrategy::contextTest
      (*dInst1P, demandVolSchedule);

    CLEAR_AND_DESTROY_SF_SCHEDULE_PTRS(dInst1P);
  }


  testingMessage( "Testing LgFrCumDemandsPartScheduleStrategy\n" );
  {
    LgFrCumDemandsPartScheduleStrategy::test();
    
    director.construct(deniseProb, calInst);
    LgFrScenarioSmartPointer dInst1P = builder.getScenario();

    LgFrSortingDemandScheduleFloatSmartPointer demandSchedule
      = dInst1P->scheduleFactory() -> newDemandSchedule (LGFRDemandVolume);

    LgFrCumDemandsPartScheduleStrategy::contextTest
      (*dInst1P, demandSchedule);

    CLEAR_AND_DESTROY_SF_SCHEDULE_PTRS(dInst1P);
  }

  testingMessage( "Testing LgFrPartSummaryScheduleStrategy\n" );
  {
    LgFrPartSummaryScheduleStrategy::test();
    
    director.construct(diner12Prob, calInst);
    LgFrScenarioSmartPointer dInst1P = builder.getScenario();

    LgFrSortingPartScheduleFloatSmartPointer supplySchedule
      = dInst1P->scheduleFactory() -> newPartSchedule (LGFRSupplyVolume);

    LgFrPartSummaryScheduleStrategy::contextTest
      (*dInst1P, supplySchedule);

    CLEAR_AND_DESTROY_SF_SCHEDULE_PTRS(dInst1P);
  }

  testingMessage( "Testing LgFrDemandSummaryScheduleStrategy\n" );
  {
    LgFrDemandSummaryScheduleStrategy::test();

    director.construct(diner12Prob, calInst);
    LgFrScenarioSmartPointer dInst1P = builder.getScenario();

    LgFrSortingDemandScheduleFloatSmartPointer demandSchedule
      = dInst1P->scheduleFactory() -> newDemandSchedule (LGFRDemandVolume);

    LgFrDemandSummaryScheduleStrategy::contextTest
      (*dInst1P, demandSchedule);

    CLEAR_AND_DESTROY_SF_SCHEDULE_PTRS(dInst1P);
  }


  testingMessage( "Testing LgFrPartScheduleTimesUnitCostScheduleStrategy\n" );
  {
    LgFrPartScheduleTimesUnitCostScheduleStrategy::test();
    
    director.construct(diner12Prob, calInst);
    LgFrScenarioSmartPointer dInst1P = builder.getScenario();

    LgFrSortingPartScheduleFloatSmartPointer supplySchedule
      = dInst1P->scheduleFactory() -> newPartSchedule (LGFRSupplyVolume);

    LgFrPartScheduleTimesUnitCostScheduleStrategy::contextTest
      (*dInst1P, supplySchedule);

    CLEAR_AND_DESTROY_SF_SCHEDULE_PTRS(dInst1P);
  }

  testingMessage( "Testing LgFrTwoScheduleDeltaPartScheduleStrategy\n" );
  {
    LgFrTwoScheduleDeltaPartScheduleStrategy::test();
    
    director.construct(diner12Prob, calInst);
    LgFrScenarioSmartPointer dInst1P = builder.getScenario();

    LgFrSortingPartScheduleFloatSmartPointer supplySchedule
      = dInst1P->scheduleFactory() -> newPartSchedule (LGFRSupplyVolume);

    LgFrTwoScheduleDeltaPartScheduleStrategy::contextTest
      (*dInst1P, supplySchedule);

    CLEAR_AND_DESTROY_SF_SCHEDULE_PTRS(dInst1P);
  }

  testingMessage( "Testing LgFrApplyOffsetScheduleStrategy\n" );
  {
    LgFrApplyOffsetScheduleStrategy::test();
    
    director.construct(diner12Prob, calInst);
    LgFrScenarioSmartPointer dInst1P = builder.getScenario();

    LgFrSortingPartScheduleFloatSmartPointer supplySchedule
      = dInst1P->scheduleFactory() -> newPartSchedule (LGFRSupplyVolume);

    LgFrSortingPartScheduleFloatSmartPointer cycleTimeSchedule
      = dInst1P->scheduleFactory() -> newPartSchedule (LGFRCycleTime);

    LgFrApplyOffsetScheduleStrategy::contextTest
      (*dInst1P, supplySchedule, cycleTimeSchedule);

    CLEAR_AND_DESTROY_SF_SCHEDULE_PTRS(dInst1P);
  }

  testingMessage( "Testing LgFrUsageScheduleStrategy\n" );
  {
    LgFrUsageScheduleStrategy::test();
    
    director.construct(diner12Prob, calInst);
    LgFrScenarioSmartPointer dInst1P = builder.getScenario();

    LgFrSortingPartScheduleFloatSmartPointer supplySchedule
      = dInst1P->scheduleFactory() -> newPartSchedule (LGFRSupplyVolume);

    LgFrSortingPartScheduleFloatSmartPointer prodSchedule
      = dInst1P->scheduleFactory() -> newPartSchedule (LGFRProductionVolume);

    LgFrSortingPartScheduleFloatSmartPointer stockSchedule
      = dInst1P->scheduleFactory() -> newPartSchedule (LGFRStockVolume);

    LgFrSortingPartScheduleFloatSmartPointer scrapSchedule
      = dInst1P->scheduleFactory() -> newPartSchedule (LGFRScrapVolume);

    LgFrUsageScheduleStrategy::contextTest
      (*dInst1P, supplySchedule, prodSchedule, stockSchedule, scrapSchedule);

    CLEAR_AND_DESTROY_SF_SCHEDULE_PTRS(dInst1P);
  }
  
  testingMessage( "Testing LgFrPriorityScheduleStrategy\n" );
  {
    LgFrPriorityScheduleStrategy::test();
    
    director.construct(diner12Prob, calInst);
    LgFrScenarioSmartPointer dInst1P = builder.getScenario();

    LgFrPriorityScheduleStrategy::contextTest
      (*dInst1P, dInst1P->wip_, dInst1P->soDemands_ );

  }
  
  testingMessage( "Testing LgFrObj1ShipRewardScheduleStrategy\n" );
  {
    LgFrObj1ShipRewardScheduleStrategy::test();

    director.construct(diner12Prob, calInst);
    LgFrScenarioSmartPointer dInst1P = builder.getScenario();

    LgFrObj1ShipRewardScheduleStrategy::contextTest
      (*dInst1P, dInst1P->wip_, dInst1P->soDemands_ );
  }
  
  /*testingMessage( "Testing LgFrShipSoftLowerBoundScheduleStrategy\n" );
  {
    LgFrShipSoftLowerBoundScheduleStrategy::test();

    director.construct(diner12Prob, calInst);
    LgFrScenarioSmartPointer dInst1P = builder.getScenario();

    LgFrShipSoftLowerBoundScheduleStrategy::contextTest
      (*dInst1P, dInst1P->wip_, dInst1P->soDemands_ );
  }*/

}

void
dataInterfaceTest( )
{
/*
  testingMessage( "Testing LgFrDataInterfaceSpecialPurposeFromFile\n" );
  LgFrDataInterfaceSpecialPurposeFromFile::test();

  testingMessage( "Testing LgFrDataIntefaceImplementationPerfectWorldCMRP\n" );
  {

    LgFrDataInterfaceImplementation * difPWImpPtr
      = new LgFrDataInterfaceImplementation("internal data set pw1");
    LgFrDataInterface dif(difPWImpPtr);
    delete difPWImpPtr;
    difPWImpPtr = 0;

    LgFrCalendar calInst;    
    LgFrScenarioBuilderForDifTesting builder;
    LgFrScenarioDirector director(&builder);
    director.construct(dif, calInst);
    LgFrScenarioSmartPointer s1 = builder.getScenario();
    LgFrScenarioSmartPointer sDup  =  s1.deepCopy();

    difPWcmrpContextTest(
	(*((LgFrDataInterface*)(s1->difPtr_))),
	(*((LgFrDataInterface*)(sDup->difPtr_))),
	&(*s1)
	);

  }

  testingMessage( "Testing LgFrDataIntefaceImplementationPerfectWorldATP\n" );
  {

    LgFrDataInterfaceImplementation * difPWImpPtr
      = new LgFrDataInterfaceImplementation("internal data set pw2");
    LgFrDataInterface dif(difPWImpPtr);
    delete difPWImpPtr;
    difPWImpPtr = 0;


    LgFrCalendar calInst;    
    LgFrScenarioBuilderForDifTesting builder;
    LgFrScenarioDirector director(&builder);
    director.construct(dif, calInst);
    LgFrScenarioSmartPointer s1 = builder.getScenario();
    LgFrScenarioSmartPointer sDup  =  s1.deepCopy();

    difPWatpContextTest(
	(*((LgFrDataInterface*)(s1->difPtr_))),
	(*((LgFrDataInterface*)(sDup->difPtr_))),
	&(*s1)
	);

  }

  testingMessage( "Testing LgFrDataIntefaceImplementationVolkswagenCMRP\n" );
  {
    
    LgFrDataInterfaceImplementation * difVWImpPtr
      = new LgFrDataInterfaceImplementation("internal data set automobile1");
    LgFrDataInterface dif(difVWImpPtr);
    delete difVWImpPtr;
    difVWImpPtr = 0;


    LgFrCalendar calInst;    
    LgFrScenarioBuilderForDifTesting builder;
    LgFrScenarioDirector director(&builder);
    director.construct(dif, calInst);
    LgFrScenarioSmartPointer s1 = builder.getScenario();
    LgFrScenarioSmartPointer sDup  =  s1.deepCopy();

    difVWcmrContextTest(
	(*((LgFrDataInterface*)(s1->difPtr_))),
	(*((LgFrDataInterface*)(sDup->difPtr_))),
	&(*s1)
	);


  }

  testingMessage( "Testing LgFrDataIntefaceImplementationFromWitDataFile\n" );
  {
    LgFrDataInterfaceImplementation * dif1ImpPtr    
      = new LgFrDataInterfaceImplementationFromWitDataFile(
                      "../../../../wit/wit-data/examples/diner12/wit.data");
    LgFrDataInterface dif1(dif1ImpPtr);
    delete dif1ImpPtr;
    dif1ImpPtr = 0;

    LgFrDataInterfaceImplementation * dif2ImpPtr    
      = new LgFrDataInterfaceImplementationFromWitDataFile(
                       "../../../../wit/wit-data/examples/difWit/wit.data");
    LgFrDataInterface dif2(dif2ImpPtr);
    delete dif2ImpPtr;
    dif2ImpPtr = 0;


    LgFrCalendar calInst;

    LgFrScenarioBuilderForDifTesting builder1, builder2;
    LgFrScenarioDirector director1(&builder1), director2(&builder2);
    director1.construct(dif1, calInst);
    director2.construct(dif2, calInst);
    LgFrScenarioSmartPointer s1 = builder1.getScenario();
    LgFrScenarioSmartPointer s2 = builder2.getScenario();
    LgFrScenarioSmartPointer s1Dup  =  s1.deepCopy();
    LgFrDataInterfaceImplementationFromWitDataFile::contextTest(
       ((LgFrDataInterface*)(s1->difPtr_)),
       ((s1->lps_)),
       ((LgFrDataInterface*)(s1Dup->difPtr_)),
       ((LgFrDataInterface*)(s2->difPtr_)),
       &(*s1));
    
  }

    testingMessage( "Testing LgFrDataIntefaceImplementationFromWitRun\n" );
  {
    WitRun *witRun,*witRun2;
    witNewRun ( &witRun);
    witNewRun ( &witRun2);
    witSetMesgTimesPrint(witRun, WitTRUE, WitINFORMATIONAL_MESSAGES, 0);
    witSetMesgTimesPrint(witRun2, WitTRUE, WitINFORMATIONAL_MESSAGES, 0);
    witInitialize(witRun);

    witSetMesgTimesPrint( witRun, WitTRUE, 538, 0 );
    witSetWit34Compatible( witRun, WitTRUE );

    witReadData(witRun, "../../../../wit/wit-data/examples/difWit/wit.data");
    LgFrDataInterfaceImplementation * difImpPtr    
      = new LgFrDataInterfaceImplementationFromWitRun(
                       witRun,             
                       "RiskForecast.*",
                       "FirmForecast.*",
                       "NewOps.*");
    

    LgFrDataInterface dif(difImpPtr);
    delete difImpPtr;
    difImpPtr = 0; 

    witCopyData( witRun2, witRun );

    LgFrDataInterfaceImplementation * difImpPtr2    
      = new LgFrDataInterfaceImplementationFromWitRun(
                       witRun2,             
                       "RiskForecast.*",
                       "FirmForecast.*",
                       "NewOps.*");
    
    LgFrDataInterface dif2(difImpPtr2);
    delete difImpPtr2;
    difImpPtr2 = 0; 

    LgFrCalendar calInst;
    LgFrScenarioBuilderForDifTesting builder, builder2;
    LgFrScenarioDirector director(&builder), director2(&builder2);
    director.construct(dif, calInst);
    director2.construct(dif2, calInst);
    LgFrScenarioSmartPointer s1 = builder.getScenario();
    LgFrScenarioSmartPointer s2 = builder2.getScenario();
    LgFrScenarioSmartPointer sDup  = s1.deepCopy();

    LgFrDataInterfaceImplementationFromWitRun::contextTest(
       ((LgFrDataInterface*)(s1->difPtr_)),
       ((s1->lps_)),
       ((LgFrDataInterface*)(sDup->difPtr_)),
       ((LgFrDataInterface*)(s2->difPtr_)),
       &(*s1));
    
  }
  
  testingMessage( "Testing LgFrDataIntefaceImplementationFromCmrpFile\n" );
  {
    LgFrDataInterfaceImplementation * difImpPtr
	= new LgFrDataInterfaceImplementationFromCmrpFiles(
                      CMRP_DATA_PATH "difTest");   
    LgFrDataInterfaceImplementation * difImpPtr2
	= new LgFrDataInterfaceImplementationFromCmrpFiles(
                      CMRP_DATA_PATH "difTest");

    LgFrCalendar calInst( 3,RWTime(RWDate(10,4,1995)),LgFrDay);
    LgFrDataInterface dif(difImpPtr);
    LgFrDataInterface dif2(difImpPtr2);
    delete difImpPtr;
    delete difImpPtr2;
    difImpPtr = 0; 
    difImpPtr2= 0; 

    LgFrScenarioBuilderForDifTesting builder, builder2;
    LgFrScenarioDirector director(&builder), director2(&builder2);
    director.construct(dif, calInst);
    director2.construct(dif2, calInst);

    LgFrScenarioSmartPointer s1 = builder.getScenario();
    LgFrScenarioSmartPointer s2 = builder2.getScenario();

    LgFrScenarioSmartPointer sDup  =  s1.deepCopy();
    LgFrDataInterfaceImplementationFromCmrpFiles::contextTest(
        ((LgFrDataInterface*)(s1->difPtr_)),
        ((s1->lps_)),
        ((LgFrDataInterface*)(sDup->difPtr_)),
        ((LgFrDataInterface*)(s2->difPtr_)),
	&(*s1));

  }
  
  testingMessage( "Testing LgFrDataIntefaceImplementationForTest\n" );
  {

    LgFrDataInterfaceImplementation * difImpPtr
      = new LgFrDataInterfaceImplementation(
	  "Test problem for LgFrDataInterfaceImplementation");
    LgFrDataInterface dif(difImpPtr);
    delete difImpPtr;
    difImpPtr = 0; 

    LgFrCalendar calInst;    
    LgFrScenarioBuilderForDifTesting builder;
    LgFrScenarioDirector director(&builder);
    director.construct(dif, calInst);
    LgFrScenarioSmartPointer s1 = builder.getScenario();
    LgFrScenarioSmartPointer sDup  =  s1.deepCopy();

    difTestContextTest(
        *(s1),
	(s1->lps_),
	(*((LgFrDataInterface*)(s1->difPtr_))),
	*sDup,
	(*((LgFrDataInterface*)(sDup->difPtr_)))
	);

  }*/

}


void
filterStrategiesTest( )
{
  testingMessage( "Testing LgFrFilterStrategy\n" );
  LgFrFilterStrategy::test();

  testingMessage( "Testing LgFrZeroTimeVecFilterStrategy\n" );
  LgFrZeroTimeVecFilterStrategy::test();

  testingMessage( "Testing LgFrDemandNameFilterStrategy\n" );
  LgFrDemandNameFilterStrategy::test();

  testingMessage( "Testing LgFrPartNameFilterStrategy\n" );
  //LgFrPartNameFilterStrategy::test();

  testingMessage( "Testing LgFrSetOfPartsFilterStrategy\n" );
  {
    //LgFrSetOfPartsFilterStrategy::test();
    
    LgFrDataInterfaceSpecialPurposeFromFile
      diner12Prob ("../../../../wit/wit-data/examples/diner12/wit.data");
    LgFrCalendar calInst;

    LgFrScenarioBuilderForDefault builder;
    LgFrScenarioDirector director(&builder);
    director.construct(diner12Prob, calInst);
    LgFrScenarioSmartPointer dInst1P = builder.getScenario();

    //LgFrSetOfPartsFilterStrategy::contextTest (*dInst1P,
     // dInst1P->setOfParts( LGFR_UNIVERSAL_SET ) );
  }

  testingMessage( "Testing LgFrPartCategoryFilterStrategy\n" );
  {
    LgFrPartCategoryFilterStrategy::test();
    
    LgFrDataInterfaceSpecialPurposeFromFile
      diner12Prob ("../../../../wit/wit-data/examples/diner12/wit.data");
    LgFrCalendar calInst;

    LgFrScenarioBuilderForDefault builder;
    LgFrScenarioDirector director(&builder);
    director.construct(diner12Prob, calInst);
    LgFrScenarioSmartPointer dInst1P = builder.getScenario();

    LgFrPartCategoryFilterStrategy::contextTest (*dInst1P);
  }
  
  testingMessage( "Testing LgFrScheduleSensitiveFilterStrategy\n" );
  LgFrSortingPartScheduleFloatSmartPointer supplySchedule;
  LgFrSortingPartScheduleFloatSmartPointer criticalSchedule;

  {
    LgFrScheduleSensitiveFilterStrategy::test();

    // Make scenario for more complete test of 
    // LgFrScheduleSensitiveFilterStrategy

    LgFrDataInterfaceSpecialPurposeFromFile    
      deniseProb ("../../../../wit/wit-data/brenda/denise/wit.data");
    LgFrCalendar calInst;
    LgFrScenarioBuilderForDefault builder;
    LgFrScenarioDirector director(&builder);
    director.construct(deniseProb, calInst);
    LgFrScenarioSmartPointer dp = builder.getScenario();
    // Make schedules for more ...

    LgFrSortingPartScheduleFloatSmartPointer criticalSchedule;
    criticalSchedule = dp->scheduleFactory()->newPartSchedule(LGFRCriticalPartRankingList);

    LgFrSortingPartScheduleFloatSmartPointer supplySchedule;
    supplySchedule = dp->scheduleFactory()->newPartSchedule(LGFRSupplyVolume);
    LgFrScheduleSensitiveFilterStrategy::contextTest
      (*dp, dp->lps_, criticalSchedule, supplySchedule);

    CLEAR_AND_DESTROY_SF_SCHEDULE_PTRS(dp);

    // supplySchedule must be deleted before the criticalSchedule,
    // because when supplySchedule is deleted, its subscription to the
    // criticalSchedule is removed. The criticalSchedule must exist when this is
    // done.
  }

  testingMessage( "Testing LgFrNotFilterStrategy\n" );
  LgFrNotFilterStrategy::test();

  testingMessage( "Testing LgFrAndFilterStrategy\n" );
  LgFrAndFilterStrategy::test();

  testingMessage( "Testing LgFrOrFilterStrategy\n" );
  LgFrOrFilterStrategy::test();

}

