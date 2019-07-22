// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL -- inserted includes.
#include <iostream>
#include <math.h>

#include <scenario/src/cumSS.h>
#include <scenario/src/partSchd.h>
#include <scenario/src/partSchP.h>
#include <scenario/src/scenario.h>
#include <scenario/src/demdSchd.h>
#include <scenario/src/demdSchP.h>
#include <scenario/src/demdPnPC.h>

//-----------------------------------------------------------------------
//
//  Implementation of LgFrCumTimeVecPartScheduleStrategy
//
//-----------------------------------------------------------------------


// Populate Schedule with itemTimeVecs
void
LgFrCumTimeVecPartScheduleStrategy::kernalOfPopulate(
   LgFrSortingScheduleFloat& schedule )
const
{
  assert( schedules().size() == 1 && 
          "Cumulative Schedule Strategy depends on exactly 1 schedule");

  LgFrPart part;
  LgFrTimeVecFloat tv;
  LgFrItemTimeVecPairFloat itv( part, tv );

  const LgFrSortingScheduleFloat & indepSched = *(schedules()[0]);

  // Loop once for each item in independent schedule
  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i=0; i<indepSched.size(); i++ ) {

     // get next itemTimeVec
     itv = indepSched[i];

     // Form cumulative time vec
     itv.timeVecFloat( itv.timeVecFloat().scan(LgFrAdd) );

     // Check to see if i'th itemTimeVec is to be filtered out
     if ( schedule.filterStrategy().exclude( itv ) ) continue;

     // Insert itemTimeVec into schedule
     schedule.localInsert( itv );
  }

}


// Make clone copy of object
LgFrAttributeScheduleStrategy *
LgFrCumTimeVecPartScheduleStrategy::clone()
const
{
    LgFrCumTimeVecPartScheduleStrategy* temp = 
	new LgFrCumTimeVecPartScheduleStrategy(*this);
    assert( schedules().size() == 1 );
    // hack up the schedules
    // need to deep copy the independent schedules
    size_t i = 0; // Pulled out of the for below by RW2STL
    for ( i=0; i< temp->independentSchedules_.size(); i++ )  {
      // cast away const here to get subscription stuff to work
	temp->independentSchedules_[i]->removeDependentFromHeterogeneousList(temp );
	temp->independentSchedules_[i] = independentSchedules_[i].deepCopy();
	temp->independentSchedules_[i]->addDependent(temp );
    }
    return temp;
}

// Make clone copy of object
LgFrAttributeScheduleStrategy *
LgFrCumTimeVecPartScheduleStrategy::clone(LgFrScenario& newScenario)
const
{  
  return new LgFrCumTimeVecPartScheduleStrategy(*this, newScenario);
}


// Assignment operator
LgFrCumTimeVecPartScheduleStrategy &
LgFrCumTimeVecPartScheduleStrategy::operator=(
	  const LgFrCumTimeVecPartScheduleStrategy& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
    (this->LgFrPartScheduleSensitiveScheduleStrategy::operator= ) (rhs);
  }
  return *this;
}


// Copy constructor
LgFrCumTimeVecPartScheduleStrategy::
   LgFrCumTimeVecPartScheduleStrategy (
      const LgFrCumTimeVecPartScheduleStrategy & source)
:  LgFrPartScheduleSensitiveScheduleStrategy( source ), 
   id_(__LGFRCUMTIMEVECPARTSCHEDULESTRATEGY)
{
}

// Copy constructor with newScenario
LgFrCumTimeVecPartScheduleStrategy::
   LgFrCumTimeVecPartScheduleStrategy (
      const LgFrCumTimeVecPartScheduleStrategy & source,
      LgFrScenario & newScenario)
:  LgFrPartScheduleSensitiveScheduleStrategy( 
         source, 
         newScenario ),
   id_(__LGFRCUMTIMEVECPARTSCHEDULESTRATEGY)
{
}

// Constructor
LgFrCumTimeVecPartScheduleStrategy::
   LgFrCumTimeVecPartScheduleStrategy (
     LgFrScenario & scenario )
:  LgFrPartScheduleSensitiveScheduleStrategy( scenario ),
   id_(__LGFRCUMTIMEVECPARTSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Default Constructor
// This is a private member function.
LgFrCumTimeVecPartScheduleStrategy::
   LgFrCumTimeVecPartScheduleStrategy ()
:  LgFrPartScheduleSensitiveScheduleStrategy(),
   id_(__LGFRCUMTIMEVECPARTSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Destructor
LgFrCumTimeVecPartScheduleStrategy::
   ~LgFrCumTimeVecPartScheduleStrategy ()
{
   // Nothing to do here
}


//------------------------------------------------------------------------
//
//  	Implementation of LgFrCumTimeVecDemandScheduleStrategy
//
//------------------------------------------------------------------------

// Populate Schedule with itemTimeVecs
void
LgFrCumTimeVecDemandScheduleStrategy::kernalOfPopulate(
   LgFrSortingScheduleFloat& schedule )
const
{
  assert( schedules().size() == 1 && 
          "Cumulative Schedule Strategy depends on exactly 1 schedule");

  LgFrDemand part;
  LgFrTimeVecFloat tv;
  LgFrItemTimeVecPairFloat itv( part, tv );

  const LgFrSortingScheduleFloat & indepSched = *(schedules()[0]);

  // Loop once for each item in independent schedule
  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i=0; i<indepSched.size(); i++ ) {

     // get next itemTimeVec
     itv = indepSched[i];

     // Form cumulative time vec
     itv.timeVecFloat( itv.timeVecFloat().scan(LgFrAdd) );

     // Check to see if i'th itemTimeVec is to be filtered out
     if ( schedule.filterStrategy().exclude( itv ) ) continue;

     // Insert itemTimeVec into schedule
     schedule.localInsert( itv );
  }

}


// Make clone copy of object
LgFrAttributeScheduleStrategy *
LgFrCumTimeVecDemandScheduleStrategy::clone()
const
{
    LgFrCumTimeVecDemandScheduleStrategy* temp = 
	new LgFrCumTimeVecDemandScheduleStrategy(*this);
    assert( schedules().size() == 1 );
    // hack up the schedules
    // need to deep copy the independent schedules
    size_t i = 0; // Pulled out of the for below by RW2STL
    for ( i=0; i< temp->independentSchedules_.size(); i++ )  {
      // cast away const here to get subscription stuff to work
	temp->independentSchedules_[i]->removeDependentFromHeterogeneousList(temp );
	temp->independentSchedules_[i] = independentSchedules_[i].deepCopy();
	temp->independentSchedules_[i]->addDependent(temp );
    }
    return temp;
}

// Make clone copy of object
LgFrAttributeScheduleStrategy *
LgFrCumTimeVecDemandScheduleStrategy::clone(LgFrScenario& newScenario)
const
{ 
  return new LgFrCumTimeVecDemandScheduleStrategy(*this, newScenario);
}


// Assignment operator
LgFrCumTimeVecDemandScheduleStrategy &
LgFrCumTimeVecDemandScheduleStrategy::operator=(
	  const LgFrCumTimeVecDemandScheduleStrategy& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
    (this->LgFrDemandScheduleSensitiveScheduleStrategy::operator= ) (rhs);
  }
  return *this;
}


// Copy constructor
LgFrCumTimeVecDemandScheduleStrategy::
   LgFrCumTimeVecDemandScheduleStrategy (
      const LgFrCumTimeVecDemandScheduleStrategy & source)
:  LgFrDemandScheduleSensitiveScheduleStrategy( source ), 
   id_(__LGFRCUMTIMEVECDEMANDSCHEDULESTRATEGY)
{
}


// Copy constructor with newScenario
LgFrCumTimeVecDemandScheduleStrategy::
   LgFrCumTimeVecDemandScheduleStrategy (
      const LgFrCumTimeVecDemandScheduleStrategy & source,
      LgFrScenario & newScenario)
:  LgFrDemandScheduleSensitiveScheduleStrategy( 
         source, 
         newScenario ),
   id_(__LGFRCUMTIMEVECDEMANDSCHEDULESTRATEGY)
{
}

// Constructor
LgFrCumTimeVecDemandScheduleStrategy::
   LgFrCumTimeVecDemandScheduleStrategy (
     LgFrScenario & scenario )
:  LgFrDemandScheduleSensitiveScheduleStrategy( scenario ),
   id_(__LGFRCUMTIMEVECDEMANDSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Default Constructor
// This is a private member function.
LgFrCumTimeVecDemandScheduleStrategy::
   LgFrCumTimeVecDemandScheduleStrategy ()
:  LgFrDemandScheduleSensitiveScheduleStrategy(),
   id_(__LGFRCUMTIMEVECDEMANDSCHEDULESTRATEGY)
{
  // Nothing to do here
}


// Destructor
LgFrCumTimeVecDemandScheduleStrategy::
   ~LgFrCumTimeVecDemandScheduleStrategy ()
{
   // Nothing to do here
}

//-----------------------------------------------------------------------
//
//  Implementation of LgFrCumDemandsPartScheduleStrategy
//
//-----------------------------------------------------------------------


// Populate Schedule with itemTimeVecs
void
LgFrCumDemandsPartScheduleStrategy::kernalOfPopulate(
   LgFrSortingScheduleFloat& schedule )
const
{
  assert( schedules().size() == 1 && 
          "LgFrCumDemandsPartScheduleStrategy depends on exactly 1 schedule");

  size_t i;
  LgFrSortingScheduleFloat & indepSched = (LgFrSortingScheduleFloat &) *(schedules()[0]);
  const LgFrSortingScheduleFloat & constIndepSched = indepSched;

  // Sort independent schedule by part name
  const LgFrPairCompareFloat * const savedPcPtr = indepSched.compareStrategy();
  LgFrPairCompareFloat * myCopySavedPcPtr = 0;
  if ( savedPcPtr != 0 ) myCopySavedPcPtr = savedPcPtr->clone();

  LgFrDemandPartNamePairCompare dpnpc;
  indepSched.sort( &dpnpc );
   
  LgFrDemand demandi;
  LgFrTimeVecFloat tvi;
  LgFrItemTimeVecPairFloat itvi( demandi, tvi );
  
  LgFrPart cumParti;
  LgFrTimeVecFloat cumTv;
  LgFrItemTimeVecPairFloat cumItv( cumParti, cumTv );


  // Loop once for each item in independent schedule
  bool lastIter, firstIter, sameParts;
  for ( i=0;
        i<constIndepSched.size(); 
        i++ ) {

     // get i'th
     itvi = constIndepSched[ i ];

     firstIter = ( i==0 );
     lastIter = ( i==constIndepSched.size()-1 );

     if (firstIter) {
        cumItv.item( *( constIndepSched[0].demand().partPtr() ) );
        cumTv = constIndepSched[0].timeVecFloat();
        cumTv = 0;
        cumItv.timeVecFloat( cumTv ); 
     }

     sameParts = (*(itvi.demand().partPtr()) == cumItv.part()) ? true : false;

     // Check to see if itv should be added to cumItv
     // (ie parts are the same)
     if (  sameParts ) {
        cumItv.timeVecFloat( cumItv.timeVecFloat() + itvi.timeVecFloat() );
     }

     // Check to see if cumItv should be inserted into schedule
     // (ie parts different)
     if (  !sameParts ) {
        if ( ! schedule.filterStrategy().exclude( cumItv ) ) 
           schedule.localInsert( cumItv );
     }

     // Check to see if cumItv should be reinitialized
     // (ie parts different)
     if (  !sameParts ) {
        cumItv.item( *( itvi.demand().partPtr() ) );
        cumItv.timeVecFloat( itvi.timeVecFloat() );
     }

     if ( lastIter ) {
        if ( ! schedule.filterStrategy().exclude( cumItv ) ) 
           schedule.localInsert( cumItv );
     }

  }

  // Restore independent schedule to original sort order
  indepSched.sort( myCopySavedPcPtr );
  delete myCopySavedPcPtr;

}

// Insert independent Schedule
void
LgFrCumDemandsPartScheduleStrategy::insertPartSchedule(
   LgFrSortingPartScheduleFloatSmartPointer  )
{
   std::cerr <<"LgFrCumDemandsPartScheduleStrategy::insertPartSchedule"
        <<std::endl;
   std::cerr <<"Class does not allow inserting part schedules" <<std::endl;
   abort();
}

void
LgFrCumDemandsPartScheduleStrategy::insertDemandSchedule(
   LgFrSortingDemandScheduleFloatSmartPointer schedule )
{
   localInsert( schedule );
}

// Make clone copy of object
LgFrAttributeScheduleStrategy *
LgFrCumDemandsPartScheduleStrategy::clone()
const
{
    LgFrCumDemandsPartScheduleStrategy* temp = 
	new LgFrCumDemandsPartScheduleStrategy(*this);
    assert( schedules().size() == 1 );
    // hack up the schedules
    // need to deep copy the independent schedules
    size_t i = 0; // Pulled out of the for below by RW2STL
    for ( i=0; i< temp->independentSchedules_.size(); i++ )  {
      // cast away const here to get subscription stuff to work
	temp->independentSchedules_[i]->removeDependentFromHeterogeneousList(temp );
	temp->independentSchedules_[i] = independentSchedules_[i].deepCopy();
	temp->independentSchedules_[i]->addDependent(temp );
    }
    return temp;
}

// Make clone copy of object
LgFrAttributeScheduleStrategy *
LgFrCumDemandsPartScheduleStrategy::clone(LgFrScenario& newScenario)
const
{  
  return new LgFrCumDemandsPartScheduleStrategy(*this, newScenario);
}


// Assignment operator
LgFrCumDemandsPartScheduleStrategy &
LgFrCumDemandsPartScheduleStrategy::operator=(
	  const LgFrCumDemandsPartScheduleStrategy& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
    (this->LgFrPartScheduleSensitiveScheduleStrategy::operator= ) (rhs);
  }
  return *this;
}


// Copy constructor
LgFrCumDemandsPartScheduleStrategy::
   LgFrCumDemandsPartScheduleStrategy (
      const LgFrCumDemandsPartScheduleStrategy & source)
:  LgFrPartScheduleSensitiveScheduleStrategy( source ), 
   id_(__LGFRCUMDEMANDSPARTSCHEDULESTRATEGY)
{
}

// Copy constructor with newScenario
LgFrCumDemandsPartScheduleStrategy::
   LgFrCumDemandsPartScheduleStrategy (
      const LgFrCumDemandsPartScheduleStrategy & source,
      LgFrScenario & newScenario)
:  LgFrPartScheduleSensitiveScheduleStrategy( 
         source, 
         newScenario ),
   id_(__LGFRCUMDEMANDSPARTSCHEDULESTRATEGY)
{
}

// Constructor
LgFrCumDemandsPartScheduleStrategy::
   LgFrCumDemandsPartScheduleStrategy (
     LgFrScenario & scenario )
:  LgFrPartScheduleSensitiveScheduleStrategy( scenario ),
   id_(__LGFRCUMDEMANDSPARTSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Default Constructor
// This is a private member function.
LgFrCumDemandsPartScheduleStrategy::
   LgFrCumDemandsPartScheduleStrategy ()
:  LgFrPartScheduleSensitiveScheduleStrategy(),
   id_(__LGFRCUMDEMANDSPARTSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Destructor
LgFrCumDemandsPartScheduleStrategy::
   ~LgFrCumDemandsPartScheduleStrategy ()
{
   // Nothing to do here
}



//-----------------------------------------------------------------------
//
//  Implementation of LgFrPartSummaryScheduleStrategy
//
//-----------------------------------------------------------------------


// Populate Schedule with itemTimeVecs
void
LgFrPartSummaryScheduleStrategy::kernalOfPopulate(
   LgFrSortingScheduleFloat& schedule )
const
{
  assert( schedules().size() == 1 && 
          "PartSummary Schedule Strategy depends on exactly 1 schedule");

  const LgFrSortingScheduleFloat & indepSched = *(schedules()[0]);

  // NOTE!! If the dependent schedule is empty, we should return a timeVec
  // of 0.0's  However, this ASS does not know how many periods there are
  // in the problem.  So, for now, it just returns an empty schedule.
  if (indepSched.size() == 0)
    return;

  LgFrPart part;

  // create a timeVec that has the correct length.  If you don't do this
  // and you cumulate over an empty Schedule, then you're in trouble.
  // Assign initial values of 0.0
//  LgFrTimeVecFloat tv(this->lastProblemSolved().numberOfPeriods(), 0.0);
  LgFrTimeVecFloat tv((indepSched[0]).timeVecFloat().length(), 0.0);
  
  LgFrItemTimeVecPairFloat itv( part, tv );
  
  // Loop once for each item in independent schedule
  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i=0; i<indepSched.size(); i++ ) {

     // get next itemTimeVec
     itv = indepSched[i];

     // add the timeVec
     tv = tv + itv.timeVecFloat();
  }

  // now that you've cumulated all the timeVecs in the dependent
  // schedule into one total timeVec, we need to set each
  // tv in the reultant schedule to that same result
  size_t j = 0; // Pulled out of the for below by RW2STL
  for ( j=0; j<setOfParts().size(); j++ ) {
     part = setOfParts()[j];
     LgFrItemTimeVecPairFloat itv( part, tv );
     
     // Check to see if j'th itemTimeVec is to be filtered out
     if ( schedule.filterStrategy().exclude( itv ) ) continue;

     // Insert itemTimeVec into schedule
     schedule.localInsert( itv );
   }
}


//estimatedSize
size_t
LgFrPartSummaryScheduleStrategy::estimatedSize(
   const LgFrSortingScheduleFloat& schedule )
const
{
  size_t retVal = setOfParts().size();

  if (  schedule.filterStrategy().GetFSType() != __LGFRFILTERSTRATEGY )
     retVal = retVal / 2 + 1;

  retVal = size_tMax(
                    retVal,
                    LgFrAttributeScheduleStrategy::estimatedSize( schedule )
                    );

  return retVal;
}

// Make clone copy of object
LgFrAttributeScheduleStrategy *
LgFrPartSummaryScheduleStrategy::clone()
const
{
    LgFrPartSummaryScheduleStrategy* temp = 
	new LgFrPartSummaryScheduleStrategy(*this);
    assert( schedules().size() == 1 );
    // hack up the schedules
    // need to deep copy the independent schedules
    size_t i = 0; // Pulled out of the for below by RW2STL
    for ( i=0; i< temp->independentSchedules_.size(); i++ )  {
      // cast away const here to get subscription stuff to work
	temp->independentSchedules_[i]->removeDependentFromHeterogeneousList(temp );
	temp->independentSchedules_[i] = independentSchedules_[i].deepCopy();
	temp->independentSchedules_[i]->addDependent(temp );
    }
    return temp;
}

// Make clone copy of object
LgFrAttributeScheduleStrategy *
LgFrPartSummaryScheduleStrategy::clone(LgFrScenario& newScenario)
const
{  
  return new LgFrPartSummaryScheduleStrategy(*this, newScenario);
}


// Assignment operator
LgFrPartSummaryScheduleStrategy &
LgFrPartSummaryScheduleStrategy::operator=(
	  const LgFrPartSummaryScheduleStrategy& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
    (this->LgFrPartScheduleSensitiveScheduleStrategy::operator= ) (rhs);
  }
  return *this;
}


// Copy constructor
LgFrPartSummaryScheduleStrategy::
   LgFrPartSummaryScheduleStrategy (
      const LgFrPartSummaryScheduleStrategy & source)
:  LgFrPartScheduleSensitiveScheduleStrategy( source ), 
   id_(__LGFRPARTSUMMARYSCHEDULESTRATEGY)
{
}

// Copy constructor with newScenario
LgFrPartSummaryScheduleStrategy::
   LgFrPartSummaryScheduleStrategy (
      const LgFrPartSummaryScheduleStrategy & source,
      LgFrScenario & newScenario)
:  LgFrPartScheduleSensitiveScheduleStrategy( 
         source, 
         newScenario ),
   id_(__LGFRPARTSUMMARYSCHEDULESTRATEGY)
{
}

// Constructor
LgFrPartSummaryScheduleStrategy::
   LgFrPartSummaryScheduleStrategy (
     LgFrScenario & scenario )
:  LgFrPartScheduleSensitiveScheduleStrategy( scenario ),
   id_(__LGFRPARTSUMMARYSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Default Constructor
// This is a private member function.
LgFrPartSummaryScheduleStrategy::
   LgFrPartSummaryScheduleStrategy ()
:  LgFrPartScheduleSensitiveScheduleStrategy(),
   id_(__LGFRPARTSUMMARYSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Destructor
LgFrPartSummaryScheduleStrategy::
   ~LgFrPartSummaryScheduleStrategy ()
{
   // Nothing to do here
}



//-----------------------------------------------------------------------
//
//  Implementation of LgFrDemandSummaryScheduleStrategy
//
//-----------------------------------------------------------------------


// Populate Schedule with itemTimeVecs
void
LgFrDemandSummaryScheduleStrategy::kernalOfPopulate(
   LgFrSortingScheduleFloat& schedule )
const
{
  assert( schedules().size() == 1 && 
          "DemandSummary Schedule Strategy depends on exactly 1 schedule");

  const LgFrSortingScheduleFloat & indepSched = *(schedules()[0]);

  // NOTE!! If the dependent schedule is empty, we should return a timeVec
  // of 0.0's  However, this ASS does not know how many periods there are
  // in the problem.  So, for now, it just returns an empty schedule.
  if (indepSched.size() == 0)
    return;

  LgFrDemand demand;

  // create a timeVec that has the correct length.  If you don't do this
  // and you cumulate over an empty Schedule, then you're in trouble.
  // Assign initial values of 0.0
//  LgFrTimeVecFloat tv(this->lastProblemSolved().numberOfPeriods(), 0.0);
  LgFrTimeVecFloat tv((indepSched[0]).timeVecFloat().length(), 0.0);
  
  LgFrItemTimeVecPairFloat itv( demand, tv );
  
  // Loop once for each item in independent schedule
  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i=0; i<indepSched.size(); i++ ) {

     // get next itemTimeVec
     itv = indepSched[i];

     // add the timeVec
     tv = tv + itv.timeVecFloat();
  }

  // now that you've cumulated all the timeVecs in the dependent
  // schedule into one total timeVec, we need to set each
  // tv in the reultant schedule to that same result
  size_t j = 0; // Pulled out of the for below by RW2STL
  for ( j=0; j<setOfDemands().size(); j++ ) {
     demand = setOfDemands()[j];
     LgFrItemTimeVecPairFloat itv( demand, tv );
     
     // Check to see if j'th itemTimeVec is to be filtered out
     if ( schedule.filterStrategy().exclude( itv ) ) continue;

     // Insert itemTimeVec into schedule
     schedule.localInsert( itv );
   }
}


//estimatedSize
size_t
LgFrDemandSummaryScheduleStrategy::estimatedSize(
   const LgFrSortingScheduleFloat& schedule )
const
{
  size_t retVal = setOfDemands().size();

  if (  schedule.filterStrategy().GetFSType() != __LGFRFILTERSTRATEGY )
     retVal = retVal / 2 + 1;

  retVal = size_tMax(
                    retVal,
                    LgFrAttributeScheduleStrategy::estimatedSize( schedule )
                    );

  return retVal;
}

// Make clone copy of object
LgFrAttributeScheduleStrategy *
LgFrDemandSummaryScheduleStrategy::clone()
const
{
    LgFrDemandSummaryScheduleStrategy* temp = 
	new LgFrDemandSummaryScheduleStrategy(*this);
    assert( schedules().size() == 1 );
    // hack up the schedules
    // need to deep copy the independent schedules
    size_t i = 0; // Pulled out of the for below by RW2STL
    for ( i=0; i< temp->independentSchedules_.size(); i++ )  {
      // cast away const here to get subscription stuff to work
	temp->independentSchedules_[i]->removeDependentFromHeterogeneousList(temp );
	temp->independentSchedules_[i] = independentSchedules_[i].deepCopy();
	temp->independentSchedules_[i]->addDependent(temp );
    }
    return temp;
}

// Make clone copy of object
LgFrAttributeScheduleStrategy *
LgFrDemandSummaryScheduleStrategy::clone(LgFrScenario& newScenario)
const
{  
  return new LgFrDemandSummaryScheduleStrategy(*this, newScenario);
}


// Assignment operator
LgFrDemandSummaryScheduleStrategy &
LgFrDemandSummaryScheduleStrategy::operator=(
	  const LgFrDemandSummaryScheduleStrategy& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
    (this->LgFrDemandScheduleSensitiveScheduleStrategy::operator= ) (rhs);
  }
  return *this;
}


// Copy constructor
LgFrDemandSummaryScheduleStrategy::
   LgFrDemandSummaryScheduleStrategy (
      const LgFrDemandSummaryScheduleStrategy & source)
:  LgFrDemandScheduleSensitiveScheduleStrategy( source ), 
   id_(__LGFRDEMANDSUMMARYSCHEDULESTRATEGY)
{
}

// Copy constructor with newScenario
LgFrDemandSummaryScheduleStrategy::
   LgFrDemandSummaryScheduleStrategy (
      const LgFrDemandSummaryScheduleStrategy & source,
      LgFrScenario & newScenario)
:  LgFrDemandScheduleSensitiveScheduleStrategy( 
         source, 
         newScenario ),
   id_(__LGFRDEMANDSUMMARYSCHEDULESTRATEGY)
{
}

// Constructor
LgFrDemandSummaryScheduleStrategy::
   LgFrDemandSummaryScheduleStrategy (
     LgFrScenario & scenario )
:  LgFrDemandScheduleSensitiveScheduleStrategy( scenario ),
   id_(__LGFRDEMANDSUMMARYSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Default Constructor
// This is a private member function.
LgFrDemandSummaryScheduleStrategy::
   LgFrDemandSummaryScheduleStrategy ()
:  LgFrDemandScheduleSensitiveScheduleStrategy(),
   id_(__LGFRDEMANDSUMMARYSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Destructor
LgFrDemandSummaryScheduleStrategy::
   ~LgFrDemandSummaryScheduleStrategy ()
{
   // Nothing to do here
}

//-----------------------------------------------------------------------
//
//  Implementation of LgFrCompoundInterestScheduleStrategy
//
//-----------------------------------------------------------------------


// Populate Schedule with itemTimeVecs
void
LgFrCompoundInterestScheduleStrategy::kernalOfPopulate(
   LgFrSortingScheduleFloat& schedule )
const
{
  assert( schedules().size() == 1 && 
          "Cumulative Schedule Strategy depends on exactly 1 schedule");

  LgFrPart part;
  LgFrTimeVecFloat tv;
  LgFrItemTimeVecPairFloat itv( part, tv );

  const LgFrSortingScheduleFloat & indepSched = *(schedules()[0]);

  // Loop once for each item in independent schedule
  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i=0; i<indepSched.size(); i++ ) {

     // get next itemTimeVec from dependent schedule
     // NOTE: dependent schedule should represent a dollar savings schedule
     itv = indepSched[i];
     
     // Form cumulative compound interest time vec
     // initial tv of zero's
     LgFrTimeVecFloat ciTV(itv.timeVecFloat().length(), 0.0);

     
     size_t t = 0; // Pulled out of the for below by RW2STL
     for (t=0; t<itv.timeVecFloat().length(); t++) {
       if (t == 0)  {
          // interest earned in period 0
          ciTV[t] = (itv.timeVecFloat())[t] * interestRate_ / 52.0f;
        }
        else  {
#define COMPOUND_INTEREST
#ifdef COMPOUND_INTEREST
          // interest earned in period t > 0 is the interest on whatever savings
          // were in period t plus the interest on the interest earned in period
          // t-1.  
          ciTV[t] = ((itv.timeVecFloat())[t] + ciTV[t-1]) * interestRate_ / 52.0f;
          // now cum the interest
          ciTV[t] += ciTV[t-1];
#else
          ciTV[t] = ((itv.timeVecFloat())[t]) * interestRate_ / 52.0;
#endif
        }
      }     
#ifndef COMPOUND_INTEREST
     // now cum the interest
     for (t=1; t<itv.timeVecFloat().length(); t++) {
       ciTV[t] += ciTV[t-1];
     }
#endif

     itv.timeVecFloat(ciTV);

     // Check to see if i'th itemTimeVec is to be filtered out
     if ( schedule.filterStrategy().exclude( itv ) ) continue;

     // Insert itemTimeVec into schedule
     schedule.localInsert( itv );
  }

}


// set/get the interest rate
void
LgFrCompoundInterestScheduleStrategy::interestRate(float theRate)
{
  interestRate_ = theRate;
}

float
LgFrCompoundInterestScheduleStrategy::interestRate()
     const
{
  return interestRate_;
}

// Make clone copy of object
LgFrAttributeScheduleStrategy *
LgFrCompoundInterestScheduleStrategy::clone()
const
{
    LgFrCompoundInterestScheduleStrategy* temp = 
	new LgFrCompoundInterestScheduleStrategy(*this);
    assert( schedules().size() == 1 );
    // hack up the schedules
    // need to deep copy the independent schedules
    size_t i = 0; // Pulled out of the for below by RW2STL
    for ( i=0; i< temp->independentSchedules_.size(); i++ )  {
      // cast away const here to get subscription stuff to work
	temp->independentSchedules_[i]->removeDependentFromHeterogeneousList(temp );
	temp->independentSchedules_[i] = independentSchedules_[i].deepCopy();
	temp->independentSchedules_[i]->addDependent(temp );
    }
    return temp;
}

// Make clone copy of object
LgFrAttributeScheduleStrategy *
LgFrCompoundInterestScheduleStrategy::clone(LgFrScenario& newScenario)
const
{
  return new LgFrCompoundInterestScheduleStrategy(*this, newScenario);
}


// Assignment operator
LgFrCompoundInterestScheduleStrategy &
LgFrCompoundInterestScheduleStrategy::operator=(
	  const LgFrCompoundInterestScheduleStrategy& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
    (this->LgFrPartScheduleSensitiveScheduleStrategy::operator= ) (rhs);
    this->interestRate_ = rhs.interestRate_;
  }
  return *this;
}


// Copy constructor
LgFrCompoundInterestScheduleStrategy::
   LgFrCompoundInterestScheduleStrategy (
      const LgFrCompoundInterestScheduleStrategy & source)
:  LgFrPartScheduleSensitiveScheduleStrategy( source ),
   interestRate_(source.interestRate_),
   id_(__LGFRCOMPOUNDINTERESTSCHEDULESTRATEGY)
{
}


// Copy constructor with newScenario
LgFrCompoundInterestScheduleStrategy::
   LgFrCompoundInterestScheduleStrategy (
      const LgFrCompoundInterestScheduleStrategy & source,
      LgFrScenario & newScenario)
:  LgFrPartScheduleSensitiveScheduleStrategy( 
         source, 
         newScenario ),
   interestRate_(source.interestRate_),
   id_(__LGFRCOMPOUNDINTERESTSCHEDULESTRATEGY)
{
}

// Constructor
LgFrCompoundInterestScheduleStrategy::
   LgFrCompoundInterestScheduleStrategy (
     LgFrScenario & scenario )
:  LgFrPartScheduleSensitiveScheduleStrategy( scenario ),
   interestRate_(0.10f),
   id_(__LGFRCOMPOUNDINTERESTSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Default Constructor
// This is a private member function.
LgFrCompoundInterestScheduleStrategy::
   LgFrCompoundInterestScheduleStrategy ()
:  LgFrPartScheduleSensitiveScheduleStrategy(),
   interestRate_(0.10f),
   id_(__LGFRCOMPOUNDINTERESTSCHEDULESTRATEGY)
{
  // Nothing to do here
}

// Destructor
LgFrCompoundInterestScheduleStrategy::
   ~LgFrCompoundInterestScheduleStrategy ()
{
   // Nothing to do here
}


//-------------------------------------------------------------------------
//
//		Test Methods
//
//-------------------------------------------------------------------------


//-----------------------------------------------------------------------
//
//  Test of LgFrCumTimeVecPartScheduleStrategy
//
//-----------------------------------------------------------------------

#ifdef NDEBUG
#undef NDEBUG
#endif

// test LgFrCumTimeVecPartScheduleStrategy methods.

void
LgFrCumTimeVecPartScheduleStrategy::test()
{
  LgFrCumTimeVecPartScheduleStrategy cs1;
  assert( cs1.schedules().size() == 0 );
}


// test LgFrCumTimeVecPartScheduleStrategy methods.
// Expects testScenario to contain data from
// /u/implode/wit/data/examples/diner12/wit.data
// Expects supplySchedule to be a supply schedule for testScenario
// (containing a LgFrSupplyScheduleStrategy)
void
LgFrCumTimeVecPartScheduleStrategy::contextTest
    (
     LgFrScenario & testScenario,
     LgFrSortingPartScheduleFloatSmartPointer supplySchedule    
     )
{
std::cerr << "entering LgFrCumTimeVecPartScheduleStrategy::contextTest" << std::endl;
  int i;
  LgFrCumTimeVecPartScheduleStrategy cs1;

  // Test inserting single dependent schedule
  cs1.insertPartSchedule( supplySchedule );
  assert( cs1.schedules()[0] == supplySchedule );

  // Test copy constructor - shallow copy
  LgFrCumTimeVecPartScheduleStrategy cs2(cs1);
  assert( cs2.schedules()[0] == supplySchedule );

  // Test assignment operator - shallow copy
  LgFrCumTimeVecPartScheduleStrategy cs3;
  cs3 = cs2;
  assert( cs3.schedules()[0] == supplySchedule );


  // Test clone method
  {
    LgFrAttributeScheduleStrategy *cs4Ptr = cs1.clone();
    assert( (  (LgFrCumTimeVecPartScheduleStrategy *) cs4Ptr)
            ->schedules().size() == 1 );
    assert( (  (LgFrCumTimeVecPartScheduleStrategy *) cs4Ptr)
            ->schedules()[0] 
            != supplySchedule );
    assert( *( (  (LgFrCumTimeVecPartScheduleStrategy *) cs4Ptr)
            ->schedules()[0])
            == *supplySchedule );
    
    // Test destructor
    delete cs4Ptr;
  }


  // Test Kernal of populate
  const LgFrSortingPartScheduleFloat & constSupplySchedule = *supplySchedule;
  LgFrSortingPartScheduleFloat cumSup;
  const LgFrSortingPartScheduleFloat & constCumSup = cumSup;
  cumSup.attributeScheduleStrategy( cs1 );
  cumSup.populate();
  for ( i=0; i<supplySchedule->size(); i++ ) 
     assert(         constCumSup[i].timeVecFloat() == 
             constSupplySchedule[i].timeVecFloat().scan(LgFrAdd) );


  // Test subscription mechanism
  LgFrPart part2 = constSupplySchedule[2].part();
  supplySchedule->insertKeyAndValue( part2, LgFrTimeVecFloat(2, 3.1415f) );
  for ( i=0; i<supplySchedule->size(); i++ ) 
     assert(         constCumSup[i].timeVecFloat() == 
             constSupplySchedule[i].timeVecFloat().scan(LgFrAdd) );

  // Test scenario copy constructor - deep copy
  LgFrCumTimeVecPartScheduleStrategy scenCtor(cs1,testScenario);
  assert( scenCtor.schedules().size() ==  
               cs1.schedules().size() );

  // Test cloning w/scenario - deep copy
  LgFrAttributeScheduleStrategy *cs6Ptr = cs1.clone(testScenario);
  assert( ((LgFrCumTimeVecPartScheduleStrategy *)cs6Ptr)->schedules()[0] 
          != supplySchedule );
  assert( ((LgFrCumTimeVecPartScheduleStrategy *)cs6Ptr)->schedules()[0] 
          != cs1.schedules()[0] );
  // schedules have the same content but are deep copies
  assert( *(((LgFrCumTimeVecPartScheduleStrategy *)cs6Ptr)->schedules()[0])
          == *(cs1.schedules()[0]) );
  delete cs6Ptr;
}

//-----------------------------------------------------------------------
//
//  Test of LgFrCumTimeVecDemandScheduleStrategy
//
//-----------------------------------------------------------------------


// test LgFrCumTimeVecDemandScheduleStrategy methods.

void
LgFrCumTimeVecDemandScheduleStrategy::test()
{
  LgFrCumTimeVecDemandScheduleStrategy cs1;
  assert( cs1.schedules().size() == 0 );
}


// test LgFrCumTimeVecDemandScheduleStrategy methods.
// Expects testScenario to contain data from
// /u/implode/wit/data/examples/diner12/wit.data
// Expects supplySchedule to be a supply schedule for testScenario
// (containing a LgFrSupplyScheduleStrategy)
void
LgFrCumTimeVecDemandScheduleStrategy::contextTest
    (
     LgFrScenario & testScenario,
     LgFrSortingDemandScheduleFloatSmartPointer supplySchedule    
     )
{
std::cerr << "entering LgFrCumTimeVecDemandScheduleStrategy::contextTest" << std::endl;
  int i;
  LgFrCumTimeVecDemandScheduleStrategy cs1;

  // Test inserting single dependent schedule
  cs1.insertDemandSchedule( supplySchedule );
  assert( cs1.schedules()[0] == supplySchedule );

  // Test copy constructor - shallow copy of schedules
  LgFrCumTimeVecDemandScheduleStrategy cs2(cs1);
  assert( cs2.schedules()[0] == supplySchedule );

  // Test assignment operator - shallow copy of schedules
  LgFrCumTimeVecDemandScheduleStrategy cs3;
  cs3 = cs2;
  assert( cs3.schedules()[0] == supplySchedule );

  // Test clone method - deep copy of schedules
  {
    LgFrAttributeScheduleStrategy *cs4Ptr = cs1.clone();
    assert( (  (LgFrCumTimeVecDemandScheduleStrategy *) cs4Ptr)
            ->schedules().size() == 1 );
    assert( (  (LgFrCumTimeVecDemandScheduleStrategy *) cs4Ptr)
            ->schedules()[0] 
            != supplySchedule );
    assert( *( (  (LgFrCumTimeVecDemandScheduleStrategy *) cs4Ptr)
            ->schedules()[0])
            == *supplySchedule );
    
    // Test destructor
    delete cs4Ptr;
  }


  // Test Kernal of populate
  const LgFrSortingDemandScheduleFloat & constSupplySchedule = *supplySchedule;
  LgFrSortingDemandScheduleFloat cumSup;
  const LgFrSortingDemandScheduleFloat & constCumSup = cumSup;
  cumSup.attributeScheduleStrategy( cs1 );
  cumSup.populate();
  for ( i=0; i<supplySchedule->size(); i++ ) 
     assert(         constCumSup[i].timeVecFloat() == 
             constSupplySchedule[i].timeVecFloat().scan(LgFrAdd) );

  // Test subscription mechanism
  LgFrDemand part2 = constSupplySchedule[2].demand();
  supplySchedule->insertKeyAndValue( part2, LgFrTimeVecFloat(2, 3.1415f) );
  for ( i=0; i<supplySchedule->size(); i++ ) 
     assert(         constCumSup[i].timeVecFloat() == 
             constSupplySchedule[i].timeVecFloat().scan(LgFrAdd) );

  // Test scenario copy constructor
  LgFrCumTimeVecDemandScheduleStrategy scenCtor(cs1,testScenario);
  assert( scenCtor.schedules().size() ==  
               cs1.schedules().size() );


  // Test cloning w/scenario 
  LgFrAttributeScheduleStrategy *cs6Ptr = cs1.clone(testScenario);
  assert( ((LgFrCumTimeVecDemandScheduleStrategy *)cs6Ptr)->schedules()[0] 
          != supplySchedule );
  assert( ((LgFrCumTimeVecDemandScheduleStrategy *)cs6Ptr)->schedules()[0] 
          != cs1.schedules()[0] );
  assert( *(((LgFrCumTimeVecDemandScheduleStrategy *)cs6Ptr)->schedules()[0])
          == *(cs1.schedules()[0]) );

  delete cs6Ptr;
}

//-----------------------------------------------------------------------
//
//  Test of LgFrCumDemandsPartScheduleStrategy
//
//-----------------------------------------------------------------------


#ifdef NDEBUG
#undef NDEBUG
#endif
// test LgFrCumDemandsPartScheduleStrategy methods.
void
LgFrCumDemandsPartScheduleStrategy::test()
{
  LgFrCumDemandsPartScheduleStrategy cs1;
  assert( cs1.schedules().size() == 0 );
}


// test LgFrCumDemandsPartScheduleStrategy methods.
// Expects testScenario to contain data from
// /u/implode/wit/data/brenda/denise/wit.data
// Expects supplySchedule to be a supply schedule for testScenario
// (containing a LgFrSupplyScheduleStrategy)
void
LgFrCumDemandsPartScheduleStrategy::contextTest
    (
     LgFrScenario & testScenario,
     LgFrSortingDemandScheduleFloatSmartPointer demandSchedule    
     )
{
std::cerr << "entering LgFrCumDemandsPartScheduleStrategy::contextTest" << std::endl;
  LgFrCumDemandsPartScheduleStrategy cs1;


  // Test inserting single dependent schedule
  cs1.insertDemandSchedule( demandSchedule );
  assert( cs1.schedules()[0] == demandSchedule );

  // Test copy constructor
  LgFrCumDemandsPartScheduleStrategy cs2(cs1);
  assert( cs2.schedules()[0] == demandSchedule );

  // Test assignment operator
  LgFrCumDemandsPartScheduleStrategy cs3;
  cs3 = cs2;
  assert( cs3.schedules()[0] == demandSchedule );

  // Test clone method
  {
    LgFrAttributeScheduleStrategy *cs4Ptr = cs1.clone();
    assert( (  (LgFrCumDemandsPartScheduleStrategy *) cs4Ptr)
            ->schedules().size() == 1 );
    assert( (  (LgFrCumDemandsPartScheduleStrategy *) cs4Ptr)
            ->schedules()[0] 
            != demandSchedule );

    assert( *( (  (LgFrCumDemandsPartScheduleStrategy *) cs4Ptr)
            ->schedules()[0])
            == *demandSchedule );
    
    // Test destructor
    delete cs4Ptr;
  }


  // Test Kernal of populate
  const LgFrSortingDemandScheduleFloat & constDemandSchedule = *demandSchedule;
  LgFrSortingPartScheduleFloat cum;
  const LgFrSortingPartScheduleFloat & constCum = cum;
  cum.attributeScheduleStrategy( cs1 );
  cum.populate();

  assert( demandSchedule->size() > cum.size() );


  // Test scenario copy constructor
  LgFrCumDemandsPartScheduleStrategy scenCtor(cs1,testScenario);
  assert( scenCtor.schedules().size() ==  
               cs1.schedules().size() );


  // Test cloning w/scenario 
  LgFrAttributeScheduleStrategy *cs6Ptr = cs1.clone(testScenario);
  assert( ((LgFrCumDemandsPartScheduleStrategy *)cs6Ptr)->schedules()[0] 
          != demandSchedule );
  assert( ((LgFrCumDemandsPartScheduleStrategy *)cs6Ptr)->schedules()[0] 
          != cs1.schedules()[0] );
  assert( *(((LgFrCumDemandsPartScheduleStrategy *)cs6Ptr)->schedules()[0])
          == *(cs1.schedules()[0]) );

  delete cs6Ptr;
}


//-----------------------------------------------------------------------
//
//  Test of LgFrPartSummaryScheduleStrategy
//
//-----------------------------------------------------------------------

#ifdef NDEBUG
#undef NDEBUG
#endif

// test LgFrPartSummaryScheduleStrategy methods.

void
LgFrPartSummaryScheduleStrategy::test()
{
  LgFrPartSummaryScheduleStrategy cs1;
  assert( cs1.schedules().size() == 0 );
}


// test LgFrPartSummaryScheduleStrategy methods.
// Expects testScenario to contain data from
// /u/implode/wit/data/examples/diner12/wit.data
// Expects supplySchedule to be a supply schedule for testScenario
// (containing a LgFrSupplyScheduleStrategy)
void
LgFrPartSummaryScheduleStrategy::contextTest
    (
     LgFrScenario & testScenario,
     LgFrSortingPartScheduleFloatSmartPointer supplySchedule    
     )
{
std::cerr << "entering LgFrPartSummaryScheduleStrategy::contextTest" << std::endl;
  int i;
  float fltEps = 0.0001f;
  LgFrPartSummaryScheduleStrategy cs1(testScenario);

  // Test inserting single dependent schedule
  cs1.insertPartSchedule( supplySchedule );
  assert( cs1.schedules()[0] == supplySchedule );

  // Test copy constructor
  LgFrPartSummaryScheduleStrategy cs2(cs1);
  assert( cs2.schedules()[0] == supplySchedule );

  // Test assignment operator
  LgFrPartSummaryScheduleStrategy cs3(testScenario);
  cs3 = cs2;
  assert( cs3.schedules()[0] == supplySchedule );

  // Test clone method
  {
    LgFrAttributeScheduleStrategy *cs4Ptr = cs1.clone();
    assert( (  (LgFrPartSummaryScheduleStrategy *) cs4Ptr)
            ->schedules().size() == 1 );
    assert( (  (LgFrPartSummaryScheduleStrategy *) cs4Ptr)
            ->schedules()[0] 
            != supplySchedule );
    assert( *( (  (LgFrPartSummaryScheduleStrategy *) cs4Ptr)
            ->schedules()[0])
            == *supplySchedule );
    
    // Test destructor
    delete cs4Ptr;
  }


  // Test Kernal of populate
  const LgFrSortingPartScheduleFloat & constSupplySchedule = *supplySchedule;
  LgFrSortingPartScheduleFloat cumSup;
  const LgFrSortingPartScheduleFloat & constCumSup = cumSup;
  cumSup.attributeScheduleStrategy( cs1 );
  cumSup.populate();
  for ( i=0; i<cumSup.size(); i++ ) {
    assert( fabs( (constCumSup[i].timeVecFloat())[0] - 158.0) < fltEps);
    assert( fabs( (constCumSup[i].timeVecFloat())[1] - 137.0) < fltEps);
   }

  // Test subscription mechanism
  LgFrPart part00 = constSupplySchedule[0].part();
  supplySchedule->insertKeyAndValue( part00, LgFrTimeVecFloat(2, "5.0 10.0") );
  for ( i=0; i<cumSup.size(); i++ )   {
    assert( fabs( (constCumSup[i].timeVecFloat())[0] - 148.0) < fltEps);
    assert( fabs( (constCumSup[i].timeVecFloat())[1] - 127.0) < fltEps);
  }

  // Test scenario copy constructor
  LgFrPartSummaryScheduleStrategy scenCtor(cs1,testScenario);
  assert( scenCtor.schedules().size() ==  
               cs1.schedules().size() );

  // Test cloning w/scenario 
  LgFrAttributeScheduleStrategy *cs6Ptr = cs1.clone(testScenario);
  assert( ((LgFrPartSummaryScheduleStrategy *)cs6Ptr)->schedules()[0] 
          != supplySchedule );
  assert( ((LgFrPartSummaryScheduleStrategy *)cs6Ptr)->schedules()[0] 
          != cs1.schedules()[0] );
  assert( *(((LgFrPartSummaryScheduleStrategy *)cs6Ptr)->schedules()[0])
          == *(cs1.schedules()[0]) );

  delete cs6Ptr;

  // test creating the sum over an empty schedule
  LgFrSortingPartScheduleFloatSmartPointer emptySched;
  assert(emptySched->size() == 0);
  LgFrPartSummaryScheduleStrategy cs8(testScenario);

  // Test inserting single dependent schedule
  cs8.insertPartSchedule( emptySched );

  LgFrSortingPartScheduleFloat sumOfNothing;
  sumOfNothing.attributeScheduleStrategy( cs8 );
  sumOfNothing.populate();
  assert(sumOfNothing.size() == 0);


}

//-----------------------------------------------------------------------
//
//  Test of LgFrDemandSummaryScheduleStrategy
//
//-----------------------------------------------------------------------

#ifdef NDEBUG
#undef NDEBUG
#endif

// test LgFrDemandSummaryScheduleStrategy methods.

void
LgFrDemandSummaryScheduleStrategy::test()
{
  LgFrDemandSummaryScheduleStrategy cs1;
  assert( cs1.schedules().size() == 0 );
}


// test LgFrDemandSummaryScheduleStrategy methods.
// Expects testScenario to contain data from
// /u/implode/wit/data/examples/diner12/wit.data
// Expects demandVolSchedule to be a demandVol schedule for testScenario
// (containing a LgFrDemandVolScheduleStrategy)
void
LgFrDemandSummaryScheduleStrategy::contextTest
    (
     LgFrScenario & testScenario,
     LgFrSortingDemandScheduleFloatSmartPointer demandVolSchedule    
     )
{
std::cerr << "entering LgFrDemandSummaryScheduleStrategy::contextTest" << std::endl;
  int i;
  float fltEps = 0.0001f;
  LgFrDemandSummaryScheduleStrategy cs1(testScenario);

  // Test inserting single dependent schedule
  cs1.insertDemandSchedule( demandVolSchedule );
  assert( cs1.schedules()[0] == demandVolSchedule );

  // Test copy constructor
  LgFrDemandSummaryScheduleStrategy cs2(cs1);
  assert( cs2.schedules()[0] == demandVolSchedule );

  // Test assignment operator
  LgFrDemandSummaryScheduleStrategy cs3(testScenario);
  cs3 = cs2;
  assert( cs3.schedules()[0] == demandVolSchedule );

  // Test clone method
  {
    LgFrAttributeScheduleStrategy *cs4Ptr = cs1.clone();
    assert( (  (LgFrDemandSummaryScheduleStrategy *) cs4Ptr)
            ->schedules().size() == 1 );
    assert( (  (LgFrDemandSummaryScheduleStrategy *) cs4Ptr)
            ->schedules()[0] 
            != demandVolSchedule );
    assert( *( (  (LgFrDemandSummaryScheduleStrategy *) cs4Ptr)
            ->schedules()[0])
            == *demandVolSchedule );
    
    // Test destructor
    delete cs4Ptr;
  }


  // Test Kernal of populate
  const LgFrSortingDemandScheduleFloat & constDemandVolSchedule = *demandVolSchedule;
  LgFrSortingDemandScheduleFloat cumDem;
  const LgFrSortingDemandScheduleFloat & constCumDem = cumDem;
  cumDem.attributeScheduleStrategy( cs1 );
  cumDem.populate();
  for ( i=0; i<cumDem.size(); i++ ) {
    assert( fabs( (constCumDem[i].timeVecFloat())[0] - 25.0) < fltEps);
    assert( fabs( (constCumDem[i].timeVecFloat())[1] - 29.0) < fltEps);
   }

  // Test subscription mechanism
  LgFrDemand demand00 = constDemandVolSchedule[0].demand();
  demandVolSchedule->insertKeyAndValue( demand00, LgFrTimeVecFloat(2, "3.0 6.0") );
  for ( i=0; i<cumDem.size(); i++ )   {
    assert( fabs( (constCumDem[i].timeVecFloat())[0] - 28.0) < fltEps);
    assert( fabs( (constCumDem[i].timeVecFloat())[1] - 35.0) < fltEps);
  }

  // Test scenario copy constructor
  LgFrDemandSummaryScheduleStrategy scenCtor(cs1,testScenario);
  assert( scenCtor.schedules().size() ==  
               cs1.schedules().size() );

  // Test cloning w/scenario 
  LgFrAttributeScheduleStrategy *cs6Ptr = cs1.clone(testScenario);
  assert( ((LgFrDemandSummaryScheduleStrategy *)cs6Ptr)->schedules()[0] 
          != demandVolSchedule );
  assert( ((LgFrDemandSummaryScheduleStrategy *)cs6Ptr)->schedules()[0] 
          != cs1.schedules()[0] );
  assert( *(((LgFrDemandSummaryScheduleStrategy *)cs6Ptr)->schedules()[0])
          == *(cs1.schedules()[0]) );

  delete cs6Ptr;

  // test creating the sum over an empty schedule
  LgFrSortingDemandScheduleFloatSmartPointer emptySched;
  assert(emptySched->size() == 0);
  LgFrDemandSummaryScheduleStrategy cs8(testScenario);

  // Test inserting single dependent schedule
  cs8.insertDemandSchedule( emptySched );

  LgFrSortingDemandScheduleFloat sumOfNothing;
  sumOfNothing.attributeScheduleStrategy( cs8 );
  sumOfNothing.populate();
  assert(sumOfNothing.size() == 0);


}
