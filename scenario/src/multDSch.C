#include <iostream.h>
#include <scenario/src/multDSch.h>
#include <scenario/src/scenario.h>
#include <scenario/src/schFctry.h>
#include <scenario/src/schdSnFS.h>
#include <scenario/src/backlgSS.h>
#include <scenario/src/zeroTvFS.h>
#include <scenario/src/tvElemPC.h>
#include <scenario/src/demdPnPC.h>
#include <scenario/src/demdNmFS.h>
#include <scenario/src/demdSchP.h>

// ---------------------------------------------------------------------
// Add a new schedule if it isn't already in the multi-schedule
// and return TRUE, otherwise just return TRUE.
// ---------------------------------------------------------------------
RWBoolean
  LgFrMultiDemandScheduleFloat::newSchedule(const LgFrScheduleDescriptor& sd)
{
  // First check to see if there is one already
  if ( this->contains(sd) ) return TRUE;   
  const LgFrScheduleFactory* sfPtr = this->scenario()->scheduleFactory();
  LgFrSortingDemandScheduleFloatSmartPointer demandSchedPtr = sfPtr->newDemandSchedule(sd, FALSE);
  LgFrDescriptorSchedulePairFloat* dsPairPtr =
    new LgFrDescriptorSchedulePairFloat(sd,demandSchedPtr);
  LgFrDescriptorSchedulePairFloatGSList& descSchedPairList = this->descSchedPairList();
  descSchedPairList.insert(dsPairPtr);
  addSubscription( dsPairPtr );

  // Set default selection and order schedules if this is the first one
  if ( this->entries() == 1) {
    this->itemSelectionSchedule(sd,NULL);
    this->itemOrderSchedule(sd,NULL);
  }
  return TRUE;
}

//---------------------------------------------------------------------------
// Set a Selection schedule using a FilterDescriptor
// This method removes the filter from the current selection schedule.
// Implementation note: this call can be used to change the filter if sd is
// the descriptor of the old selection-schedule
// Return TRUE  :if sd is found and changes are made.
//        FALSE :if sd is not found.
//---------------------------------------------------------------------------
RWBoolean
LgFrMultiDemandScheduleFloat::filterItemSelectionSchedule(
   const LgFrScheduleDescriptor& sd,
   const LgFrFilterDescriptor & fd,
   const RWCString & s1,
   const RWCString & s2 )
{
   RWBoolean retVal = FALSE;
   const LgFrScheduleFactory* sf = scenario()->scheduleFactory();

   LgFrSortingDemandScheduleFloatSmartPointer filtSched(NULL);
   LgFrScheduleSensitiveFilterStrategy schedSensFiltStrat;

   if ( fd == LGFRFilterOnBacklogDemand ) {

      filtSched = sf->newDemandSchedule(LGFRBacklogVolume);
      filtSched->filterStrategy( LgFrZeroTimeVecFilterStrategy() );
      schedSensFiltStrat.schedule( filtSched );
      retVal = itemSelectionSchedule( sd, &schedSensFiltStrat );
 
   }

   else if ( fd == LGFRFilterOnDemandAndPartName ) {

      LgFrDemandNameFilterStrategy fs( s1, s2 );
      retVal = itemSelectionSchedule( sd, &fs );
 
   }

   else {
      retVal =
         LgFrMultiScheduleFloat::filterItemSelectionSchedule( sd, fd, s1,s2 );
   }

   return retVal;
}

// ---------------------------------------------------------------------
// Return a pair compair strategy that is corresponding to the
// sorting descriptor and optional second parameter period.
// Client of this method has to free the memory of LgFrPairCompareFloat*
// returned.
// This method handles pair compare strategies related only to "demand",
// other general strategies will be handles by the base class.
// ---------------------------------------------------------------------
LgFrPairCompareFloat*
  LgFrMultiDemandScheduleFloat::makePairCompareStrategy
                            (LgFrSortDescriptor sortDesc,
                             int period)
        const
{
  LgFrPairCompareFloat* pcPtr = NULL;
  if ( sortDesc == LGFRSortByPartNameAndDemand ) {
    // DemandPnPC will sort by part name first and then the demand name.
    pcPtr = new LgFrDemandPartNamePairCompare;
  }
  else if ( sortDesc == LGFRSortByCustomer ) {
    // need to check with Tom.
  }
  else if ( sortDesc == LGFRSortByOrderNumber ) {
  }  // need to check with Tom.
  else if ( sortDesc == LGFRSortByDemandQtyInPeriod      ||
            sortDesc == LGFRSortByCumDemandQtyInPeriod   ||
            sortDesc == LGFRSortByCumDemandRevInPeriod   ||
            sortDesc == LGFRSortByCumShipmentRevInPeriod 
            ) {
    LgFrTimeVecElementPairCompareFloat* tvElemPC =
      new LgFrTimeVecElementPairCompareFloat;
    if ( period != -1 ) {
      assert (  0 <= period &&
                period < this->scenario()->numberOfPeriods() );
      tvElemPC->period(period);
    }
    pcPtr = tvElemPC; 
  }
  else pcPtr = LgFrMultiScheduleFloat::makePairCompareStrategy(sortDesc,period);
  return pcPtr;
}

// ---------------------------------------------------------------------
// Return the schedule type: has to be DEMAND
// ---------------------------------------------------------------------
LgFrScheduleType
  LgFrMultiDemandScheduleFloat::schedType()
     const
{
  return DEMAND;
}

// ---------------------------------------------------------------------
// Return a string suitable for use as the description of the items
// in the schedule
// ---------------------------------------------------------------------
RWCString
LgFrMultiDemandScheduleFloat::itemHeading ()
     const
{
  if ( !( scenario().null() ) )
    {
    if ( scenario()->nationalLanguage() == Kana )
      return RWCString("ÌÞËÝÊÞÝºÞ³") + RWCString(", ") + RWCString("ÃÞÝËß®³ No");
    }
    return RWCString("Part #") + RWCString(", ") + RWCString("Demand #");
}


const LgFrItem *
LgFrMultiDemandScheduleFloat::findItem (const RWCString & guiName)
     const
{
  RWCString sep
    = LgFrDemand::guiSeparator ();
  int sepStart = guiName.index(sep);
  if (sepStart == RW_NPOS) {
    cerr << "Demand formatted incorrectly: " << guiName << endl;
    return NULL;
  }
  else {
    RWCString partName = guiName (0, sepStart);
    int demandNameLen = guiName.length() - sep.length() - partName.length();
    RWCString demandName = guiName (sepStart + sep.length(), demandNameLen);
    const LgFrItem * itemPtr
      = ( this->scenario() -> constSetOfDemands(LGFR_UNIVERSAL_SET)
          ) . findValue(demandName,
                        partName);
    if (itemPtr == NULL)
      cerr << "No such demand: " << guiName << endl;
    return itemPtr;
  }
}
     

// ---------------------------------------------------------------------
// Create a copy of the *this on the heap for a new and different
// scenario and return a pointer to it.
// Make sure it returns the right MS.
// ---------------------------------------------------------------------
LgFrMultiScheduleFloat*
LgFrMultiDemandScheduleFloat::clone(LgFrScenarioSmartPointer newScenario)
     const
{
  if ( (LgFrScenarioSmartPointerToConst&)newScenario == this->scenario() ) {
    cerr << "Clone a multi-demand-schedule in the same scenario is not allowed." << endl;
    return NULL;
  }
  else return new LgFrMultiDemandScheduleFloat(*this, newScenario);
}

// ---------------------------------------------------------------------
// Copy constructor (deep copy to a new and different scenario)
// Used by clone method only so that it can check if the new scenario
// is different from the current one.
// ---------------------------------------------------------------------
LgFrMultiDemandScheduleFloat::LgFrMultiDemandScheduleFloat
  (const LgFrMultiDemandScheduleFloat & source,
   LgFrScenarioSmartPointer newScenario
   )
: LgFrMultiScheduleFloat(source, newScenario)
{
  // Nothing to do here.
}

// ---------------------------------------------------------------------
// Assignment operator (not allowed, use clone(LgFrScenarioSmartPointer) method)
// ---------------------------------------------------------------------
LgFrMultiDemandScheduleFloat &
  LgFrMultiDemandScheduleFloat::operator=(const LgFrMultiDemandScheduleFloat& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
    ( this->LgFrMultiScheduleFloat::operator= ) (rhs);
  }
   return *this;
}

// ---------------------------------------------------------------------
// Copy constructor (not allowed, use clone(LgFrScenarioSmartPointer) method)
// ---------------------------------------------------------------------
LgFrMultiDemandScheduleFloat::LgFrMultiDemandScheduleFloat
  (const LgFrMultiDemandScheduleFloat & source)
:   LgFrMultiScheduleFloat(source)
{
  // Nothing to do in here
}

// ---------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------
LgFrMultiDemandScheduleFloat::LgFrMultiDemandScheduleFloat (LgFrScenarioSmartPointer scenPtr)
: LgFrMultiScheduleFloat(scenPtr)
{
  // Nothing to do in here
}

// ---------------------------------------------------------------------
// Default constructor
// ---------------------------------------------------------------------
LgFrMultiDemandScheduleFloat::LgFrMultiDemandScheduleFloat ()
: LgFrMultiScheduleFloat()
{
  // Nothing to do in here
}

// ---------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------
LgFrMultiDemandScheduleFloat::~LgFrMultiDemandScheduleFloat ()
{
  // Nothing to do in here 
}

// ---------------------------------------------------------------------
// Print member data in class
// ---------------------------------------------------------------------
void LgFrMultiDemandScheduleFloat::print() const
{
   cout <<"*******************************************************************" <<endl;
   cout <<"Multiple Demand Schedules:" << endl;
   this->LgFrMultiScheduleFloat::print();
}

#ifdef NDEBUG
#undef NDEBUG
#endif

#include <scenario/src/schdDeSt.h>

// ---------------------------------------------------------------------
// test LgFrMultiDemandScheduleFloat methods
// Expects the data to come from
// "/u/implode/wit/data/examples/diner12/wit.data"
// ---------------------------------------------------------------------
void
LgFrMultiDemandScheduleFloat::contextTest(
                                    LgFrScenarioSmartPointer scenarioPtr,
                                    LgFrScenarioSmartPointer newScenarioPtr)
{
  // Test default, copy constructors and assignment
  LgFrMultiDemandScheduleFloat defmsd;
  LgFrMultiDemandScheduleFloat copymsd = defmsd;
  LgFrMultiDemandScheduleFloat assignmsd;
  assignmsd = defmsd;
  assert ( defmsd.entries() == 0 );
  assert ( copymsd.isEmpty() );
  assert ( assignmsd.isEmpty() && !assignmsd.contains(LGFRBacklogVolume));
  LgFrMultiDemandScheduleFloat msd(scenarioPtr);
  assert ( msd.isEmpty() && !msd.scenario().null() );

  // Test the clone method
  {
    LgFrMultiDemandScheduleFloat* clonemsd
      = (LgFrMultiDemandScheduleFloat*) defmsd.clone(newScenarioPtr);
    assert ( clonemsd->isEmpty() );
    delete clonemsd;            // leak fix attempt by Wally, 9/27/94
  }

  // Test the itemHeading method
  assert ( ! defmsd.itemHeading().isNull());
  
  // Run implosion
  LgFrScenarioSmartPointer scenario = scenarioPtr;
  //  Use default implosion method.
  scenario->newImplosionSolution();
  
  // Test newSchedule, deleteSchedule, and schedDescVec
  msd.newSchedule (LGFRDemandVolume);
  msd.newSchedule (LGFRShipmentVolume);
  assert (msd.entries() == 2);
  LgFrVectorScheduleDescriptor sd = msd.schedDescVec();
  assert (sd.length() == 2);
  assert (sd[0] == LGFRDemandVolume);
  assert (sd[1] == LGFRShipmentVolume);
  msd.deleteSchedule (LGFRDemandVolume);
  assert (msd.entries() == 1  &&  msd.schedDescVec().length() == 1);
  msd.newSchedule (LGFRBacklogVolume);
  msd.newSchedule (LGFRDemandVolume);
  assert (msd.entries() == 3);
  msd.deleteSchedule (LGFRBacklogVolume);
  assert (msd.entries() == 2);
  msd.newSchedule (LGFRBacklogVolume);
  assert (msd.entries() == 3);
  assert (!msd.isEmpty());
  
  assert (msd.contains(LGFRDemandVolume));
  assert (msd.contains(LGFRBacklogVolume));
  assert (msd.contains(LGFRShipmentVolume));
  
  assert (!msd.contains(LGFRExcessVolume));
  assert (!msd.contains(LGFRFocussedShortageSchedule));
  assert (!msd.contains(LGFRCriticalPartRankingList));
  
  // Test priority, priorityShift, and prioritySwap
  assert ( msd.priority(LGFRShipmentVolume) == 0 );
  assert ( msd.priority(LGFRDemandVolume) == 1 );
  assert ( msd.priority(LGFRBacklogVolume) == 2 );
  msd.priorityShift(LGFRShipmentVolume, LGFRBacklogVolume, AFTER);
  assert ( msd.priority(LGFRShipmentVolume) == 2 );
  assert ( msd.priority(LGFRBacklogVolume) == 1 );
  assert ( msd.priority(LGFRDemandVolume) == 0 );
  msd.priorityShift(LGFRBacklogVolume, LGFRDemandVolume, BEFORE);
  assert ( msd.priority(LGFRShipmentVolume) == 2 );
  assert ( msd.priority(LGFRBacklogVolume) == 0 );
  assert ( msd.priority(LGFRDemandVolume) == 1 );
  msd.prioritySwap(LGFRShipmentVolume, LGFRDemandVolume);
  msd.prioritySwap(LGFRBacklogVolume, LGFRDemandVolume);
  msd.prioritySwap(LGFRShipmentVolume, LGFRBacklogVolume);
  assert ( msd.priority(LGFRShipmentVolume) == 2 );
  assert ( msd.priority(LGFRDemandVolume) == 0 );
  assert ( msd.priority(LGFRBacklogVolume) == 1 );

  // Test index, find, isMutable
  assert ( msd.index(LGFRShipmentVolume) == 2 );
  assert ( msd.index(LGFRDemandVolume) == 0 );
  assert ( msd.index(LGFRBacklogVolume) == 1 );
  LgFrDescriptorSchedulePairFloat* dsPairPtr = msd.find(LGFRDemandVolume);
  assert ( dsPairPtr->descriptor() == LGFRDemandVolume );
  assert ( dsPairPtr->schedule()->entries() == 10 );
  assert ( msd.isMutable(LGFRDemandVolume) );
  assert ( !msd.isMutable(LGFRShipmentVolume) );
  assert ( !msd.isMutable(LGFRBacklogVolume) );
  
  // Test itemSelectionSchedule and itemOrderSchedule, and sort
  assert ( !(msd.itemSelectionSchedule().name() == "") );
  assert ( !(msd.itemOrderSchedule().name() == "") );
  LgFrZeroTimeVecFilterStrategy fs;
  msd.itemSelectionSchedule(LGFRDemandVolume,&fs);
  assert ( msd.itemSelectionSchedule() == LGFRDemandVolume );
  assert ( msd.schedule(LGFRDemandVolume)->entries() == 5 );

  LgFrDemandPartNamePairCompare pc;
  msd.itemOrderSchedule(LGFRDemandVolume,&pc);
  assert ( msd.itemOrderSchedule() == LGFRDemandVolume );
  assert ( msd.schedule(LGFRDemandVolume)->entries() == 5 );
  assert ( msd.schedule(LGFRDemandVolume)->compareStrategy()->isA() == pc.isA() );
/*
  cout << "Print msd after sorting by part name and demand name" << endl;
  msd.printByItemAndAttribute();
  
  msd.sort();
  cout << "Print msd by default" << endl;
  msd.printByItemAndAttribute();

  msd.sort(LGFRSortByDemandQtyInPeriod,0);
  cout << "Print msd after sorting by demand qty in period 0" << endl;
  msd.printByItemAndAttribute();
  
  msd.sort(LGFRSortByCumDemandQtyInPeriod,1);
  cout << "Print msd after sorting by cum. demand qty in period 1" << endl;
  msd.printByItemAndAttribute();
  
  msd.sort(LGFRSortByCumDemandRevInPeriod,3);
  cout << "Print msd after sorting by cum. demand rev in period 3" << endl;
  msd.printByItemAndAttribute();

  msd.sort(LGFRSortByCumShipmentRevInPeriod,2);
  cout << "Print msd after sorting by cum. shipment rev in period 2" << endl;
  msd.printByItemAndAttribute();
  */  
   // Test clone with new scenario and cloneDescSchdList
  LgFrMultiDemandScheduleFloat* msdClonePtr
    =  ( (LgFrMultiDemandScheduleFloat*)(msd.clone(newScenarioPtr)) );
  {
    LgFrMultiDemandScheduleFloat& msdClone = *msdClonePtr;
    assert ( !msdClone.isEmpty() );
    assert ( msdClone.entries() == 3 );
    assert ( msdClone.priority(LGFRDemandVolume) == 0 );
    assert ( msdClone.priority(LGFRShipmentVolume) == 2 );
    assert ( msdClone.priority(LGFRBacklogVolume) == 1 );
  
    // Test changeTimeVecInSchedule and changeValueInSchedule
    LgFrPart ham_sandp("Ham_Sand");
    LgFrPart plain_omletp("Plain_Omlet");
    LgFrDemand ham_sand( "demand", &ham_sandp);
    LgFrDemand plain_omlet( "demand", &plain_omletp);
    // msd.changeValueInSchedule( ham_sand, LGFRDemandVolume, 1, 10.0);
    assert (   msd.changeValueInSchedule( RWCString("Ham_Sand")
                                            + LgFrDemand::guiSeparator()
                                            + "demand",
                                          LGFRDemandVolume, 1, 10.0)      );
  
    LgFrTimeVecFloat newTV(2,"1.0 5.0");
    assert (   msd.changeTimeVecInSchedule( RWCString("Plain_Omlet")
                                              + LgFrDemand::guiSeparator()
                                              + "demand",
                                            LGFRDemandVolume, newTV)       );

    const LgFrSortingScheduleFloat& demandSchd = *(msd.schedule(LGFRDemandVolume));
    const LgFrTimeVecFloat& ham_sandTV = demandSchd[ham_sand].timeVecFloat();
    assert ( ham_sandTV[1] == 10.0 );
    const LgFrTimeVecFloat& plain_omletTV = demandSchd[plain_omlet].timeVecFloat();
    assert ( plain_omletTV[0] == 1.0 && plain_omletTV[1] == 5.0 );

    //// LET'S TRY THIS AGAIN...
    /////const LgFrSortingDemandScheduleFloat& demandSchdClone
    /////= *( (LgFrSortingDemandScheduleFloatSmartPointer&)(msdClone.schedule(LGFRDemandVolume)) );

    LgFrSortingScheduleFloatSmartPointer XtmpschX = msdClone.schedule(LGFRDemandVolume);
    LgFrSortingDemandScheduleFloatSmartPointer &XtmpschX2 = *(LgFrSortingDemandScheduleFloatSmartPointer*)&XtmpschX;
    const LgFrSortingDemandScheduleFloat& demandSchdClone = *(XtmpschX2);

    ///// DONE

    const LgFrTimeVecFloat& ham_sandTVClone
      = demandSchdClone[ham_sand].timeVecFloat();
    assert ( ham_sandTVClone[1] == 8.0 );

    // Test descSchedPairList, schedule, scenario
    assert ( !msd.scenario().null() && !msdClone.scenario().null() );
    assert ( msdClone.scenario() != msd.scenario() );
    assert ( msd.schedule(LGFRDemandVolume)->entries() == 6 );
    assert ( msd.schedule(LGFRShipmentVolume)->entries() == 10 );
    const LgFrDescriptorSchedulePairFloatGSList&
      dsPairList = msd.descSchedPairList();
    const LgFrDescriptorSchedulePairFloatGSList&
      dsPairListClone = msdClone.descSchedPairList();
    assert ( dsPairList.entries() == 3 );
    assert ( dsPairList.entries() == dsPairListClone.entries() );
    
    // Test schedType()
    LgFrScheduleType stype = msd.schedType();
    assert ( stype == DEMAND );
    
    // Test clear and destructor
    msdClone.clear();
    assert ( msdClone.isEmpty() );
    LgFrMultiDemandScheduleFloat* newMsd = new LgFrMultiDemandScheduleFloat(msd);
    delete newMsd;
  }
  delete msdClonePtr;
}

