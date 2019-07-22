// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL -- inserted includes.
#include <scenario/src/wrkgProb.h>

#include <math.h>
#include <assert.h>

// Include file needed for testing
#include <scenario/src/machdep.h>
#include <scenario/src/usrInMod.h>

// Usefull constants
const int originalSchedule = 0;		// Used for accessing partSchedules
const int latestSchedule = 1;		// Used for accessing partSchedules
const int nSchedules = 2;		// Number of part-schedules


int
LgFrWorkingImplosionProblem::isValid()
     const
{
  return (lpsPtr_ != 0);	// If it is null, *this is not a valid
  				// instance, except for assignment
}


// Return a list of all parts that have been changed since this problem was
// initialized
LgFrPartVector
LgFrWorkingImplosionProblem::changedParts()
     const
{
  assert (this-> isValid());
  // Construct an ordered vector capable of holding the number of parts
  // that have been changed.
  size_t nChangedParts = partSchedules_[latestSchedule].size();
  LgFrPartVector partVec;
  partVec.reserve( nChangedParts );//size_tMax(1,nChangedParts) );
     // size_tMax used to avoid suspected problem with purify or RW

  // Fill the vector in
  int i = 0; // Pulled out of the for below by RW2STL robot
  for (i = 0;  i < nChangedParts;  ++i) {
    partVec.push_back ( partSchedules_[latestSchedule][i].part() );
  }

  return partVec;
}

// Return a list of all parts that have been changed since this problem was
// initialized
LgFrPartVector
LgFrWorkingImplosionProblem::changedPartsCycleTime()
     const
{
  assert (this-> isValid());
  // Construct an ordered vector capable of holding the number of parts
  // that have been changed.
  size_t nChangedParts = cycleTimeSchedules_[latestSchedule].size();
  LgFrPartVector partVec;
  partVec.reserve( nChangedParts );//size_tMax(1,nChangedParts) );
     // size_tMax used to avoid suspected problem with purify or RW

  // Fill the vector in
  int i = 0; // Pulled out of the for below by RW2STL robot
  for (i = 0;  i < nChangedParts;  ++i) {
    partVec.push_back ( cycleTimeSchedules_[latestSchedule][i].part() );
  }

  return partVec;
}


// Return a list of all demands that have had their demandVol
// changed since this problem was initialized
LgFrDemandVector
LgFrWorkingImplosionProblem::changedDemands()
     const
{
  assert (this-> isValid());
  // Construct an ordered vector capable of holding the number of demands
  // that have been changed.
  size_t nChangedDemands = demandSchedules_[latestSchedule].size();
  LgFrDemandVector demandVec;
  demandVec.reserve( nChangedDemands );//size_tMax(1,nChangedDemands) );
     // size_tMax used to avoid suspected problem with purify or RW

  // Fill the vector in
  int i = 0; // Pulled out of the for below by RW2STL robot
  for (i = 0;  i < nChangedDemands;  ++i) {
    demandVec.push_back ( demandSchedules_[latestSchedule][i].demand() );
  }

  return demandVec;
}

// Return a list of all demands that have had their priorities
// changed since this problem was initialized
LgFrDemandVector
LgFrWorkingImplosionProblem::changedDemandsPriority()
     const
{
  assert (this-> isValid());
  // Construct an ordered vector capable of holding the number of demands
  // that have been changed.
  size_t nChangedDemands = prioritySchedules_[latestSchedule].size();
  LgFrDemandVector demandVec;
  demandVec.reserve( nChangedDemands );//size_tMax(1,nChangedDemands) );
     // size_tMax used to avoid suspected problem with purify or RW

  // Fill the vector in
  int i = 0; // Pulled out of the for below by RW2STL robot
  for (i = 0;  i < nChangedDemands;  ++i) {
    demandVec.push_back ( prioritySchedules_[latestSchedule][i].demand() );
  }

  return demandVec;
}


// Return a list of all demands that have had their obj1ShipRewards
// changed since this problem was initialized
LgFrDemandVector
LgFrWorkingImplosionProblem::changedDemandsObj1ShipReward()
     const
{
  assert (this-> isValid());
  // Construct an ordered vector capable of holding the number of demands
  // that have been changed.
  size_t nChangedDemands = obj1ShipRewardSchedules_[latestSchedule].size();
  LgFrDemandVector demandVec;
  demandVec.reserve( nChangedDemands );//size_tMax(1,nChangedDemands) );
     // size_tMax used to avoid suspected problem with purify or RW

  // Fill the vector in
  int i = 0; // Pulled out of the for below by RW2STL robot
  for (i = 0;  i < nChangedDemands;  ++i) {
    demandVec.push_back ( obj1ShipRewardSchedules_[latestSchedule][i].demand() );
  }

  return demandVec;
}

// Return a list of all demands that have had their cumShipSoftLowerBound
// changed since this problem was initialized
LgFrDemandVector
LgFrWorkingImplosionProblem::changedDemandsCumShipSoftLowerBound()
     const
{
  assert (this-> isValid());
  // Construct an ordered vector capable of holding the number of demands
  // that have been changed.
  size_t nChangedDemands
    = cumShipSoftLowerBoundSchedules_[latestSchedule].size();
  LgFrDemandVector demandVec;
  demandVec.reserve( nChangedDemands );//size_tMax(1,nChangedDemands) );
     // size_tMax used to avoid suspected problem with purify or RW

  // Fill the vector in
  int i = 0; // Pulled out of the for below by RW2STL robot
  for (i = 0;  i < nChangedDemands;  ++i) {
    demandVec.push_back(
                    cumShipSoftLowerBoundSchedules_[latestSchedule][i].demand()
                    );
  }

  return demandVec;
}

// Return a schedule containing the orignal or latest values of
// supplyVolume for each part whose supply has changed.
const LgFrSortingPartScheduleFloat&
LgFrWorkingImplosionProblem::supplyVolume(const LgFrWipSchedule& which)
const
{
  return 
    partSchedules_[ which==LgFrORIGINAL ? originalSchedule : latestSchedule];
}

// Return a schedule of all changes made to part supplies
LgFrSortingPartScheduleFloat
LgFrWorkingImplosionProblem::changedSupplyVolume()
const
{
   return 
      partSchedules_[latestSchedule] -
      partSchedules_[originalSchedule];
}

// Return a schedule of all changes made to part supplies
LgFrSortingPartScheduleFloat
LgFrWorkingImplosionProblem::changedCycleTime()
const
{
   return 
      cycleTimeSchedules_[latestSchedule] -
      cycleTimeSchedules_[originalSchedule];
}

// Return a schedule of all changes made to demand volumes
LgFrSortingDemandScheduleFloat
LgFrWorkingImplosionProblem::changedDemandVolume()
const
{
   return 
      demandSchedules_[latestSchedule] -
      demandSchedules_[originalSchedule];
}

// Return a schedule of all changes made to demand priorities
LgFrSortingDemandScheduleFloat
LgFrWorkingImplosionProblem::changedPriority()
const
{
   return 
      prioritySchedules_[latestSchedule] -
      prioritySchedules_[originalSchedule];
}


// Return a schedule of all changes made to demand obj1ShipRewards
LgFrSortingDemandScheduleFloat
LgFrWorkingImplosionProblem::changedObj1ShipReward()
const
{
   return
      obj1ShipRewardSchedules_[latestSchedule] -
      obj1ShipRewardSchedules_[originalSchedule];
}

// Return a schedule of all changes made to demand cumShipSoftLowerBound
LgFrSortingDemandScheduleFloat
LgFrWorkingImplosionProblem::changedCumShipSoftLowerBound()
const
{
   return
     cumShipSoftLowerBoundSchedules_[latestSchedule] -
     cumShipSoftLowerBoundSchedules_[originalSchedule];
}

// Return a part's (latest) supply vector
LgFrTimeVecFloat
LgFrWorkingImplosionProblem::timeVec (
				      const LgFrPart part)
     const
{
  assert (  this->isValid()  );
  const LgFrSortingPartScheduleFloat &latestPartSched =
     partSchedules_[latestSchedule];
  if (  latestPartSched.contains (part)  ) 
    return 
      (latestPartSched[part]).timeVecFloat();
  else
    return lpsPtr_->supplyVolume (&part);
}


// Change a part's supply vector
void
LgFrWorkingImplosionProblem::timeVec (
				      const LgFrPart part,
				      const LgFrTimeVecFloat& timeVec
				      )
{
  assert ( this->isValid() );
  // If the part is not already in *this, record the original
  // vector for it.
  if (! (partSchedules_[originalSchedule] . contains (part) )  ) {
    partSchedules_[originalSchedule] 
      . insertKeyAndValue (part,
			   lpsPtr_ -> supplyVolume (&part));
  };
  partSchedules_[latestSchedule] . insertKeyAndValue (part, timeVec);

  // Inform subscribers that wip has been updated
  if (!inTransaction_)  {
    LgFrUserInputModification modify(*this);
    LgFrSubscription::externalChange( &modify );
  }
}


// Return a demand's (latest) demand quantity vector
LgFrTimeVecFloat
LgFrWorkingImplosionProblem::timeVec (
				      const LgFrDemand demand)
     const
{
  assert (  this->isValid()  );
  const LgFrSortingDemandScheduleFloat &latestDemandSched 
    = demandSchedules_[latestSchedule];
  if (  latestDemandSched.contains (demand)  )
    return 
      (latestDemandSched[demand]).timeVecFloat();
  else
    return lpsPtr_ -> demandVolume (&demand);
}


// Change a demand's demand Quantity vector
void
LgFrWorkingImplosionProblem::timeVec (
				      const LgFrDemand demand,
				      const LgFrTimeVecFloat& timeVec
				      )
{
  assert ( this->isValid() );
  // If the demand is not already in *this, record the original
  // vector for it.
  if (! (demandSchedules_[originalSchedule] . contains (demand) )  ) {
    demandSchedules_[originalSchedule] 
      . insertKeyAndValue (demand,
			   lpsPtr_ -> demandVolume (&demand));
  };
  demandSchedules_[latestSchedule] . insertKeyAndValue (demand, timeVec);

  // Inform subscribers that wip has been updated
  if (!inTransaction_)  {
    LgFrUserInputModification modify(*this);
    LgFrSubscription::externalChange( &modify );
  }
}



// Return a demand's (latest) priority vector
LgFrTimeVecFloat
LgFrWorkingImplosionProblem::priority (
				      const LgFrDemand demand)
     const
{
  assert (  this->isValid()  );
  const LgFrSortingDemandScheduleFloat &latestDemandSched 
    = prioritySchedules_[latestSchedule];
  if (  latestDemandSched.contains (demand)  )
    return 
      (latestDemandSched[demand]).timeVecFloat();
  else
    return lpsPtr_ -> priority (&demand);
}





// Change a demand's priority vector
void
LgFrWorkingImplosionProblem::priority (
				      const LgFrDemand demand,
				      const LgFrTimeVecFloat& timeVec
				      )
{
  assert ( this->isValid() );
  // If the demand is not already in *this, record the original
  // vector for it.
  if (! (prioritySchedules_[originalSchedule] . contains (demand) )  ) {
    prioritySchedules_[originalSchedule] 
      . insertKeyAndValue (demand,
			   lpsPtr_ -> priority (&demand));
  };
  prioritySchedules_[latestSchedule] . insertKeyAndValue (demand, timeVec);

  // Inform subscribers that wip has been updated
  if (!inTransaction_)  {
    LgFrUserInputModification modify(*this);
    LgFrSubscription::externalChange( &modify );
  }
}





// Return a demand's (latest) obj1ShipReward vector
LgFrTimeVecFloat
LgFrWorkingImplosionProblem::obj1ShipReward (
                                      const LgFrDemand demand)
     const
{
  assert (  this->isValid()  );
  const LgFrSortingDemandScheduleFloat &latestDemandSched
    = obj1ShipRewardSchedules_[latestSchedule];
  if (  latestDemandSched.contains (demand)  )
    return
      (latestDemandSched[demand]).timeVecFloat();
  else
    return lpsPtr_ -> obj1ShipReward (&demand);
}


// Change a demand's obj1ShipReward vector
void
LgFrWorkingImplosionProblem::obj1ShipReward (
                                      const LgFrDemand demand,
                                      const LgFrTimeVecFloat& timeVec
                                      )
{
  assert ( this->isValid() );
  // If the demand is not already in *this, record the original
  // vector for it.
  if (! (obj1ShipRewardSchedules_[originalSchedule] . contains (demand) )  ) {
    obj1ShipRewardSchedules_[originalSchedule]
      . insertKeyAndValue (demand,
                           lpsPtr_ -> obj1ShipReward (&demand));
  };
  obj1ShipRewardSchedules_[latestSchedule] . insertKeyAndValue (
     demand, timeVec);

  // Inform subscribers that wip has been updated
  if (!inTransaction_)  {
    LgFrUserInputModification modify(*this);
    LgFrSubscription::externalChange( &modify );
  }
}




// Return a demand's (latest) cumShipSoftLowerBound vector
LgFrTimeVecFloat
LgFrWorkingImplosionProblem::cumShipSoftLowerBound (
                                      const LgFrDemand demand)
     const
{
  assert (  this->isValid()  );
  const LgFrSortingDemandScheduleFloat &latestDemandSched
    = cumShipSoftLowerBoundSchedules_[latestSchedule];

  if (  latestDemandSched.contains (demand)  )
    return
      (latestDemandSched[demand]).timeVecFloat();
  else
    return lpsPtr_ -> cumShipSoftLowerBound (&demand);
}


// Change a demand's cumShipSoftLowerBound vector
void
LgFrWorkingImplosionProblem::cumShipSoftLowerBound (
                                      const LgFrDemand demand,
                                      const LgFrTimeVecFloat& timeVec
                                      )
{
  assert ( this->isValid() );
  // If the demand is not already in *this, record the original
  // vector for it.
  if (
     !(cumShipSoftLowerBoundSchedules_[originalSchedule] . contains (demand) )
     )
    {
      cumShipSoftLowerBoundSchedules_[originalSchedule]
        . insertKeyAndValue (demand,
                             lpsPtr_ -> cumShipSoftLowerBound (&demand));
    };
  cumShipSoftLowerBoundSchedules_[latestSchedule] . insertKeyAndValue (
     demand, timeVec);

  // Inform subscribers that wip has been updated
  if (!inTransaction_)  {
    LgFrUserInputModification modify(*this);
    LgFrSubscription::externalChange( &modify );
  }
}




// Return a part's cycleTime vector
LgFrTimeVecFloat
LgFrWorkingImplosionProblem::cycleTime (
				      const LgFrPart part)
     const
{
  assert (  this->isValid()  );
  const LgFrSortingPartScheduleFloat &latestPartCycleTimeSched 
    = cycleTimeSchedules_[latestSchedule];
  if (  latestPartCycleTimeSched.contains (part)  )
    return 
      ( latestPartCycleTimeSched[part] )  .  timeVecFloat();
  else
    return lpsPtr_->cycleTime (&part);
}


void
LgFrWorkingImplosionProblem::cycleTime (
				      const LgFrPart part,
				      const LgFrTimeVecFloat& timeVec
				      )
{
  assert ( this->isValid() );
  // If the part is not already in *this, record the original
  // vector for it.
  if (! (cycleTimeSchedules_[originalSchedule] . contains (part) )  ) {
    cycleTimeSchedules_[originalSchedule] 
      . insertKeyAndValue (part,
			   lpsPtr_ -> cycleTime (&part));
  };
  cycleTimeSchedules_[latestSchedule] . insertKeyAndValue (part, timeVec);

  // Inform subscribers that wip has been updated
  if (!inTransaction_)  {
    LgFrUserInputModification modify(*this);
    LgFrSubscription::externalChange( &modify );
  }
}


void
LgFrWorkingImplosionProblem::beginSeriesOfChanges()
{
  inTransaction_ = 1;
}

void
LgFrWorkingImplosionProblem::endSeriesOfChanges()
{
  inTransaction_ = 0;
  LgFrUserInputModification modify(*this);
  LgFrSubscription::externalChange( &modify );
}

// Another Constructor
LgFrWorkingImplosionProblem::LgFrWorkingImplosionProblem (
				LgFrLastProblemSolved& myLps,
				LgFrSetOfParts& mySoParts,
				LgFrSetOfDemands& mySoDemands
							  )
: soPartsPtr_ (&mySoParts),
  soDemandsPtr_ (&mySoDemands),
  lpsPtr_ (&myLps),
  // Don't know what size to use for partSchedules_[0 and 1] so let
  // defaults get taken.
  implosionMethod_   (3, 0),
  defaultImplosionMethod_(LgFrHEURISTIC),
  equitability_      (3, 0),
  buildAhead_        (3, false),
  computeCriticalList_ (3, false),
  weightBounds_      (3, 0.0),
  inTransaction_ (0)  
{
  // Nothing to do here
}


// Another "copy constructor"
LgFrWorkingImplosionProblem::LgFrWorkingImplosionProblem(
			      const LgFrWorkingImplosionProblem & source,
			      LgFrLastProblemSolved& sourceLps,
			      const LgFrSetOfParts& sourceSoParts,
			      const LgFrSetOfDemands& sourceSoDemands)
: soPartsPtr_ (&sourceSoParts),
  soDemandsPtr_ (&sourceSoDemands),
  lpsPtr_ (&sourceLps)
{
  this->copyInPlace (source);
}


// This method copies the data that is contained in the source to *this
// without changing the soPartsPtr_, soDemandsPtr_, lpsPtr_ of *this.
void
LgFrWorkingImplosionProblem::copyInPlace (const LgFrWorkingImplosionProblem& source)
{
  implosionMethod_   = source.implosionMethod_;
  defaultImplosionMethod_= LgFrHEURISTIC;
  equitability_      = source.equitability_;
  buildAhead_        = source.buildAhead_;
  computeCriticalList_ = source.computeCriticalList_; 
  weightBounds_      = source.weightBounds_;
  inTransaction_     = source.inTransaction_;
  
  // Copy the part and demand schedules
  int i = 0; // Pulled out of the for below by RW2STL robot
  for (i = 0;  i < nSchedules; i++) {
    partSchedules_ [i] = source.partSchedules_[i];
    demandSchedules_ [i] = source.demandSchedules_[i];
  }
}


// Destructor
LgFrWorkingImplosionProblem::~LgFrWorkingImplosionProblem()
{
  // The thing lpsPtr_ points to should not be deleted by this class.
}


// Assignment operator
LgFrWorkingImplosionProblem&
LgFrWorkingImplosionProblem::operator=(
            const LgFrWorkingImplosionProblem& rhs)
{
  soPartsPtr_ = rhs.soPartsPtr_;
  soDemandsPtr_ = rhs.soDemandsPtr_;
  lpsPtr_ = rhs.lpsPtr_;
  inTransaction_ = rhs.inTransaction_;

  int i = 0; // Pulled out of the for below by RW2STL robot
  for (i = 0;  i < nSchedules; ++i) {
    partSchedules_[i] = rhs.partSchedules_[i];
    demandSchedules_[i] = rhs.demandSchedules_[i];
  }

  return *this;
}

// Set Implostion Method
void
LgFrWorkingImplosionProblem::implosionMethod(
   const LgFrImplosionMethod newMeth )
{
   if ( implosionMethod_[isChanged]==0 ) {
      implosionMethod_[orig] = (int) defaultImplosionMethod_;
      implosionMethod_[isChanged] = 1;
   }
   implosionMethod_[latest] = (int) newMeth;
}

// Get Implosion Method
LgFrImplosionMethod
LgFrWorkingImplosionProblem::implosionMethod()
const
{
   LgFrImplosionMethod retVal;
   if ( implosionMethod_[isChanged]!=0 ) 
      {
         retVal = (LgFrImplosionMethod) implosionMethod_[latest];
      }
   else
      {
         retVal = defaultImplosionMethod_;
      }
   return retVal;
}


// Set heuristic equitable allocation
void
LgFrWorkingImplosionProblem::equitability(
   const int newValue )
{
   setWitInt(newValue,
               equitability_,
	     &LgFrLastProblemSolved::equitability);
}

// Get heuristic equitable allocation
int
LgFrWorkingImplosionProblem::equitability()
const
{
   return getWitInt(equitability_, 
		    &LgFrLastProblemSolved::equitability );
}

// Set heuristic build ahead or not
void
LgFrWorkingImplosionProblem::buildAhead(
   const bool newValue )
{
   setWitbool(newValue,
               buildAhead_,
		   &LgFrLastProblemSolved::buildAhead);
}

// Get heuristic build ahead or not
bool
LgFrWorkingImplosionProblem::buildAhead()
const
{
   return getWitbool(buildAhead_, 
			  &LgFrLastProblemSolved::buildAhead );
}

// Set heuristic compute critical list or not
void
LgFrWorkingImplosionProblem::computeCriticalList(
   const bool newValue )
{
   setWitbool(newValue,
               computeCriticalList_,
		   &LgFrLastProblemSolved::computeCriticalList);
}

// Get heuristic compute critical list or not
bool
LgFrWorkingImplosionProblem::computeCriticalList()
const
{
   return getWitbool(computeCriticalList_, 
			  &LgFrLastProblemSolved::computeCriticalList );
}


// Set objective function weight on Bounds
void
LgFrWorkingImplosionProblem::weightBounds(
   const float newValue )
{
   setWitFloat(newValue,
               weightBounds_,
               &LgFrLastProblemSolved::weightBounds);
}

// Get objective function weight on Bounds
float
LgFrWorkingImplosionProblem::weightBounds()
const
{
   return getWitFloat(weightBounds_, 
                      &LgFrLastProblemSolved::weightBounds );
}




   //-------------------------------------------------------------------------
   //
   // Subscribe methods
   //
   //-------------------------------------------------------------------------

   // Inform object m when the implosion solution or MRP solution
   // has changed.  The object added must have a member function
   // updateFrom( LgFrModel * p, void * d ).  
   // When the working implosion problem
   // is changed then m.updateFrom( this, LgFrUserInputModification() ) is invoked.
   //
   // See LgFrModel and LgFrModelClient.

void 
LgFrWorkingImplosionProblem::addDependent(
   LgFrModelClient * m)
{
   LgFrSubscription::addDependent( m );
}

   // No longer inform object m when implosion or MRP solution has changed.
void 
LgFrWorkingImplosionProblem::removeDependent(
   LgFrModelClient * m )
{
   LgFrSubscription::removeDependent( m );
}

// Update From should not be invoked because wip does not subscribe to
// any other objects.
void 
LgFrWorkingImplosionProblem::localUpdate(
   LgFrModel *,
   void * )
{
   std::cerr <<"LgFrWorkingImplosionProblem::updateFrom()" <<std::endl
        <<"This method should not be invoked because wip does not" <<std::endl
        <<"subscribe to any other objects" <<std::endl;
   abort();
}



// Copy constructor
LgFrWorkingImplosionProblem::LgFrWorkingImplosionProblem(
            const LgFrWorkingImplosionProblem& source)
: soPartsPtr_ (source.soPartsPtr_),
  soDemandsPtr_ (source.soDemandsPtr_),
  lpsPtr_ (source.lpsPtr_),
  implosionMethod_   (3, 0),
  defaultImplosionMethod_(LgFrHEURISTIC),
  equitability_      (3, 0),
  buildAhead_        (3, false),
  computeCriticalList_ (3, false),
  weightBounds_      (3, 0.0)
{
  this->copyInPlace (source);  
}

// Default Constructor
// Initialize file name to null string.
LgFrWorkingImplosionProblem::LgFrWorkingImplosionProblem()
: soPartsPtr_ (0),
  soDemandsPtr_ (0),
  lpsPtr_ (0),
  implosionMethod_   (3, 0),
  defaultImplosionMethod_(LgFrHEURISTIC),
  equitability_      (3, 0),
  buildAhead_        (3, false),
  computeCriticalList_ (3, false),
  weightBounds_      (3, 0.0)
{
  // Nothing else to do
}


// Set WIT Float attribute
void
LgFrWorkingImplosionProblem::setWitFloat(
   const float newValue,
   LgFrFloatVector & statusVec,
   float (LgFrLastProblemSolved::*lpsGetFunc) () const )
{
   assert ( statusVec.size() == 3 );
   if ( statusVec[isChanged]==0. ) {
      statusVec[orig] = (lpsPtr_->*lpsGetFunc)();
      statusVec[isChanged] = 1.;
   }
   statusVec[latest] = newValue;
}

// Get WIT Float attribute
float
LgFrWorkingImplosionProblem::getWitFloat(
   const LgFrFloatVector statusVec,
   float (LgFrLastProblemSolved::*lpsGetFunc) () const )
const
{
   float retVal;
   if ( statusVec[isChanged]!=0 ) 
      {
         retVal = statusVec[latest];
      }
   else
      {
         retVal = (lpsPtr_->*lpsGetFunc)();
      }
   return retVal;
}

// Set WIT bool attribute
void
LgFrWorkingImplosionProblem::setWitbool(
   const bool newValue,
   LgFrBoolVector & statusVec,
   bool (LgFrLastProblemSolved::*lpsGetFunc) () const )
{
   if ( !statusVec[isChanged] ) {
      statusVec[orig] = (lpsPtr_->*lpsGetFunc)();
      statusVec[isChanged] = true;
   }
   statusVec[latest] = newValue;
}

// Get WIT Int attribute
bool
LgFrWorkingImplosionProblem::getWitbool(
   const LgFrBoolVector statusVec,
   bool (LgFrLastProblemSolved::*lpsGetFunc) () const )
const
{
   bool retVal;
   if ( statusVec[isChanged] ) 
      {
         retVal = statusVec[latest];
      }
   else
      {
         retVal = (lpsPtr_->*lpsGetFunc)();
      }
   return retVal;
}

// Set WIT Int attribute
void
LgFrWorkingImplosionProblem::setWitInt(
   const int newValue,
   LgFrIntVector & statusVec,
   int (LgFrLastProblemSolved::*lpsGetFunc) () const )
{
   if ( statusVec[isChanged]==0 ) {
      statusVec[orig] = (lpsPtr_->*lpsGetFunc)();
      statusVec[isChanged] = 1;
   }
   statusVec[latest] = newValue;
}

// Get WIT Int attribute
int
LgFrWorkingImplosionProblem::getWitInt(
   const LgFrIntVector statusVec,
   int (LgFrLastProblemSolved::*lpsGetFunc) () const )
const
{
   int retVal;
   if ( statusVec[isChanged]!=0 ) 
      {
         retVal = statusVec[latest];
      }
   else
      {
         retVal = (lpsPtr_->*lpsGetFunc)();
      }
   return retVal;
}

#ifdef NDEBUG
#undef NDEBUG
#endif
void
LgFrWorkingImplosionProblem::test()
{
  // Test constructor
  LgFrWorkingImplosionProblem defaultWip;
  assert (defaultWip.lpsPtr_ == 0);
}


// Expects the data in aWip to come from
// "/u/implode/wit/data/standard/obj1/wit.data"
void
LgFrWorkingImplosionProblem::contextTest
  (LgFrWorkingImplosionProblem & aWip,
   LgFrSetOfParts & sop,		// from same scenario as aWip
   LgFrSetOfDemands & sod)		// from same scenario as aWip
{
  const float fltEps = .0001f;
  const LgFrPart * comp1ptr = sop.findValue ("COMP1");
  assert (comp1ptr != 0);
  const LgFrDemand * s2demand1ptr = sod.findValue ("S2DEMAND1", "SUBASMBL2");
  assert (s2demand1ptr != 0);
  LgFrPart comp1 = (*comp1ptr);
  LgFrDemand s2demand1 = (*s2demand1ptr); 
  
  // Test regular methods
  LgFrTimeVecFloat ptv = aWip.timeVec (comp1);
  assert (ptv[0] == 100  && ptv[7] == 50);
  ptv[0]=99;
  aWip.timeVec(comp1, ptv);
  LgFrTimeVecFloat nptv = aWip.timeVec (comp1);
  assert (nptv[0] == ptv[0]);

  LgFrPartVector changedParts(aWip.changedParts());
  assert (changedParts.size() == 1);
  assert (changedParts[0] == comp1);

  {
    // Test changedDemands when the number of changed demands is zero
    LgFrDemandVector changedDemands(aWip.changedDemands());
    assert (changedDemands.size() == 0);
  }

  LgFrTimeVecFloat dtv = aWip.timeVec (s2demand1);
  assert (dtv[0] == 0  &&  dtv[1] == 0  && dtv[7] == 20);
  dtv[0]=17;
  aWip.timeVec (s2demand1, dtv);
  LgFrTimeVecFloat ndtv = aWip.timeVec (s2demand1);
  assert (ndtv[0] == dtv[0]);

  LgFrDemandVector changedDemands(aWip.changedDemands());
  assert (changedDemands.size() == 1);
  assert (changedDemands[0] == s2demand1);

  LgFrTimeVecFloat prioritytv = aWip.priority (s2demand1);
  float priority1 = prioritytv[1];
  aWip.priority (s2demand1, prioritytv+1. );
  LgFrTimeVecFloat nprioritytv = aWip.priority (s2demand1);
  assert (nprioritytv == prioritytv+1. );

  LgFrDemandVector changedPriority(aWip.changedDemandsPriority());
  assert (changedPriority.size() == 1);
  assert (changedPriority[0] == s2demand1);

  // Test setting/getting implosion method
  assert( aWip.implosionMethod() == LgFrHEURISTIC );
  aWip.implosionMethod(LgFrHEURISTIC);
  assert( aWip.implosionMethod() == LgFrHEURISTIC );
#ifdef OPT_IMPLODE
  aWip.implosionMethod(LgFrOPTIMIZE);
  assert( aWip.implosionMethod() == LgFrOPTIMIZE );
#endif


  // Test setting/getting of WIT equitability
  aWip.equitability( 35 );
  assert( aWip.equitability()==35 );
  aWip.equitability( 1 );
  assert( aWip.equitability()==1 );

  // Test setting/getting of WIT build ahead
  aWip.buildAhead( true );
  assert( aWip.buildAhead() );
  aWip.buildAhead( false );
  assert( !aWip.buildAhead() );

  // Test setting/getting of WIT critical list 
  aWip.computeCriticalList( false );
  assert( !aWip.computeCriticalList() );
  aWip.computeCriticalList( true );
  assert( aWip.computeCriticalList() );


  // Test setting/getting of WIT weightBounds
  aWip.weightBounds( 3. );
  assert( fabs(aWip.weightBounds()-3.) < fltEps );
  aWip.weightBounds( 4. );
  assert( fabs(aWip.weightBounds()-4.) < fltEps );

  // Test changedSupplyVolume
  {
  LgFrSortingPartScheduleFloat deltaSupply( aWip.changedSupplyVolume() );
  const  LgFrSortingPartScheduleFloat & constDeltaSupply = deltaSupply;
  // cout <<"changedSupplyVolume" <<std::endl;
  // cout <<deltaSupply.format() <<std::endl;
  assert( deltaSupply.size()==1 );
  assert( constDeltaSupply[0].part() == comp1 );
  assert( constDeltaSupply[0].timeVecFloat()[0] == -1 );
  }

  // Test changedDemandVolume
  {
  LgFrSortingDemandScheduleFloat deltaDemand( aWip.changedDemandVolume() );
  const LgFrSortingDemandScheduleFloat & constDeltaDemand = deltaDemand;
  // cout <<"changedDemandVolume" <<std::endl;
  // cout <<deltaDemand.format() <<std::endl;
  assert( deltaDemand.size()==1 );
  assert( constDeltaDemand[0].demand() == s2demand1 );
  assert( constDeltaDemand[0].timeVecFloat()[0] == 17 );
  }

  // Test changedPriority
  {
  LgFrSortingDemandScheduleFloat deltaPriority( aWip.changedPriority() );
  const LgFrSortingDemandScheduleFloat & constDeltaPriority = deltaPriority;
  // cout <<"changedDemandVolume" <<std::endl;
  // cout <<deltaDemand.format() <<std::endl;
  assert( deltaPriority.size()==1 );
  assert( constDeltaPriority[0].demand() == s2demand1 );
  assert( constDeltaPriority[0].timeVecFloat() == nprioritytv - prioritytv );
  }

  // Test cumShipSoftLowerBound
  {   
  LgFrTimeVecFloat csslb = aWip.cumShipSoftLowerBound (s2demand1);
  LgFrTimeVecFloat newCsslb = csslb + 15;
  aWip.cumShipSoftLowerBound (s2demand1, newCsslb );
  assert (aWip.cumShipSoftLowerBound (s2demand1) == newCsslb );

  LgFrDemandVector chgDemands
    = aWip.changedDemandsCumShipSoftLowerBound();
  assert( chgDemands.size() == 1 );
  assert( chgDemands[0] == s2demand1 );
  
  LgFrSortingDemandScheduleFloat delta( aWip.changedCumShipSoftLowerBound() );
  const LgFrSortingDemandScheduleFloat & constDelta = delta;
  assert( delta.size()==1 );
  assert( constDelta[0].demand() == s2demand1 );
  assert( constDelta[0].timeVecFloat() == ( newCsslb - csslb ) );
  }

}
