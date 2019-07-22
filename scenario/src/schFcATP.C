#include <scenario/src/schFcATP.h>
#include <scenario/src/schdDeSt.h>
#include <scenario/src/sortSchd.h>
#include <scenario/src/partSS.h>
#include <scenario/src/demVolSS.h>
#include <scenario/src/demSSATP.h>
#include <scenario/src/shipSS.h>
#include <scenario/src/backlgSS.h>
#include <scenario/src/cumSS.h>
#include <scenario/src/fssSS.h>
#include <scenario/src/scenario.h>
#include <scenario/src/supplySS.h>
#include <scenario/src/supSSATP.h>
#include <scenario/src/shpSSATP.h>
#include <scenario/src/bacSSATP.h>
#include <scenario/src/fssSSATP.h>
#include <scenario/src/scenATP.h>
#include <scenario/src/partSchP.h>
#include <scenario/src/demdSchP.h>
#include <scenario/src/zeroTvFS.h>

// Create a new sorting part schedule
LgFrSortingPartScheduleFloatSmartPointer
LgFrScheduleFactoryForATP::newPartScheduleFromScratch (const LgFrScheduleDescriptor& desc,
					    const RWBoolean deepCopy,
					    const RWBoolean toPopulate
					    )
     const
{
  LgFrPartScheduleStrategy *partStrg;
  LgFrScenarioForATPSmartPointer scenario = this->scenario();

  //---------------------------------------------------------------------------
  //   Material Recovery 
  //---------------------------------------------------------------------------

  if ( desc == LGFRMaterialSupplyVolume) {
      partStrg = new LgFrSupplyScheduleStrategyForATP(scenario,
						      LGFR_ATP_STATE_COMMITTED_IMPLODED_RECOVERY);
      partStrg->setOfParts(scenario->constSetOfParts(LGFR_MATERIAL_SET));
  }

  else if ( desc == LGFRMaterialCumSupplyVolume) {
      partStrg = new LgFrCumTimeVecPartScheduleStrategy(*scenario);
      LgFrCumTimeVecPartScheduleStrategy * cumSS = 
	  (LgFrCumTimeVecPartScheduleStrategy *) partStrg;
      LgFrSortingPartScheduleFloatSmartPointer dependentSched =
	  this->newPartSchedule(LGFRMaterialSupplyVolume, deepCopy, toPopulate);
      cumSS->insertPartSchedule(dependentSched);
      partStrg->setOfParts(scenario->constSetOfParts(LGFR_MATERIAL_SET));
  }

  else if ( desc == LGFRMaterialFocussedShortageSchedule) {
      partStrg = new LgFrFocussedShortageScheduleScheduleStrategyForATP(scenario,
									LGFR_ATP_STATE_COMMITTED_IMPLODED_RECOVERY);
      partStrg->setOfParts(scenario->constSetOfParts(LGFR_MATERIAL_SET));
  }

  else if ( desc == LGFRMaterialCumFocussedShortageSchedule) {
      partStrg = new LgFrCumTimeVecPartScheduleStrategy(*scenario);
      LgFrCumTimeVecPartScheduleStrategy * cumSS = 
	  (LgFrCumTimeVecPartScheduleStrategy *) partStrg;
      LgFrSortingPartScheduleFloatSmartPointer dependentSched =
	  this->newPartSchedule(LGFRMaterialFocussedShortageSchedule, deepCopy, toPopulate);
      cumSS->insertPartSchedule(dependentSched);
      partStrg->setOfParts(scenario->constSetOfParts(LGFR_MATERIAL_SET));
  }

  //---------------------------------------------------------------------------
  //   Capacity Recovery 
  //---------------------------------------------------------------------------

  else if ( desc == LGFRCapacitySupplyVolume) {
      partStrg = new LgFrSupplyScheduleStrategyForATP(scenario,
						      LGFR_ATP_STATE_COMMITTED_IMPLODED_RECOVERY);
      partStrg->setOfParts(scenario->constSetOfParts(LGFR_CAPACITY_SET));
  }

  else if ( desc == LGFRCapacityFocussedShortageSchedule) {
      partStrg = new LgFrFocussedShortageScheduleScheduleStrategyForATP(scenario,
									LGFR_ATP_STATE_COMMITTED_IMPLODED_RECOVERY);
      partStrg->setOfParts(scenario->constSetOfParts(LGFR_CAPACITY_SET));
  }

  else if ( desc == LGFRCapacityCumFocussedShortageSchedule) {
      partStrg = new LgFrCumTimeVecPartScheduleStrategy(*scenario);
      LgFrCumTimeVecPartScheduleStrategy * cumSS = 
	  (LgFrCumTimeVecPartScheduleStrategy *) partStrg;
      LgFrSortingPartScheduleFloatSmartPointer dependentSched =
	  this->newPartSchedule(LGFRCapacityFocussedShortageSchedule, deepCopy, toPopulate);
      cumSS->insertPartSchedule(dependentSched);
      partStrg->setOfParts(scenario->constSetOfParts(LGFR_CAPACITY_SET));
  }

  else if ( desc == LGFRTotalCommittedDemandVol) {
      LgFrCumDemandsPartScheduleStrategy* totCommDemandsSS
	  = new LgFrCumDemandsPartScheduleStrategy(*scenario);
      LgFrSortingDemandScheduleFloatSmartPointer dependentSched = 
	  this->newDemandSchedule(LGFRCommittedDemandVolume, deepCopy, toPopulate);
      totCommDemandsSS->insertDemandSchedule(dependentSched);
      partStrg = totCommDemandsSS;
      partStrg->setOfParts(scenario->constSetOfParts(LGFR_DEFAULT_SET));
  }

  else return LgFrScheduleFactory::newPartScheduleFromScratch(desc, deepCopy, toPopulate);

  LgFrSortingPartScheduleFloatSmartPointer spsf = createPartSched(*partStrg);
  if ( toPopulate ) 
      if ( this->scheduleType(desc,*partStrg) == INPUT )
	  spsf->populate();
      else {
	  LgFrLastProblemSolved& lps = this->lastProblemSolved();
	  if ( lps.imploded() ) spsf->populate();
      }
  delete partStrg;

  return spsf;
}
  
// Create a new sorting demand schedule
LgFrSortingDemandScheduleFloatSmartPointer
  LgFrScheduleFactoryForATP::newDemandScheduleFromScratch(const LgFrScheduleDescriptor& desc,
					       const RWBoolean deepCopy,
					       const RWBoolean toPopulate
					       )
     const
{
  LgFrDemandScheduleStrategy *demandStrg;
  LgFrScenarioForATPSmartPointer scenario = this->scenario();
  LgFrFilterStrategy * filterStrategyPtr = NULL;

  if ( desc == LGFRDefaultDemandVolume ) {
      demandStrg = new LgFrDemandVolumeScheduleStrategy(*scenario);
      demandStrg->setOfDemands(scenario->constSetOfDemands());
  }

  //---------------------------------------------------------------------------
  //   Committed Demands 
  //---------------------------------------------------------------------------
  else if ( desc == LGFRCommittedDemandVolume) {
      demandStrg = new LgFrDemandVolumeScheduleStrategyForATP(scenario,
							      LGFR_ATP_STATE_COMMITTED_IMPLODED);
      demandStrg->setOfDemands(
	  scenario->constSetOfDemands(LGFR_COMMITTED_DEMANDS));
  }

  else if ( desc == LGFRCommittedCumDemandVolume) {
      demandStrg = new LgFrCumTimeVecDemandScheduleStrategy(*scenario);
      LgFrCumTimeVecDemandScheduleStrategy * cumSS = 
	  (LgFrCumTimeVecDemandScheduleStrategy *) demandStrg;
      LgFrSortingDemandScheduleFloatSmartPointer dependentSched =
	  this->newDemandSchedule(LGFRCommittedDemandVolume, deepCopy, toPopulate);
      cumSS->insertDemandSchedule(dependentSched);
      demandStrg->setOfDemands(scenario->constSetOfDemands(LGFR_COMMITTED_DEMANDS));
  }


  else if ( desc == LGFRCommittedShipmentVolume) {
      demandStrg = new LgFrShipmentScheduleStrategyForATP(scenario,
							  LGFR_ATP_STATE_COMMITTED_IMPLODED);
      demandStrg->setOfDemands(
	  scenario->constSetOfDemands(LGFR_COMMITTED_DEMANDS));
  }

  else if ( desc == LGFRCommittedCumShipmentVolume) {
      demandStrg = new LgFrCumTimeVecDemandScheduleStrategy(*scenario);
      LgFrCumTimeVecDemandScheduleStrategy * cumSS = 
	  (LgFrCumTimeVecDemandScheduleStrategy *) demandStrg;
      LgFrSortingDemandScheduleFloatSmartPointer dependentSched =
	  this->newDemandSchedule(LGFRCommittedShipmentVolume, deepCopy, toPopulate);
      cumSS->insertDemandSchedule(dependentSched);
      demandStrg->setOfDemands(scenario->constSetOfDemands(LGFR_COMMITTED_DEMANDS));
  }

  else if ( desc == LGFRCommittedBacklog) {
      demandStrg = new LgFrBacklogScheduleStrategyForATP(scenario,
							 LGFR_ATP_STATE_COMMITTED_IMPLODED);
      demandStrg->setOfDemands(
	  scenario->constSetOfDemands(LGFR_COMMITTED_DEMANDS));
  }

  else if ( desc == LGFRCommittedNonZeroBacklog) {
      demandStrg = new LgFrBacklogScheduleStrategyForATP(scenario,
							 LGFR_ATP_STATE_COMMITTED_IMPLODED);
      demandStrg->setOfDemands(
	  scenario->constSetOfDemands(LGFR_COMMITTED_DEMANDS));
      filterStrategyPtr = new LgFrZeroTimeVecFilterStrategy();
  }

#if 0
//      WIBNI
      else if ( desc == LGFRCommittedPriority) {
else if ( desc == LGFRCommittedRevenueTimesDemandVolume) {
else if ( desc == LGFRCommittedCumRevenueTimesDemandVolume) {
else if ( desc == LGFRCommittedRevenueTimesShipmentVolume) {
else if ( desc == LGFRCommittedCumRevenueTimesShipmentVolume) {
#endif


//---------------------------------------------------------------------------
//   New Opp Demands 
//---------------------------------------------------------------------------
  else if ( desc == LGFRNewOpportunityDemandVolume) {
      demandStrg = new LgFrDemandVolumeScheduleStrategyForATP(scenario,
							      LGFR_ATP_STATE_NEWOPP_IMPLODED);
      demandStrg->setOfDemands(scenario->constSetOfDemands(LGFR_NEWOPPOR_DEMANDS));
  }

  else if ( desc == LGFRNewOpportunityCumDemandVolume) {
      demandStrg = new LgFrCumTimeVecDemandScheduleStrategy(*scenario);
      LgFrCumTimeVecDemandScheduleStrategy * cumSS = 
	  (LgFrCumTimeVecDemandScheduleStrategy *) demandStrg;
      LgFrSortingDemandScheduleFloatSmartPointer dependentSched =
	  this->newDemandSchedule(LGFRNewOpportunityDemandVolume, deepCopy, toPopulate);
      cumSS->insertDemandSchedule(dependentSched);
      demandStrg->setOfDemands(scenario->constSetOfDemands(LGFR_NEWOPPOR_DEMANDS));
  }

  else if ( desc == LGFRNewOpportunityShipmentVolume) {
      demandStrg = new LgFrShipmentScheduleStrategyForATP(scenario,
							  LGFR_ATP_STATE_NEWOPP_IMPLODED);
      demandStrg->setOfDemands(scenario->constSetOfDemands(LGFR_NEWOPPOR_DEMANDS));
  }

  else if ( desc == LGFRNewOpportunityCumShipmentVolume) {
      demandStrg = new LgFrCumTimeVecDemandScheduleStrategy(*scenario);
      LgFrCumTimeVecDemandScheduleStrategy * cumSS = 
	  (LgFrCumTimeVecDemandScheduleStrategy *) demandStrg;
      LgFrSortingDemandScheduleFloatSmartPointer dependentSched =
	  this->newDemandSchedule(LGFRNewOpportunityShipmentVolume, deepCopy, toPopulate);
      cumSS->insertDemandSchedule(dependentSched);
      demandStrg->setOfDemands(scenario->constSetOfDemands(LGFR_NEWOPPOR_DEMANDS));
  }
  else if ( desc == LGFRNewOpportunityBacklog) {
      demandStrg = new LgFrBacklogScheduleStrategyForATP(scenario,
							 LGFR_ATP_STATE_NEWOPP_IMPLODED);
      demandStrg->setOfDemands(scenario->constSetOfDemands(LGFR_NEWOPPOR_DEMANDS));
  }

#if 0
//  WIBNI
else if ( desc == LGFRNewOpportunityPriority) {
#endif


//---------------------------------------------------------------------------
//   Firm Forecast Demands 
//---------------------------------------------------------------------------
  else if ( desc == LGFRFirmForecastDemandVolume) {
      demandStrg = new LgFrDemandVolumeScheduleStrategyForATP(scenario,
							      LGFR_ATP_STATE_ATP_IMPLODED);
      demandStrg->setOfDemands(
	  scenario->constSetOfDemands(LGFR_FIRMFORECAST_DEMANDS));
  }

  else if ( desc == LGFRFirmForecastCumDemandVolume) {
      demandStrg = new LgFrCumTimeVecDemandScheduleStrategy(*scenario);
      LgFrCumTimeVecDemandScheduleStrategy * cumSS = 
	  (LgFrCumTimeVecDemandScheduleStrategy *) demandStrg;
      LgFrSortingDemandScheduleFloatSmartPointer dependentSched =
	  this->newDemandSchedule(LGFRFirmForecastDemandVolume, deepCopy, toPopulate);
      cumSS->insertDemandSchedule(dependentSched);
      demandStrg->setOfDemands(scenario->constSetOfDemands(LGFR_FIRMFORECAST_DEMANDS));
  }

  else if ( desc == LGFRFirmForecastShipmentVolume) {
      demandStrg = new LgFrShipmentScheduleStrategyForATP(scenario,
							  LGFR_ATP_STATE_ATP_IMPLODED);
      demandStrg->setOfDemands(scenario->constSetOfDemands(LGFR_FIRMFORECAST_DEMANDS));
  }

  else if ( desc == LGFRFirmForecastCumShipmentVolume) {
      demandStrg = new LgFrCumTimeVecDemandScheduleStrategy(*scenario);
      LgFrCumTimeVecDemandScheduleStrategy * cumSS = 
	  (LgFrCumTimeVecDemandScheduleStrategy *) demandStrg;
      LgFrSortingDemandScheduleFloatSmartPointer dependentSched =
	  this->newDemandSchedule(LGFRFirmForecastShipmentVolume, deepCopy, toPopulate);
      cumSS->insertDemandSchedule(dependentSched);
      demandStrg->setOfDemands(scenario->constSetOfDemands(LGFR_FIRMFORECAST_DEMANDS));
  }
  else if ( desc == LGFRFirmForecastBacklog) {
      demandStrg = new LgFrBacklogScheduleStrategyForATP(scenario,
							 LGFR_ATP_STATE_ATP_IMPLODED);
      demandStrg->setOfDemands(scenario->constSetOfDemands(LGFR_FIRMFORECAST_DEMANDS));
  }




#ifdef USING_RISK_FORECAST
  else if ( desc == LGFRRiskForecastDemandVolume ) {
      demandStrg = new LgFrDemandVolumeScheduleStrategyForATP(scenario);
      demandStrg->setOfDemands(scenario->constSetOfDemands(LGFR_RISKFORECAST_DEMANDS));
  }
#endif
  else 
    return LgFrScheduleFactory::newDemandScheduleFromScratch(desc, deepCopy, 
							     toPopulate);

  LgFrSortingDemandScheduleFloatSmartPointer sdsf
    = createDemandSched(*demandStrg, filterStrategyPtr);
  if ( toPopulate )
      if ( this->scheduleType(desc,*demandStrg) == INPUT )
	  sdsf->populate();
      else {
	  LgFrLastProblemSolved& lps = this->lastProblemSolved();
	  if ( lps.imploded() ) sdsf->populate();
      }
  delete demandStrg;
  delete filterStrategyPtr;

  return sdsf;
}

// ---------------------------------------------------------------------
// Return the schedule type: INPUT or OUTPUT.
// ---------------------------------------------------------------------
schdType
LgFrScheduleFactoryForATP::scheduleType(
    const LgFrScheduleDescriptor& desc,
    const LgFrAttributeScheduleStrategy & ass)
     const
{
  if ( desc == LGFRSupplyVolume ) return INPUT; // Doing it this way (instead
  if ( desc == LGFRDemandVolume ) return INPUT; // of using || allows
                                                // inlining

  if ( desc == LGFRCommittedDemandVolume ) return INPUT;
  if ( desc == LGFRCommittedCumDemandVolume ) return INPUT;

  if ( desc == LGFRFirmForecastDemandVolume ) return INPUT;
  if ( desc == LGFRFirmForecastCumDemandVolume ) return INPUT;

  if ( desc == LGFRNewOpportunityDemandVolume ) return INPUT;
  if ( desc == LGFRNewOpportunityCumDemandVolume ) return INPUT;

  if ( desc == LGFRMaterialSupplyVolume ) return INPUT;
  if ( desc == LGFRMaterialCumSupplyVolume ) return INPUT;

  if ( desc == LGFRCapacitySupplyVolume ) return INPUT;
  
  if ( desc == LGFRTotalCommittedDemandVol ) return INPUT;  

  return this->LgFrScheduleFactory::scheduleType(desc,ass);
}

// Constructor
LgFrScheduleFactoryForATP::LgFrScheduleFactoryForATP (LgFrScenarioSmartPointer s)
:   LgFrScheduleFactory(s)
{
  // Nothing to do in here
}

// Clone method
// Create a copy of the *this on the heap and return a pointer to it
// Even thought this returns a LgFrScheduleFactory*, the pointer
// must point to a LgFrScheduleFactoryForATP.
LgFrScheduleFactory*
LgFrScheduleFactoryForATP::clone (LgFrScenarioSmartPointer newScenario)
     const
{
  return new LgFrScheduleFactoryForATP (newScenario);
}

// Destructor
LgFrScheduleFactoryForATP::~LgFrScheduleFactoryForATP()
{
  // Nothing to do in here
}


void
LgFrScheduleFactoryForATP::initHashTable ()
{
}


#ifdef NDEBUG
#undef NDEBUG
#endif

// test this class
void LgFrScheduleFactoryForATP::test()
{
}


void
LgFrScheduleFactoryForATP::contextTest(const LgFrScheduleFactory & f)
{
  // Test abbreviation function
  assert (f.abbreviationOf (LGFRSupplyVolume)
          != LGFRScrapVolume.abbrev()            );
  assert (f.abbreviationOf (LGFRSupplyVolume)
          == LGFRSupplyVolume.abbrev()           );
  assert (f.abbreviationOf (LGFRDemandVolume)
          == LGFRDemandVolume.abbrev()           );  
  assert (f.abbreviationOf (LGFRNewOpportunityDemandVolume)
          == LGFRNewOpportunityDemandVolume.abbrev()               );
}
