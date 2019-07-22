#include <mcl/src/mcl.h>
#include <sce/src/schFSce.h>
#include <sce/src/scenSceP.h>
#include <scenario/src/schdDeSt.h>
#include <sce/src/scDeSce.h>
#include <scenario/src/sortSchd.h>
#include <sce/src/scenSce.h>
#include <scenario/src/scenario.h>
#include <scenario/src/supplySS.h>
#include <scenario/src/partSchd.h>
#include <scenario/src/demdSchd.h>
#include <scenario/src/partSchP.h>
#include <scenario/src/demdSchP.h>
#include <scenario/src/zeroTvFS.h>


// Create a new sorting part schedule
LgFrSortingPartScheduleFloatSmartPointer
  LgFrScheduleFactoryForSce::newPartScheduleFromScratch (const LgFrScheduleDescriptor& desc,
					       const bool deepCopy,
					       const bool toPopulate
					       )
     const
{
  LgFrPartScheduleStrategy *partSS;
  LgFrFilterStrategy * filterStrategyPtr = 0;
  LgFrPairCompareFloat * pairComparePtr = 0;
  bool ascending = true;
  LgFrScenarioForSceSmartPointer scenario = LgFrScenarioForSceSmartPointer(this->scenario());

//summary schedules for the default set of parts for Sce
  if ( desc == LGFRInterPlantSupplyVolume ) {
    partSS = new LgFrSupplyScheduleStrategy (*scenario);
    partSS->setOfParts(scenario->constSetOfParts(LGFR_INTERPLANT_SET) );
  }
  else
    return LgFrScheduleFactory::newPartScheduleFromScratch(desc, deepCopy, toPopulate);

  LgFrSortingPartScheduleFloatSmartPointer spsf = createPartSched(
								  *partSS,
								  filterStrategyPtr,
								  pairComparePtr,
								  ascending );
  if ( toPopulate ) 
      if ( this->scheduleType(desc,*partSS) == INPUT )
	  spsf->populate();
      else {
	  LgFrLastProblemSolved& lps = this->lastProblemSolved();
	  if ( lps.imploded() ) spsf->populate();
      }
  delete partSS;
  delete filterStrategyPtr;
  delete pairComparePtr;

  return spsf;
}

// Create a new sorting demand schedule
LgFrSortingDemandScheduleFloatSmartPointer
  LgFrScheduleFactoryForSce::newDemandScheduleFromScratch(const LgFrScheduleDescriptor& desc,
						const bool deepCopy,
						const bool toPopulate
					       )
     const
{
  LgFrDemandScheduleStrategy *demandSS;
  LgFrScenarioForSceSmartPointer scenario = LgFrScenarioForSceSmartPointer(this->scenario());

    return LgFrScheduleFactory::newDemandScheduleFromScratch(desc, deepCopy, toPopulate);

}

// ---------------------------------------------------------------------
// Return the schedule type: INPUT or OUTPUT of implosion run.
// ---------------------------------------------------------------------
schdType
LgFrScheduleFactoryForSce::scheduleType(
    const LgFrScheduleDescriptor & desc,
    const LgFrAttributeScheduleStrategy & ass)
     const
{
//A schedule is an input schedule if it can be built prior to implosion.
//It doesn't matter if you don't have a schedule in here even if it is
//an input schedule. JP says that the demand volume schedules are actually
//an output schedule, but since there is no way to tell from wit whether or
//not an implosion has been done, these are put in here anyway.
  if ( desc == LGFRSupplyVolume ) return INPUT;	// Doing it this way (instead
  if ( desc == LGFRDemandVolume ) return INPUT; // of using ||) allows
						// inlining
  if ( desc == LGFRMRPManagedSupplyVol ) return INPUT;
  if ( desc == LGFRMRPManagedRequiredVol ) return INPUT;
  if ( desc == LGFRMRPManagedCumSupplyVol ) return INPUT;
  if ( desc == LGFRMRPManagedCumRequiredVol ) return INPUT;
  if ( desc == LGFRCycleTime )   return INPUT;
  if ( desc == LGFRFrozenZone )   return INPUT;
  if ( desc == LGFRProcurementLeadTime ) return INPUT;
  if ( desc == LGFRPlannerManagedSupplyVol ) return INPUT;
  if ( desc == LGFRPlannerManagedRequiredVol ) return INPUT;
  if ( desc == LGFRPlannerManagedCumSupplyVol ) return INPUT;
  if ( desc == LGFRPlannerManagedCumRequiredVol ) return INPUT;
  if ( desc == LGFRCapacitySupplyVol ) return INPUT;
  if ( desc == LGFRCapacityRequiredVol ) return INPUT;
  if ( desc == LGFRBudgetAmount ) return INPUT;

  if ( desc == LGFRCMRPDemandVol ) return INPUT;
  if ( desc == LGFRCMRPCommittedDemandVol ) return INPUT;
  if ( desc == LGFRCMRPTotalCommittedDemandVol ) return INPUT;

  if ( desc == LGFRCMRPTotalMrpProcurementCost) return INPUT;
  if ( desc == LGFRCMRPTotalWitProcurementCost) return INPUT;
  if ( desc == LGFRCMRPCumTotalMrpProcurementCost) return INPUT;
  if ( desc == LGFRCMRPCumTotalWitProcurementCost) return INPUT;
  if ( desc == LGFRCMRPDeltaCumTotalProcurementCost) return INPUT;
  if ( desc == LGFRProductionVolumeTimesUnitCost) return INPUT;
  if ( desc == LGFRProcurementVolumeTimesUnitCost) return INPUT;
  if ( desc == LGFRRequiredVolumeTimesUnitCost) return INPUT;

  return this->LgFrScheduleFactory::scheduleType(desc,ass);
}

// Constructor
LgFrScheduleFactoryForSce::LgFrScheduleFactoryForSce (
    LgFrScenarioSmartPointer s)
:   LgFrScheduleFactory(s)
{
  // Nothing to do in here
}

// Clone method
// Create a copy of the *this on the heap and return a pointer to it
// Even thought this returns a LgFrScheduleFactory*, the pointer
// must point to a LgFrScheduleFactoryForSce.
LgFrScheduleFactory*
LgFrScheduleFactoryForSce::clone (LgFrScenarioSmartPointer newScenario)
     const
{
  return  new LgFrScheduleFactoryForSce (newScenario);
}

// Destructor
LgFrScheduleFactoryForSce::~LgFrScheduleFactoryForSce()
{
  // Nothing to do in here
}

#ifdef NDEBUG
#undef NDEBUG
#endif

// test this class
void LgFrScheduleFactoryForSce::test()
{
}


void
LgFrScheduleFactoryForSce::contextTest(const LgFrScheduleFactory & f)
{
}

