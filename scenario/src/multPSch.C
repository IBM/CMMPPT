#include <iostream.h>
#include <fstream.h>
#include <assert.h>

#include <scenario/src/multPSch.h>
#include <scenario/src/scenario.h>
#include <scenario/src/schFctry.h>
#include <scenario/src/pcatPC.h>
#include <scenario/src/schdSnFS.h>
#include <scenario/src/backlgSS.h>
#include <scenario/src/zeroTvFS.h>
#include <scenario/src/tvElemPC.h>
#include <scenario/src/partNmFS.h>
#include <scenario/src/timVComp.h>
#include <scenario/src/schdSnPC.h>
#include <scenario/src/partSchP.h>

// ---------------------------------------------------------------------
// Add a new schedule if it isn't already in the multi-schedule
// and return TRUE, otherwise just return TRUE.
// ---------------------------------------------------------------------
RWBoolean
  LgFrMultiPartScheduleFloat::newSchedule(const LgFrScheduleDescriptor& sd)
{
  // First check to see if there is one already
  if ( this->contains(sd) ) return TRUE;   // Found one
  const LgFrScheduleFactory* sfPtr = this->scenario()->scheduleFactory();
  LgFrSortingPartScheduleFloatSmartPointer partSchedPtr = sfPtr->newPartSchedule(sd, FALSE);
  LgFrDescriptorSchedulePairFloat* dsPairPtr =
    new LgFrDescriptorSchedulePairFloat(sd,partSchedPtr);
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
LgFrMultiPartScheduleFloat::filterItemSelectionSchedule(
   const LgFrScheduleDescriptor& sd,
   const LgFrFilterDescriptor & fd,
   const RWCString & s1,
   const RWCString & s2 )
{
   RWBoolean retVal = FALSE;
   const LgFrScheduleFactory* sf = scenario()->scheduleFactory();

   LgFrSortingPartScheduleFloatSmartPointer filtSched(NULL);
   LgFrScheduleSensitiveFilterStrategy schedSensFiltStrat;

   if ( fd == LGFRFilterOnShortPart ) {

      filtSched = sf->newPartSchedule(LGFRFocussedShortageSchedule);
      filtSched->filterStrategy( LgFrZeroTimeVecFilterStrategy() );
      schedSensFiltStrat.schedule( filtSched );
      retVal = itemSelectionSchedule( sd, &schedSensFiltStrat );

   }

   else if ( fd == LGFRFilterOnShortCapacity ) {

      filtSched = sf->newPartSchedule(LGFRFocussedShortageSchedule);
      // Note: The following filter strategy should be a
      //       booleanOR( zeroTimeVecFS, !partCategoryCapcityRawFS ).
      //       The partCategory filter strategy does not exist and
      //       is not required for the demo since the ATP
      //       capacity window where this filter will be used, already
      //       only has capacit parts.
      filtSched->filterStrategy( LgFrZeroTimeVecFilterStrategy() );
      schedSensFiltStrat.schedule( filtSched );
      retVal = itemSelectionSchedule( sd, &schedSensFiltStrat );

   }

   else if ( fd == LGFRFilterOnPartName ) {

      LgFrPartNameFilterStrategy fs( s1 );
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
// This method handles pair compare strategies related only to "part",
// other general strategies will be handles by the base class.
// ---------------------------------------------------------------------
LgFrPairCompareFloat*
  LgFrMultiPartScheduleFloat::makePairCompareStrategy
                            (LgFrSortDescriptor sortDesc,
                             int period)
        const
{
  LgFrPairCompareFloat* pcPtr = NULL;
  if ( sortDesc == LGFRSortByPart ) {
    // Default pc will do the sorting on part name first.
    pcPtr = new LgFrPairCompareFloat;
  }
  else if ( sortDesc == LGFRSortByCriticality ) {
    // Return a schdSnPC with the critical part rank list as the independent schedule.
    const LgFrScheduleFactory* sf = this->scenario()->scheduleFactory();
    RWBoolean deepCopy = TRUE;
    RWBoolean toPopulate = TRUE;

    // Create a populated critical schedule
    LgFrSortingPartScheduleFloatSmartPointer criticalSchedule
      = sf->newPartSchedule( LGFRCriticalPartRankingList,deepCopy,toPopulate);

    // Create a schedule sensitive pair compare
    LgFrScheduleSensitivePairCompareFloat*
      sspc = new LgFrScheduleSensitivePairCompareFloat;

    // Associate the schedule with the pair compare
    sspc->schedule( criticalSchedule );

    // Compare timeVecs by comparing minimum elements.
    sspc->timeVecCompareFloat(  LgFrMinimumElementTimeVecCompareFloat() );
    pcPtr = sspc;
  }
  else if ( sortDesc == LGFRSortByAvailInPeriod    ||
            sortDesc == LGFRSortByCumAvailInPeriod 
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
  else if ( sortDesc == LGFRSortByPriority ) {
    // need to check with Tom.
  }
  else pcPtr = LgFrMultiScheduleFloat::makePairCompareStrategy(sortDesc,period);
  return pcPtr;
}

// ---------------------------------------------------------------------
// Return the schedule type: has to be PART
// ---------------------------------------------------------------------
LgFrScheduleType
  LgFrMultiPartScheduleFloat::schedType()
     const
{
  return PART;
}

// ---------------------------------------------------------------------
// Return a string suitable for use as the description of the items
// in the schedule
// ---------------------------------------------------------------------
RWCString
LgFrMultiPartScheduleFloat::itemHeading ()
     const
{
  if ( !( scenario().null() ) )
    {
    if ( scenario()->nationalLanguage() == Kana ) return RWCString("ÌÞËÝÊÞÝºÞ³");
    }
  return RWCString ("Part #");
}


const LgFrItem *
LgFrMultiPartScheduleFloat::findItem (const RWCString & guiName)
     const
{
  const LgFrPart * itemPtr
    = ( this->scenario() -> constSetOfParts(LGFR_UNIVERSAL_SET) ) . findValue(guiName);
  if (itemPtr == NULL)
    cerr << "No such part: " << guiName << endl;
  return itemPtr;  
}

// ---------------------------------------------------------------------
// Create a copy of the *this on the heap for a new and different
// scenario and return a pointer to it.
// Make sure it returns the right MS.
// ---------------------------------------------------------------------
LgFrMultiScheduleFloat*
LgFrMultiPartScheduleFloat::clone (LgFrScenarioSmartPointer newScenario)
     const
{
  if ( (LgFrScenarioSmartPointerToConst&)newScenario == this->scenario() ) {
    cerr << "Clone a multi-part-schedule in the same scenario is not allowed." << endl;
    return NULL;
  }
  else return new LgFrMultiPartScheduleFloat(*this, newScenario);
}

// ---------------------------------------------------------------------
// Copy constructor (deep copy to a new and different scenario)
// Used by clone method only so that it can check if the new scenario
// is different from the current one.
// ---------------------------------------------------------------------
LgFrMultiPartScheduleFloat::LgFrMultiPartScheduleFloat
  (const LgFrMultiPartScheduleFloat & source,
   LgFrScenarioSmartPointer newScenario
   )
: LgFrMultiScheduleFloat(source, newScenario)
{
  // Nothing to do here.
}

// ---------------------------------------------------------------------
// Assignment operator (not allowed, use clone(LgFrScenarioSmartPointer) method)
// ---------------------------------------------------------------------
LgFrMultiPartScheduleFloat &
  LgFrMultiPartScheduleFloat::operator=(const LgFrMultiPartScheduleFloat& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
    ( this->LgFrMultiScheduleFloat::operator= ) (rhs);
  }
   return *this;
}

// ---------------------------------------------------------------------
// Copy constructor (not allowed, use clone(LgFrScenarioSmartPointer) method)
// ---------------------------------------------------------------------
LgFrMultiPartScheduleFloat::LgFrMultiPartScheduleFloat
  (const LgFrMultiPartScheduleFloat & source)
:   LgFrMultiScheduleFloat(source)
{
  // Nothing to do in here
}

// ---------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------
LgFrMultiPartScheduleFloat::LgFrMultiPartScheduleFloat (LgFrScenarioSmartPointer scenPtr)
: LgFrMultiScheduleFloat(scenPtr)
{
  // Nothing to do in here
}

// ---------------------------------------------------------------------
// Default constructor
// ---------------------------------------------------------------------
LgFrMultiPartScheduleFloat::LgFrMultiPartScheduleFloat ()
: LgFrMultiScheduleFloat()
{
  // Nothing to do in here
}

// ---------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------
LgFrMultiPartScheduleFloat::~LgFrMultiPartScheduleFloat ()
{
  // Nothing to do in here 
}

// ---------------------------------------------------------------------
// Print member data in class
// ---------------------------------------------------------------------
void LgFrMultiPartScheduleFloat::print() const
{
   cout <<"************************************************" <<endl;
   cout <<"Multiple Part Schedules:" << endl;
   this->LgFrMultiScheduleFloat::print();
}

#ifdef NDEBUG
#undef NDEBUG
#endif

#include <scenario/src/schdDeSt.h>
#include <scenario/src/msItemIt.h>
#include <scenario/src/msAttrIt.h>

// ---------------------------------------------------------------------
// test LgFrMultiPartScheduleFloat methods
// Expects the data to come from
// "/u/implode/wit/data/examples/diner12/wit.data"
// ---------------------------------------------------------------------
void
LgFrMultiPartScheduleFloat::contextTest(
                                    LgFrScenarioSmartPointer scenarioPtr,
                                    LgFrScenarioSmartPointer newScenarioPtr)
{
  // Test default, copy constructors and assignment
  LgFrMultiPartScheduleFloat defmsp;
  assert( defmsp.numberDependents()==0 );
  LgFrMultiPartScheduleFloat copymsp = defmsp;
  assert( copymsp.numberDependents()==0 );
  LgFrMultiPartScheduleFloat assignmsp;
  assert( assignmsp.numberDependents()==0 );
  assignmsp = defmsp;
  assert( assignmsp.numberDependents()==0 );
  assert ( defmsp.entries() == 0 );
  assert ( copymsp.isEmpty() );
  assert ( assignmsp.isEmpty() && !assignmsp.contains(LGFRBacklogVolume));

  LgFrMultiPartScheduleFloat msp(scenarioPtr);
  assert ( msp.isEmpty() && (!msp.scenario().null()) );
    
  // Test the clone method
  {
    LgFrMultiScheduleFloat* clonemsp = defmsp.clone(newScenarioPtr);
    assert ( clonemsp->isEmpty() );
    delete clonemsp;            // leak fix attempt by Wally, 9/27/94
  }

  // Test the itemHeading method
  assert (defmsp.itemHeading() == "Part #");

  // Run implosion
  LgFrScenarioSmartPointer scenario = scenarioPtr;
  //  Use default implosion method.
  /*
  {
    // Need to set flag to true to get the critical list
    // (make sure the contextTest is a friend of scenario)
    LgFrLastProblemSolved& lps = scenario.lastProblemSolved();
    lps.computeCriticalList(TRUE);
  }
  */
  scenario->newImplosionSolution();
  
  // Test newSchedule, deleteSchedule
  msp.newSchedule (LGFRSupplyVolume);
  assert (msp.numberDependents() == 0 );
  msp.newSchedule (LGFRProductionVolume);
  assert (msp.numberDependents() == 0 );
  assert (msp.entries() == 2);        
  assert (msp.numberDependents() == 0 );
  msp.deleteSchedule (LGFRProductionVolume);
  assert (msp.entries() == 1);
  assert (msp.numberDependents() == 0 );
  msp.newSchedule (LGFRExcessVolume);
  msp.newSchedule (LGFRRequirementVolume);
  assert (msp.entries() == 3);
  msp.deleteSchedule (LGFRExcessVolume);
  assert (msp.entries() == 2);
  msp.newSchedule (LGFRProductionVolume);
  assert (msp.entries() == 3);
  assert (!msp.isEmpty());
  assert (msp.contains(LGFRSupplyVolume));
  assert (msp.contains(LGFRProductionVolume));
  assert (msp.contains(LGFRRequirementVolume));
  assert (!msp.contains(LGFRExcessVolume));
  assert (!msp.contains(LGFRFocussedShortageSchedule));
  assert (!msp.contains(LGFRCriticalPartRankingList));

  // Test priority, priorityShift, and prioritySwap
  assert ( msp.priority(LGFRSupplyVolume) == 0 );
  assert ( msp.priority(LGFRRequirementVolume) == 1 );
  assert ( msp.priority(LGFRProductionVolume) == 2 );
  msp.priorityShift(LGFRSupplyVolume, LGFRProductionVolume, AFTER);
  assert ( msp.priority(LGFRSupplyVolume) == 2 );
  assert ( msp.priority(LGFRRequirementVolume) == 0 );
  assert ( msp.priority(LGFRProductionVolume) == 1 );
  msp.priorityShift(LGFRProductionVolume, LGFRRequirementVolume, BEFORE);
  assert ( msp.priority(LGFRSupplyVolume) == 2 );
  assert ( msp.priority(LGFRRequirementVolume) == 1 );
  assert ( msp.priority(LGFRProductionVolume) == 0 );
  msp.prioritySwap(LGFRProductionVolume, LGFRRequirementVolume);
  assert ( msp.priority(LGFRSupplyVolume) == 2 );
  assert ( msp.priority(LGFRRequirementVolume) == 0 );
  assert ( msp.priority(LGFRProductionVolume) == 1 );
  msp.prioritySwap(LGFRProductionVolume, LGFRSupplyVolume);
  assert ( msp.priority(LGFRSupplyVolume) == 1 );
  assert ( msp.priority(LGFRRequirementVolume) == 0 );
  assert ( msp.priority(LGFRProductionVolume) == 2 );
  msp.prioritySwap(LGFRSupplyVolume, LGFRRequirementVolume);
  assert ( msp.priority(LGFRSupplyVolume) == 0 );
  assert ( msp.priority(LGFRRequirementVolume) == 1 );
  assert ( msp.priority(LGFRProductionVolume) == 2 );
  
  // Test find, isMutable
  LgFrDescriptorSchedulePairFloat* dsPairPtr = msp.find(LGFRSupplyVolume);
  assert ( dsPairPtr->descriptor() == LGFRSupplyVolume );
  assert ( dsPairPtr->schedule()->entries() == 19 );
  assert ( msp.isMutable(LGFRSupplyVolume) );
  assert ( !msp.isMutable(LGFRProductionVolume) );
  assert ( !msp.isMutable(LGFRRequirementVolume) );
  
  // Test abbreviationOf and schedAbbrevList
  assert ( msp.abbreviationOf(LGFRSupplyVolume) == "Supply" );
  assert ( msp.abbreviationOf(LGFRRequirementVolume) == 
           LGFRRequirementVolume.abbrev() );
  assert ( msp.abbreviationOf(LGFRProductionVolume) == 
           LGFRProductionVolume.abbrev() );
  LgFrVectorRWCString abbrevVec = msp.schedAbbrevVec();
  assert ( abbrevVec.length() == 3 );
//  cout << "Printing out the abbreviation vector" << endl;
//  cout << "Length of the abbreviation vector = " << abbrevVec.length() << endl;
  for (int i = 0; i < abbrevVec.length(); i++ ) {
    RWCString abbrev = abbrevVec(i);
//    cout << "abbrev # " << i << " = " << abbrev << endl;
    if ( i == 0 ) {  assert ( abbrev == LGFRSupplyVolume.abbrev() );  }
    else if ( i == 1 ) {  assert ( abbrev == LGFRRequirementVolume.abbrev() );  }
    else {  assert ( abbrev == LGFRProductionVolume.abbrev() );  }
  }
  
  // Test itemSelectionSchedule and itemOrderSchedule, sort
  assert ( !(msp.itemSelectionSchedule().name() == "") );
  assert ( !(msp.itemOrderSchedule().name() == "") );
  assert ( msp.itemSelectionSchedule() == LGFRSupplyVolume );
  assert ( msp.itemOrderSchedule() == LGFRSupplyVolume );
  
  LgFrZeroTimeVecFilterStrategy fs;
  msp.itemSelectionSchedule(LGFRSupplyVolume,&fs);
  assert ( msp.schedule(LGFRSupplyVolume)->entries() == 9 );
  assert ( msp.itemSelectionSchedule() == LGFRSupplyVolume );
  
  LgFrPartCategoryPairCompareFloat pc( *scenarioPtr );
  assert ( msp.itemOrderSchedule() == LGFRSupplyVolume );
  msp.itemOrderSchedule(msp.itemOrderSchedule(),&pc);
  assert ( msp.schedule(msp.itemOrderSchedule())->
           compareStrategy()->isA() == pc.isA() );
  /*
  cout << "Print msp after part category pair compare" << endl;
  msp.printByItemAndAttribute();
  
  msp.sort();
  cout << "Print msp by default" << endl;
  msp.printByItemAndAttribute();
  
  msp.sort(LGFRSortByPart);
  cout << "Print msp after sorting by part name" << endl;
  msp.printByItemAndAttribute();
  */
  /*
  {
    // Test sorting by criticality
    RWBoolean deepCopy = TRUE;
    RWBoolean toPopulate = TRUE;
    LgFrSortingPartScheduleFloat* critPartSchd =
      scenario.scheduleFactory()->newPartSchedule
                                  (LGFRCriticalPartRankingList,
				   deepCopy, toPopulate);
    cout << endl <<"Critical Part Rank Schedule:" <<endl
         << critPartSchd->format() << endl;
    delete critPartSchd;
    msp.sort(LGFRSortByCriticality);
    cout << "Print msp after sorting by criticality" << endl;
    msp.printByItemAndAttribute();
  }
  */
/*  
  msp.sort(LGFRSortByAvailInPeriod,0);
  cout << "Print msp after sorting by avail. in period 0" << endl;
  msp.printByItemAndAttribute();
  
  msp.sort(LGFRSortByCumAvailInPeriod,1);
  cout << "Print msp after sorting by cum. avail. in period 1" << endl;
  msp.printByItemAndAttribute(); 
  */
  // commented out for demo purpose because we only allow same
  // selection and order schedule at this time.
  /*
  msp.itemOrderSchedule(LGFRProductionVolume,&pc);
  assert ( msp.itemOrderSchedule() == LGFRProductionVolume );
  assert ( msp.schedule(LGFRProductionVolume)->entries() == 10 );
  assert ( msp.schedule(LGFRProductionVolume)->compareStrategy()->isA() == pc.isA() );
  */

//  cout << "*** printing msp ***" << endl; 
//  msp.print();


  // Test clone with new scenario and cloneDescSchdList
  LgFrMultiPartScheduleFloat * mspClonePtr 
    = ( (LgFrMultiPartScheduleFloat*)(msp.clone(newScenarioPtr)) );
  {
    LgFrMultiPartScheduleFloat& mspClone  =  * mspClonePtr;
    assert ( !mspClone.isEmpty() );
    assert ( mspClone.entries() == 3 );
    assert ( mspClone.priority(LGFRSupplyVolume) == 0 );
    assert ( mspClone.priority(LGFRRequirementVolume) == 1 );
    assert ( mspClone.priority(LGFRProductionVolume) == 2 );

//    cout << "*** printing mspClone ***" << endl; 
//    mspClone.print();

  
    // Test changeTimeVecInSchedule and changeValueInSchedule
    LgFrPart egg("Egg");
    LgFrPart butter("Butter");
    assert ( msp.changeValueInSchedule( egg, LGFRSupplyVolume, 1, 10.0) );
    assert ( msp.schedule(LGFRSupplyVolume)->entries() == 9 );

//  cout << "*** printing supply schedule after changing a value in eggTV ***" << endl;
//  cout << "supply size = " <<  msp.schedule(LGFRSupplyVolume)->entries();
//  msp.schedule(LGFRSupplyVolume)->print();
  
    LgFrTimeVecFloat newTV(2,"1.0 3.5");
    assert ( msp.changeTimeVecInSchedule( butter, LGFRSupplyVolume, newTV) );
    const LgFrSortingScheduleFloat& supplySchd = *(msp.schedule(LGFRSupplyVolume));
    // Now the supply entries = 10 because butter was not in the
    // non-zero supply sched before and now added in.
    assert ( msp.schedule(LGFRSupplyVolume)->entries() == 10 );
    
//  cout << "*** printing supply schedule after changing butterTV ***" << endl;
//  cout << "supply size = " <<  msp.schedule(LGFRSupplyVolume)->entries();
//  msp.schedule(LGFRSupplyVolume)->print();
  
    const LgFrTimeVecFloat& eggTV = supplySchd[egg].timeVecFloat();
    assert ( eggTV[1] == 10.0 );
    const LgFrTimeVecFloat& butterTV = supplySchd[butter].timeVecFloat();
    assert ( butterTV[0] == 1.0 && butterTV[1] == 3.5 );

    const LgFrSortingScheduleFloat& supplySchdClone
      = *(mspClone.schedule(LGFRSupplyVolume));
    const LgFrTimeVecFloat& eggTVClone = supplySchdClone[egg].timeVecFloat();
    assert ( eggTVClone[1] == 2.0 );

    // Test descSchedPairList, schedule, scenario
    assert ( !msp.scenario().null()  && !mspClone.scenario().null() );
    assert ( mspClone.scenario() != msp.scenario() );
    
    assert ( msp.schedule(LGFRSupplyVolume)->entries() == 10 );
    assert ( msp.schedule(LGFRProductionVolume)->entries() == 10);
    const LgFrDescriptorSchedulePairFloatGSList& dsPairList = msp.descSchedPairList();
    const LgFrDescriptorSchedulePairFloatGSList&
      dsPairListClone = mspClone.descSchedPairList();
    assert ( dsPairList.entries() == 3 );
    assert ( dsPairList.entries() == dsPairListClone.entries() );

    // Test LgFrMultiScheduleItemIterator and LgFrMultiScheduleAttributeIterator
    int nitems = msp.numberOfItems();
    int nattrs = msp.entries();
    int nperiods = scenarioPtr->numberOfPeriods();
    int totAttrs = nitems*nattrs;
    LgFrItemTimeVecPairFloat* itvMatrixPtr
      = new LgFrItemTimeVecPairFloat[totAttrs];
    const LgFrTimeVecFloat zerotv(nperiods,0.0);

    LgFrMultiScheduleItemIterator majorI (&msp);
    LgFrMultiScheduleItemIterator* nextItemIterPtr;
    int itemIndex = -1;
    int attrIndex = -1;

    // Iterate through all of the (major) rows
    while ( nextItemIterPtr = majorI()) {
      const LgFrItem& item = nextItemIterPtr->item();
      itemIndex++;
      attrIndex = -1;
      // Create an attribute (minor) iterator
      LgFrMultiScheduleAttributeIterator minorI (*nextItemIterPtr);
      LgFrMultiScheduleAttributeIterator* nextAttrIterPtr;
      while ( nextAttrIterPtr = minorI() ) {   // Not end of list
        attrIndex++;
        int index = itemIndex*nattrs + attrIndex;
        const LgFrItemTimeVecPairFloat * const
          curITV = nextAttrIterPtr->itemTimeVec();
        if ( curITV ) {
          *(itvMatrixPtr+index) = *curITV;
          // curITV->print();
        }
        else {
          LgFrItemTimeVecPairFloat zeroitv(item,zerotv);
          *(itvMatrixPtr+index) = zeroitv;
        }
      }
    }

    // Test base class Regular expression index method
    {
      // Find index of GrlChs_Sand
      size_t index = msp.index( "GrlChs_Sand");

      // Test to see if index really does contain GrlChs_Sand
      LgFrMultiScheduleItemIterator majorI (&msp,index);
      assert( majorI()->item().name() == "GrlChs_Sand" );
 
      // There should not be another GrlChs_Sand Part
      assert( msp.index( "GrlChs_Sand",index+1) == RW_NPOS );
    }

    // Test base class "contains search string" index method
    {
      // Find index of GrlChs_Sand using regular expression search method
      size_t reIndex = msp.index( "GrlChs_Sand");

      // Find index of item containing "lChs_S" string, ignoring case
      size_t containsIndex = msp.index( "LcHS_s", TRUE );
      assert( containsIndex == reIndex );
      
      // Find index of item containing "lChs_S" string, respecting case
      containsIndex = msp.index( "lChs_S", FALSE );
      assert( containsIndex == reIndex ); 
 
      // There should not be another GrlChs_Sand Part
      assert( msp.index( "lChs_S",FALSE,reIndex+1) == RW_NPOS );
    }
//    assert ( itvMatrix[0][0].item().name() == "Skillet");
//    assert ( (itvMatrix[0][0].timeVecFloat())[1] == 20.00 );
//    assert ( itvMatrix[9][2].item().name() == "GrlChs_Sand" );
//    assert ( (itvMatrix[9][2].timeVecFloat())[0] == 4.0 );

    delete [] itvMatrixPtr;
//    msp.printByItemAndAttribute();

    // Test schedType()
    LgFrScheduleType stype = msp.schedType();
    assert ( stype == PART );

    // Test clear and destructor
    mspClone.clear();
    assert ( mspClone.isEmpty() );
    LgFrMultiPartScheduleFloat* newMsp = new LgFrMultiPartScheduleFloat(msp);
    delete newMsp;
  }

  // Test base class format and printMultiScheduleReport methods
  cout <<"----multiSchedule::format()--------" <<endl;
  cout <<msp.format();
  cout <<"----multiSchedule::printMultiScheduleReport()--------" <<endl;
  msp.printMultiScheduleReport();
  cout <<"----multiSchedule::printMultiScheduleReport(TEXT,NULL,TRUE)--" <<endl;
  msp.printMultiScheduleReport(TEXT,NULL,TRUE);
  cout <<"----multiSchedule::printMultiScheduleReport(CSV,&cout)------" <<endl;
  msp.printMultiScheduleReport(CSV,&cout);
  cout <<"----multiSchedule::printMultiScheduleReport(CSV,&cout,TRUE)-" <<endl;
  msp.printMultiScheduleReport(CSV,&cout,TRUE);
  cout <<"----------" <<endl;

  delete mspClonePtr;

}




