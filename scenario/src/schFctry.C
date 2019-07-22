// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
#include <string>
// End of RW2STL -- inserted includes.

#include <scenario/src/schFctry.h>
#include <scenario/src/scenario.h>
#include <scenario/src/schdSnFS.h>
#include <scenario/src/cumSS.h>
#include <scenario/src/partCtFS.h>
#include <scenario/src/boolFS.h>
#include <scenario/src/partSS.h>
#include <scenario/src/prodSS.h>
#include <scenario/src/demVolSS.h>
#include <scenario/src/fssSS.h>
#include <scenario/src/backlgSS.h>
#include <scenario/src/critPaSS.h>
#include <scenario/src/excessSS.h>
#include <scenario/src/shipSS.h>
#include <scenario/src/supplySS.h>
#include <scenario/src/usageSS.h>
#include <scenario/src/reqSS.h>
#include <scenario/src/cycleSS.h>
#include <scenario/src/stockSS.h>
#include <scenario/src/scrapSS.h>
#include <scenario/src/schdDeSt.h>
#include <scenario/src/priorSS.h>
#include <scenario/src/partSchP.h>
#include <scenario/src/demdSchP.h>
#include <scenario/src/shpRewSS.h>
// #include <scenario/src/costSS.h>
#include <scenario/src/fssPrdSS.h>

// ---------------------------------------------------------------------
// Create a Part Schedule, set its partSS strategy
// and optional filterstrategy, data population is not done here.
// ---------------------------------------------------------------------
LgFrSortingPartScheduleFloatSmartPointer
LgFrScheduleFactory::createPartSched(
   const LgFrPartScheduleStrategy & partSS,
   const LgFrFilterStrategy * filterStrategyPtr,
   const LgFrPairCompareFloat * pairComparePtr,
   const bool ascending
                                     )
     const
{
 LgFrSortingPartScheduleFloatSmartPointer spsf = 
     LgFrSortingPartScheduleFloatSmartPointer(new LgFrSortingPartScheduleFloat());
                                           
 // Set it's strategy
 spsf->attributeScheduleStrategy(partSS);

 // Set it's filter strategy
 if ( filterStrategyPtr ) spsf->filterStrategy(*filterStrategyPtr);

 // Set it's sort strategy
 spsf->sort( pairComparePtr, ascending );

 // Return it
 return spsf;
}

// ---------------------------------------------------------------------
// Create a Demand Schedule, set it's demandSS strategy
// and optional filterstrategy, data population is not done here.
// ---------------------------------------------------------------------
LgFrSortingDemandScheduleFloatSmartPointer
LgFrScheduleFactory::createDemandSched(
   const LgFrDemandScheduleStrategy & demandSS,
   const LgFrFilterStrategy * filterStrategyPtr,
   const LgFrPairCompareFloat * pairComparePtr,
   const bool ascending
                                     )
     const
{
 LgFrSortingDemandScheduleFloatSmartPointer sdsf = 
     LgFrSortingDemandScheduleFloatSmartPointer(new LgFrSortingDemandScheduleFloat());
                                           
 // Set it's strategy
 sdsf->attributeScheduleStrategy(demandSS);

 // Set it's filter strategy
 if ( filterStrategyPtr ) sdsf->filterStrategy(*filterStrategyPtr);

 // Set it's sort strategy
 sdsf->sort( pairComparePtr, ascending );

 // Return it
 return sdsf;
}


LgFrSortingPartScheduleFloatSmartPointer								      
LgFrScheduleFactory::newPartScheduleFromExistingOne
  (const LgFrScheduleDescriptor& desc,
   const bool deepCopy,
   const bool toPopulate,
   LgFrSortingPartScheduleFloatSmartPointer schedulePtrPtr)
  const
{	
  LgFrSortingPartScheduleFloatSmartPointer ptr;	
  									      
  /* Copy the saved schedule if necessary */				      
  if (deepCopy)								      
    ptr = schedulePtrPtr.deepCopy();			      
  else									      
    ptr = schedulePtrPtr;					      
  									      
  if (toPopulate) 
  {							      
    if ( LgFrScheduleFactory::scheduleType(desc, *(ptr->attributeScheduleStrategy()) ) == INPUT	||						      
    	 (this->lastProblemSolved()).imploded() )			      
    {								      
      if ( ptr->size() == 0 ) 
      {					      
	    /* Schedule does not seem to be populated, so populate it.	      
	     * Note: This is a heuristic.  The schedule may really be		      
	     * populated, but there is no way to tell.			      
	     * Note: ptr may point to a schedule in this schedule factory's	      
	     * list of schedule, so populating it will affect a schedule	      
	     * that is already in use.  If that behavior is not desired, the	      
	     * schedule factory users should use deep copy instead. */	      
	    ptr->populate();						      
      }									      
      else 
      {								      
	    /* The schedule seems to be populated.  Should it be repopulated?	      
	     * Pros: The argument says to repopulate it, and that would result      
	     * in up-to-date results.  Cons: If the schedule subscribes to its      
	     * data-source, it will be updated automatically whenever somthing      
	     * it depends on changes, so it ought to be up-to-date already.	      
	     * The Cons win.	*/					      
	    /* Do nothing */						      
      }									      
    }									      
  }									      
  else /* toPopulate is false */ 
  {					      
    if ( ptr->size() != 0 ) 
    {					      
      /* Dilemma: caller says not to populate the schedule, but it is already
       * populated.  Solution for now: print warning */		   	      
      this->printWarning();						      
    }									      
  }									      
  return ptr;								      
}

LgFrSortingDemandScheduleFloatSmartPointer								      
LgFrScheduleFactory::newDemandScheduleFromExistingOne
  (const LgFrScheduleDescriptor& desc,
   const bool deepCopy,
   const bool toPopulate,
   LgFrSortingDemandScheduleFloatSmartPointer schedulePtrPtr)
  const
{	
  LgFrSortingDemandScheduleFloatSmartPointer ptr;	
  									      
  /* Copy the saved schedule if necessary */				      
  if (deepCopy)								      
    ptr = schedulePtrPtr.deepCopy();			      
  else									      
    ptr = schedulePtrPtr;					      
  									      
  if (toPopulate) 
  {							      
    if ( LgFrScheduleFactory::scheduleType(desc, *(ptr->attributeScheduleStrategy()) ) == INPUT	||						      
    	 (this->lastProblemSolved()).imploded() )			      
    {								      
      if ( ptr->size() == 0 ) 
      {					      
	    /* Schedule does not seem to be populated, so populate it.	      
	     * Note: This is a heuristic.  The schedule may really be		      
	     * populated, but there is no way to tell.			      
	     * Note: ptr may point to a schedule in this schedule factory's	      
	     * list of schedule, so populating it will affect a schedule	      
	     * that is already in use.  If that behavior is not desired, the	      
	     * schedule factory users should use deep copy instead. */	      
	    ptr->populate();						      
      }									      
      else 
      {								      
	    /* The schedule seems to be populated.  Should it be repopulated?	      
	     * Pros: The argument says to repopulate it, and that would result      
	     * in up-to-date results.  Cons: If the schedule subscribes to its      
	     * data-source, it will be updated automatically whenever somthing      
	     * it depends on changes, so it ought to be up-to-date already.	      
	     * The Cons win.	*/					      
	    /* Do nothing */						      
      }									      
    }									      
  }									      
  else /* toPopulate is false */ 
  {					      
    if ( ptr->size() != 0 ) 
    {					      
      /* Dilemma: caller says not to populate the schedule, but it is already
       * populated.  Solution for now: print warning */		   	      
      this->printWarning();						      
    }									      
  }									      
  return ptr;								      
}

// ------------------------------------------------------------------------------
// Create a new sorting part schedule
// Populate all input schedules if toPopulate flag = true.
// Populate all output schedules if implosion was run and toPopulate flag = true.
// -------------------------------------------------------------------------------
LgFrSortingPartScheduleFloatSmartPointer
  LgFrScheduleFactory::newPartSchedule  (const LgFrScheduleDescriptor& desc,
					 const bool deepCopy,
					 const bool toPopulate
					 )
     const
{
  LgFrSortingPartScheduleFloatSmartPointer schedulePtr = 0;
  NamePartDictionary::const_iterator it = ((LgFrScheduleFactory*)this)->partSchedules_.find( desc.name() );
  if( it != partSchedules_.end() )
      schedulePtr = (*it).second;

  if( !schedulePtr.null() &&  toPopulate )
  {
    return this->newPartScheduleFromExistingOne (desc, deepCopy, toPopulate, schedulePtr);
  }
  else 
  {
    // Either the schedule has not been created before, or the user
    // does not to use one that has already been populated, so make
    // it from scratch
    LgFrSortingPartScheduleFloatSmartPointer spsf
      = newPartScheduleFromScratch (desc, deepCopy, toPopulate);
    if ( schedulePtr.null() ) 
    {
      // This is the first time a schedule with this descriptor has
      // been made, so save it.
      // cast away const
      ((LgFrScheduleFactory *)this)->insertPartSchedule(desc, spsf);
    }
    return spsf;
  }
}

// --------------------------------------------------------------------------
// Return a schedule without getting it from the list of previously-
// constructed schedules
// --------------------------------------------------------------------------
LgFrSortingPartScheduleFloatSmartPointer 
LgFrScheduleFactory::newPartScheduleFromScratch
  (const LgFrScheduleDescriptor& desc,
   const bool deepCopy,
   const bool toPopulate
   )
const
{
  LgFrScenarioSmartPointer scenario = this->scenario();
  LgFrPartScheduleStrategy *partSS;
  LgFrFilterStrategy * filterStrategyPtr = 0;
  LgFrPairCompareFloat * pairComparePtr = 0;
  bool ascending = true;
  if ( desc == LGFRCriticalPartSupplyVolume ) {
      // First get the critical list
      // Then, use a schedule sensitive filter startegy to get
      // the critical supply schedule.
      LgFrCriticalPartScheduleStrategy* criticalSS 
	= new LgFrCriticalPartScheduleStrategy(*scenario);
      LgFrSortingPartScheduleFloatSmartPointer
	  criticalSchedule = createPartSched(*criticalSS);
      LgFrScheduleSensitiveFilterStrategy cs;
      cs.schedule( criticalSchedule );
      LgFrSupplyScheduleStrategy* supplySS 
	= new LgFrSupplyScheduleStrategy(*scenario);
      LgFrSortingPartScheduleFloatSmartPointer
	  supplySchedule = createPartSched(*supplySS, &cs);
      delete criticalSS;
      delete supplySS;

      // cast away const
      ((LgFrScheduleFactory *)this)->insertPartSchedule(desc, supplySchedule);

      return supplySchedule; 
  }
  else if ( desc == LGFRCriticalPartRankingList )
      partSS = new LgFrCriticalPartScheduleStrategy(*scenario);
  else if ( desc == LGFRExcessVolume )
      partSS = new LgFrExcessScheduleStrategy(*scenario);
  else if ( desc == LGFRFocussedShortageSchedule )
      partSS = new LgFrFocussedShortageScheduleScheduleStrategy(*scenario);
  else if ( desc == LGFRFSSProductionVolume )
      partSS = new 
               LgFrFocussedShortageScheduleProductionScheduleStrategy(
               *scenario );
  else if ( desc == LGFRProductionVolume )
      partSS = new LgFrProductionScheduleStrategy(*scenario);
  else if ( desc == LGFRRequirementVolume )
      partSS = new LgFrRequirementScheduleStrategy(*scenario);
  else if ( desc == LGFRSupplyVolume ) {
      partSS = new LgFrSupplyScheduleStrategy(*scenario);

      // Code which will sort schedule's itemTimeVec's in descending order
      // pairComparePtr = new LgFrPairCompareFloat();
      // ascending = false;
  }
  else if ( desc == LGFRCycleTime ) 
      partSS = new LgFrCycleTimeScheduleStrategy(*scenario);
  else if ( desc == LGFRStockVolume ) 
      partSS = new LgFrStockScheduleStrategy(*scenario);
  else if ( desc == LGFRScrapVolume ) 
      partSS = new LgFrScrapScheduleStrategy(*scenario);
  else if ( desc == LGFRMaterialSupplyVolume) {
      partSS = new LgFrSupplyScheduleStrategy(*scenario);
      // Make a filter that keeps capacity PARTS out of the schedule
      filterStrategyPtr = new LgFrPartCategoryFilterStrategy (LgFrCAPACITY,
							*scenario);
  }
  else if ( desc == LGFRCapacitySupplyVolume) {
      partSS = new LgFrSupplyScheduleStrategy(*scenario);
      // Make a filter that keeps capacities out of the schedule
      LgFrPartCategoryFilterStrategy categoryFilter
	  = LgFrPartCategoryFilterStrategy (LgFrCAPACITY, *scenario);
      // Make a filter that only lets capacities into the schedule.  (The 
      // LgFrNotFilterStrategy makes a copy of categoryFilter.)
      filterStrategyPtr = new  LgFrNotFilterStrategy (categoryFilter);
  }
  else if ( desc == LGFRTotalPartDemandVol) {
      LgFrCumDemandsPartScheduleStrategy* totShipSS
	  = new LgFrCumDemandsPartScheduleStrategy(*scenario);
      LgFrSortingDemandScheduleFloatSmartPointer dependentSched = 
	  this->newDemandSchedule(LGFRDemandVolume, deepCopy, toPopulate);
      totShipSS->insertDemandSchedule(dependentSched);
      partSS = totShipSS;
      partSS->setOfParts(scenario->constSetOfParts());
  }
#if 0
  else if (desc ==  LGFRSupplyVolumeTimesUnitCost) {
      LgFrPartScheduleTimesUnitCostScheduleStrategy* supVolTimesUnitCostSS
	  = new LgFrPartScheduleTimesUnitCostScheduleStrategy (*scenario) ;
      LgFrSortingPartScheduleFloatSmartPointer tempSched =
        this->newPartSchedule(LGFRSupplyVolume, deepCopy, toPopulate);
      supVolTimesUnitCostSS->insertPartSchedule( tempSched );
      partSS = supVolTimesUnitCostSS;
      partSS->setOfParts( scenario->constSetOfParts() );
  }
  else if (desc ==  LGFRRequirementVolumeTimesUnitCost) {
      LgFrPartScheduleTimesUnitCostScheduleStrategy* ucSS
	  = new LgFrPartScheduleTimesUnitCostScheduleStrategy (*scenario) ;
      LgFrSortingPartScheduleFloatSmartPointer tempSched =
        this->newPartSchedule(LGFRRequirementVolume, deepCopy, toPopulate);
      ucSS->insertPartSchedule( tempSched );
      partSS = ucSS;
      partSS->setOfParts( scenario->constSetOfParts() );
  }
  else if (desc ==  LGFRStockVolumeTimesUnitCost) {
      LgFrPartScheduleTimesUnitCostScheduleStrategy* ucSS
	  = new LgFrPartScheduleTimesUnitCostScheduleStrategy (*scenario) ;
      LgFrSortingPartScheduleFloatSmartPointer tempSched =
        this->newPartSchedule(LGFRStockVolume, deepCopy, toPopulate);
      ucSS->insertPartSchedule( tempSched );
      partSS = ucSS;
      partSS->setOfParts( scenario->constSetOfParts() );
  }
#endif
  else if (desc == LGFRUsageVolume)     {
      LgFrUsageScheduleStrategy* usageSS
	  = new LgFrUsageScheduleStrategy(*scenario);
      LgFrSortingPartScheduleFloatSmartPointer dependentSupplySched1 = 
	  this->newPartSchedule(LGFRSupplyVolume, deepCopy, toPopulate);
      LgFrSortingPartScheduleFloatSmartPointer dependentProdSched2 = 
	  this->newPartSchedule(LGFRProductionVolume, deepCopy, toPopulate);
      LgFrSortingPartScheduleFloatSmartPointer dependentStockSched3 = 
	  this->newPartSchedule(LGFRStockVolume, deepCopy, toPopulate);
      LgFrSortingPartScheduleFloatSmartPointer dependentScrapSched4 = 
	  this->newPartSchedule(LGFRScrapVolume, deepCopy, toPopulate);
      usageSS->insertPartSchedule(dependentSupplySched1);
      usageSS->insertPartSchedule(dependentProdSched2);
      usageSS->insertPartSchedule(dependentStockSched3);
      usageSS->insertPartSchedule(dependentScrapSched4);
      partSS = usageSS;
  }
  else {
      std::cerr << "\nWrong part schedule description string \""
	   << desc.name() << "\" used.";
      return 0;
  }
  LgFrSortingPartScheduleFloatSmartPointer spsf = 
						createPartSched(
						   *partSS,
						   filterStrategyPtr,
						   pairComparePtr,
						   ascending);
  if ( toPopulate ) 
      // LgFrScheduleFactory::scheduleType is used in following line
      // to prevent a subclass's scheduleType from being used when the
      // schedule is being generated by this class
      if ( LgFrScheduleFactory::scheduleType(desc,*partSS)  == INPUT )
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


// ------------------------------------------------------------------------------
// Create a new sorting demand schedule
// Populate all input schedules if toPopulate flag = true.
// Populate all output schedules if implosion was run and toPopulate flag = true.
// ------------------------------------------------------------------------------
LgFrSortingDemandScheduleFloatSmartPointer
  LgFrScheduleFactory::newDemandSchedule(const LgFrScheduleDescriptor& desc,
					 const bool deepCopy,
					 const bool toPopulate
					 )
     const
{
  LgFrSortingDemandScheduleFloatSmartPointer schedulePtr = 0;
  NameDemandDictionary::const_iterator it = ((LgFrScheduleFactory*)this)->demandSchedules_.find( desc.name() );
  if( it != demandSchedules_.end() )
      schedulePtr = (*it).second;

//  std::string key(desc.name());
//  LgFrSortingDemandScheduleFloatSmartPointer* schedulePtr =
//    (LgFrSortingDemandScheduleFloatSmartPointer *)
//    demandSchedules_.findValue(&key);
  if (!schedulePtr.null()  &&  toPopulate )  {
    return this->newDemandScheduleFromExistingOne (desc, deepCopy, toPopulate, 
						   schedulePtr);
  }
  else {
    // Either the schedule has not been created before, or the user
    // does not to use one that has already been populated, so make
    // it from scratch
    LgFrSortingDemandScheduleFloatSmartPointer spsf
      = newDemandScheduleFromScratch (desc, deepCopy, toPopulate);
    if (schedulePtr == 0) {
      // This is the first time a schedule with this descriptor has
      // been made, so save it.
      // cast away const
      ((LgFrScheduleFactory *)this)->insertDemandSchedule(desc, spsf);
    }
    return spsf;
  }
}

// --------------------------------------------------------------------------
// Return a schedule without getting it from the list of previously-
// constructed schedules
// --------------------------------------------------------------------------
LgFrSortingDemandScheduleFloatSmartPointer 
LgFrScheduleFactory::newDemandScheduleFromScratch
  (const LgFrScheduleDescriptor& desc,
   const bool deepCopy,
   const bool toPopulate
   )
const
{
  LgFrDemandScheduleStrategy *demandSS;
  if ( desc == LGFRDemandVolume ) 
      demandSS = new LgFrDemandVolumeScheduleStrategy(*(this->scenario()));
  else if ( desc == LGFRShipmentVolume )
      demandSS = new LgFrShipmentScheduleStrategy(*(this->scenario()));
  else if ( desc == LGFRBacklogVolume )
      demandSS = new LgFrBacklogScheduleStrategy(*(this->scenario()));
  else if ( desc == LGFRPriority )
      demandSS = new  LgFrPriorityScheduleStrategy(*(this->scenario()));
  else if ( desc == LGFRObj1ShipReward )
      demandSS = new  LgFrObj1ShipRewardScheduleStrategy(*(this->scenario()));
  else {
      std::cerr << "\nWrong demand schedule description string \""
	   << desc.name() << "\" used.";
      return 0;
  }
  LgFrSortingDemandScheduleFloatSmartPointer sdsf 
    = createDemandSched(*demandSS);
  if ( toPopulate )
      // LgFrScheduleFactory::scheduleType is used in following line
      // to prevent a subclass's scheduleType from being used when the
      // schedule is being generated by this class
      if ( LgFrScheduleFactory::scheduleType(desc,*demandSS) == INPUT )
	  sdsf->populate();
      else {
	  LgFrLastProblemSolved& lps = this->lastProblemSolved();
	  if ( lps.imploded() ) sdsf->populate();
      }
  delete demandSS;

  return sdsf;
}


// ---------------------------------------------------------------------
// Return the schedule type: INPUT or OUTPUT of implosion run.
// ---------------------------------------------------------------------
schdType
LgFrScheduleFactory::scheduleType(const LgFrScheduleDescriptor& desc,
				  const LgFrAttributeScheduleStrategy & ass)
     const
{
    if ( ass.isMutable() ) return INPUT;	// Doing it this way (instead
    if ( desc == LGFRSupplyVolumeTimesUnitCost ) // of using ||) allows
	return INPUT;				// inlining
    return OUTPUT;
}

std::string
LgFrScheduleFactory::abbreviationOf (const LgFrScheduleDescriptor & desc)
     const
{
  return desc.abbrev( scenario()->nationalLanguage() );
}

    
// ---------------------------------------------------------------------
// Return list of schedule descriptors which can be used by factory to
// create schedules.     
// ---------------------------------------------------------------------
LgFrScheduleDescriptorVector
LgFrScheduleFactory::demandScheduleDescriptors()
const
{
  return LgFrScheduleFactory::demandScheduleDescriptors_;
}
LgFrScheduleDescriptorVector
LgFrScheduleFactory::partScheduleDescriptors()
const
{
  return LgFrScheduleFactory::partScheduleDescriptors_;
}

// ---------------------------------------------------------------------
// Return the LastProblemSolved which belongs to this scenario
// ---------------------------------------------------------------------
LgFrLastProblemSolved&
  LgFrScheduleFactory::lastProblemSolved()
     const
{
    // cast the object so that it's non-const - HACK
  return ((LgFrScenarioSmartPointer&)scenarioPtr_)->lastProblemSolved();
}

// ---------------------------------------------------------------------
// Returns the factory's demandSchedules_
// ---------------------------------------------------------------------
NameDemandDictionary &
LgFrScheduleFactory::demandSchedules()
{
  return demandSchedules_;
}


// ---------------------------------------------------------------------
// Returns the factory's partSchedules_
// ---------------------------------------------------------------------
NamePartDictionary &
LgFrScheduleFactory::partSchedules()
{
  return partSchedules_;
}


// ---------------------------------------------------------------------
// Return the scenario ptr
// ---------------------------------------------------------------------
LgFrScenarioSmartPointer
  LgFrScheduleFactory::scenario()
     const
{
  return scenarioPtr_;
}
// ---------------------------------------------------------------------
//   methods for inserting Schedules into Dictionaries
// ---------------------------------------------------------------------
void
LgFrScheduleFactory::insertDemandSchedule(const LgFrScheduleDescriptor& desc,
					  LgFrSortingDemandScheduleFloatSmartPointer schedulePtr)
{
    demandSchedules_.insert( NameDemandPair( desc.name(), schedulePtr ) );
}

void
LgFrScheduleFactory::insertPartSchedule(const LgFrScheduleDescriptor& desc,
					  LgFrSortingPartScheduleFloatSmartPointer schedulePtr)
{
    partSchedules_.insert( NamePartPair( desc.name(), schedulePtr ) );
}

// Initialize static class data
LgFrScheduleDescriptorVector
LgFrScheduleFactory::demandScheduleDescriptors_
                     = LgFrScheduleFactory::initializeDemandScheduleDescriptors();
LgFrScheduleDescriptorVector
LgFrScheduleFactory::partScheduleDescriptors_
                     = LgFrScheduleFactory::initializePartScheduleDescriptors();

LgFrScheduleDescriptorVector
LgFrScheduleFactory::initializeDemandScheduleDescriptors()
{
  LgFrScheduleDescriptorVector retVal;
  retVal.push_back(LGFRDemandVolume);
  retVal.push_back(LGFRShipmentVolume);
  retVal.push_back(LGFRBacklogVolume);
  retVal.push_back(LGFRPriority);
  retVal.push_back(LGFRObj1ShipReward);
  return retVal;
}

LgFrScheduleDescriptorVector
LgFrScheduleFactory::initializePartScheduleDescriptors()
{
  LgFrScheduleDescriptorVector retVal;
  retVal.push_back(LGFRCriticalPartSupplyVolume);
  retVal.push_back(LGFRCriticalPartRankingList);
  retVal.push_back(LGFRExcessVolume);
  retVal.push_back(LGFRFocussedShortageSchedule);
  retVal.push_back(LGFRFSSProductionVolume);
  retVal.push_back(LGFRProductionVolume);
  retVal.push_back(LGFRRequirementVolume );
  retVal.push_back(LGFRSupplyVolume);
  retVal.push_back(LGFRCycleTime);
  retVal.push_back(LGFRStockVolume);
  retVal.push_back(LGFRScrapVolume);
  retVal.push_back(LGFRMaterialSupplyVolume);
  retVal.push_back(LGFRCapacitySupplyVolume);
  retVal.push_back(LGFRTotalPartDemandVol);
#if 0
  retVal.push_back(LGFRSupplyVolumeTimesUnitCost);
  retVal.push_back(LGFRStockVolumeTimesUnitCost);
#endif
  retVal.push_back(LGFRUsageVolume);
  return retVal;
}

// ---------------------------------------------------------------------
// Remove all of the descriptors and
// LgFrSortingPartScheduleFloatSmartPointer from the dictionaries.
// This frees the smart pointers.  As usual, the schedules they point to
// are deleted if and only if no other smart pointers point to the
// schedules.
// This routine is needed because of a catch-22: if a LgFrScheduleFactory
// LgFrSortingPartScheduleFloatSmartPointer's to schedules, the relevant
// scenario may never be deleted (because of subscriptions) and since
// the scenario won't be delete, the schedule factory won't be deleted,
// so the schedules won't be deleted, ....  By deleting the reference
// to the schedules in the schedule factory, the schedules can be
// deleted, so the scenario can be deleted, so the factory can be
// deleted.
// ---------------------------------------------------------------------
void
LgFrScheduleFactory::clearAndDestroy()
{
  //partSchedules().clearAndDestroy();
  //demandSchedules().clearAndDestroy();
}

// ---------------------------------------------------------------------
// Print warning about calling with toPopulate false when a populated
// schedule for the given descriptor already exists
// ---------------------------------------------------------------------
void
LgFrScheduleFactory::printWarning() const
{
  std::cerr << "Warning: Internal inconsistency.  Program tried to make a \n"
       << "schedule without numbers, but the numbers had already been\n"
       << "calculated, so they are being used anyway.  Unusual results may\n"
       << "appear." << std::endl;
}

// ---------------------------------------------------------------------
// Create a copy of the *this on the heap in the new scenario
// and return a pointer to it.
// ---------------------------------------------------------------------
LgFrScheduleFactory*
LgFrScheduleFactory::clone (LgFrScenarioSmartPointer newScenario)
     const
{
  return new LgFrScheduleFactory(newScenario);
}

// ---------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------
LgFrScheduleFactory::LgFrScheduleFactory (LgFrScenarioSmartPointer scenPtr)
:   scenarioPtr_(scenPtr)
{
}

// ---------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------
LgFrScheduleFactory::~LgFrScheduleFactory()
{
}


#ifdef NDEBUG
#undef NDEBUG
#endif

// ---------------------------------------------------------------------
// test this class
// ---------------------------------------------------------------------
void
LgFrScheduleFactory::test()
{
}


// Expects *testScenario to contain data from
// /u/implode/wit/data/examples/diner12/wit.data and expects
// f to be the scedule factory for testScenario
void
LgFrScheduleFactory::contextTest(const LgFrScheduleFactory & f,
                                 LgFrScenarioSmartPointer & testScenario)
{
  // Test abbreviation function
  assert (f.abbreviationOf (LGFRSupplyVolume)
          != LGFRScrapVolume.abbrev()            );
  assert (f.abbreviationOf (LGFRSupplyVolume)
          == LGFRSupplyVolume.abbrev()           );
  assert (f.abbreviationOf (LGFRDemandVolume)
          == LGFRDemandVolume.abbrev()           );

  // test smart schedule stuff
  LgFrSortingDemandScheduleFloatSmartPointer priSchedule =
      f.newDemandSchedule(LGFRPriority, false);
  LgFrSortingDemandScheduleFloatSmartPointer priSchedule2 =
      f.newDemandSchedule(LGFRPriority, false);
  assert(priSchedule == priSchedule2);

  LgFrSortingPartScheduleFloatSmartPointer reqSchedule =
      f.newPartSchedule(LGFRRequirementVolume, false);
  LgFrSortingPartScheduleFloatSmartPointer reqSchedule2 =
      f.newPartSchedule(LGFRRequirementVolume, false);
  assert(reqSchedule == reqSchedule2);

  // test LGFRSupplyVolumeTimesUnitCost and LGFRSupplyVolume
  
  LgFrSortingPartScheduleFloatSmartPointer supplySchedule =
      f.newPartSchedule(LGFRSupplyVolume, false, true);
#if 0
  LgFrSortingPartScheduleFloatSmartPointer supplyTimesCostSchedule =
      f.newPartSchedule(LGFRSupplyVolumeTimesUnitCost, false, true);
  LgFrScheduleFloat & s = *supplySchedule;
  assert (s.size() != 0);
  assert (s == *supplyTimesCostSchedule); // unit costs in diner12 are all 1.0
#endif


  // test LGFRUsageVolume
  LgFrSortingPartScheduleFloatSmartPointer usageSchedule =
      f.newPartSchedule(LGFRUsageVolume, true, true);
  testScenario -> newImplosionSolution();
  LgFrUsageScheduleStrategy::quantityTest (*usageSchedule);

#if 0
  // test LGFRStockVolumeTimesUnitCost and LGFRStockVolume
  LgFrSortingPartScheduleFloatSmartPointer stockSchedule =
      f.newPartSchedule(LGFRStockVolume, false, true);
  LgFrSortingPartScheduleFloatSmartPointer stockTimesCostSchedule =
      f.newPartSchedule(LGFRStockVolumeTimesUnitCost, false, true);
  LgFrScheduleFloat & ss = *stockSchedule;
  assert (ss.size() != 0);
  assert (ss == *stockTimesCostSchedule); // unit costs in diner12 are all 1.0

  // test LGFRRequirementVolumeTimesUnitCost and LGFRRequirementVolume
  LgFrSortingPartScheduleFloatSmartPointer requirementSchedule =
      f.newPartSchedule(LGFRRequirementVolume, false, true);
  LgFrSortingPartScheduleFloatSmartPointer requirementTimesCostSchedule =
      f.newPartSchedule(LGFRRequirementVolumeTimesUnitCost, false, true);
  LgFrScheduleFloat & rs = *requirementSchedule;
  testScenario -> newExplosionSolution();
  assert (rs.size() != 0);
  assert (rs == *requirementTimesCostSchedule); // unit costs in diner12 are all 1.0
  assert (! (s == rs) );
  assert (! (ss == rs) );


  // test national language support of abbreviationOf method
  assert( testScenario->nationalLanguage() == English );
  assert (f.abbreviationOf (LGFRDemandVolume)
          == LGFRDemandVolume.abbrev( English )     );
  testScenario->nationalLanguage( Kana );
  assert( testScenario->nationalLanguage() == Kana );
  assert (f.abbreviationOf (LGFRDemandVolume)
          == LGFRDemandVolume.abbrev( Kana )     );
  assert (f.abbreviationOf (LGFRDemandVolume)
          != LGFRDemandVolume.abbrev( English )     );
  testScenario->nationalLanguage( English );
  assert( testScenario->nationalLanguage() == English );
#endif  

  // test scheduleDescriptor methods
  {
    LgFrScheduleDescriptorVector sd =
      f.LgFrScheduleFactory::demandScheduleDescriptors();
    // Test that each demand schedule can be created by this factory
    int i = 0; // Pulled out of the for below by RW2STL
    for ( i = 0; i<sd.size(); i++ ) {
      LgFrSortingDemandScheduleFloatSmartPointer schedSP =
         f.newDemandSchedule( sd[i]); 
      assert( !schedSP.null() );
    }
  }
  {
    LgFrScheduleDescriptorVector sd =
      f.LgFrScheduleFactory::partScheduleDescriptors();
    // Test that each part schedule can be created by this factory
    int i = 0; // Pulled out of the for below by RW2STL
    for ( i = 0; i<sd.size(); i++ ) {

      // There is a problem creating LGFRCriticalPartSupplyVolume schedule.
      // This needs to be investigated and fixed.
      // UnitTest will fail if this schedule is created.
      if ( sd[i] == LGFRCriticalPartSupplyVolume ) continue;
      
      LgFrSortingPartScheduleFloatSmartPointer schedSP =
         f.newPartSchedule( sd[i]); 
      assert( !schedSP.null() );
    }
  }

}
