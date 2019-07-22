#if defined(_MSC_VER)
#pragma warning(disable:4786)
#endif

// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL -- inserted includes.
#include <math.h>
#include <iostream>

#include <scenario/src/fssCtrl.h>
#include <scenario/src/scenario.h>
#include <scenario/src/lastPrbS.h>
#include <scenario/src/wrkgProb.h>
#include <scenario/src/sOfDemds.h>
#include <scenario/src/machdep.h>
#include <scenario/src/implode.h>



// updateFrom - This method is invoked when the algorthm solution has
// been updated.
void
LgFrFocussedShortageScheduleControl::localUpdate (
   LgFrModel * ,
   void * )
{
      // Mark current implosion solution invalid
      markInvalid();
}

// Set a demand's focus horizon
void
LgFrFocussedShortageScheduleControl::focusHorizon (
   const LgFrDemand demand,
   const int        focusHorizon )
{
   assert( ( focusHorizon >= -1 ) &&
           ( focusHorizon < implodePtr_->lastProblemSolved().numberOfPeriods() ) );
   focusChanged_ = true;
   int index = findDemandIndex(demand);
   if ( index>=0 ) {
      // Demand already in focus
      focus_[index].period(focusHorizon);
   }
   else {
      // Demand not in focus, it needes to be added
      focus_.push_back( LgFrDemandPeriodPair(demand,focusHorizon) );
   }
}

// Get a demand's focus horizon
int
LgFrFocussedShortageScheduleControl::focusHorizon (
   const LgFrDemand demand )
const
{
   int index = findDemandIndex(demand);
   if ( index>=0 ) {
      // Demand in focus
      return focus_[index].period();
   }
   else {
      // Demand not in focus, return default focus
      return -1;
   }
}


// Remove all demands from the focus horizon
void
LgFrFocussedShortageScheduleControl::clearFocusHorizons()
{
   size_t i = 0; // Pulled out of the for below by RW2STL
   for ( i=0; i<focus_.size(); i++ )  focus_[i].period(-1);
}


// Give WIT the input it needs to do an FSS, assuming an implosion has
// already been done.  This assumes the model in WIT has been set up
// to use focus horizons.
void
LgFrFocussedShortageScheduleControl::giveWitInputForFocusHorizons()
{
  LgFrLastProblemSolved & lps = implodePtr_->lastProblemSolved();
  size_t i;

  // Set focus horizon for all demands in the focus
  for ( i=0; i<focus_.size(); i++ ) {
    std::cerr << "INFO: Setting focus for " 
	 << focus_[i].demand().printingName(false) << " to " 
	 << focus_[i].period() << std::endl;
    lps.focusHorizon( &(focus_[i].demand()), focus_[i].period()     );
  }
}


// Give WIT the input it needs to do an FSS, assuming an implosion has
// already been done.  This assumes the model in WIT has been set up
// to use shipVol's (not focus horizons).
void
LgFrFocussedShortageScheduleControl::giveWitInputForFssShipVol()
{
  LgFrLastProblemSolved & lps = implodePtr_->lastProblemSolved();
  const float fltEps = .0001f;
  size_t i;
  size_t desiredPeriod;
  LgFrTimeVecFloat implosionShipVol;  // implosion result for period desiredPeriod
  LgFrTimeVecFloat desiredShipVol;    // desired shipVol for period desiredPeriod
  
  // Set the fssShipVol to the appropriate amount for the periods of interest.
  for ( i=0; i<focus_.size(); i++ ) {
    desiredPeriod = focus_[i].period();
    implosionShipVol = lps.shipmentVolume( &( focus_[i].demand() ) );

    if (focus_[i].period() == -1) {
      // This period should not get a focus anymore.
      desiredShipVol = implosionShipVol;
    }
    else {
      LgFrTimeVecFloat backlog = lps.backlogVolume( &( focus_[i].demand() ) );
      float b = backlog[ desiredPeriod ];
      // Get desired shipment level by adding the backlog in the
      // desired period, b, to the shipment volume in that period.
      desiredShipVol = implosionShipVol;
      desiredShipVol[ desiredPeriod ]  +=  b;
      // If the implosion result reduced the backlog by shipping later,
      // reduce desiredShipVol in those periods so that FSS does not
      // calculate the resources needed to ship in the desired period
      // *and* later.
      desiredShipVol.takeQtyFromPositiveElementsAfterIndex( b, desiredPeriod,
							    fltEps);

      if (  b < (backlog[desiredPeriod] - fltEps)  )
	std::cerr << "INFO: For FSS calculation for "
	     << focus_[i].demand().printingName(false) << " in  period "
	     << desiredPeriod << ", \n" << (backlog[desiredPeriod] - b)
	     << " of the units would have shipped in later periods.  "
	     << "FSS will calculate \n"
	     << "what has to be done to ship them on time instead of late.\n";

      std::cerr << "INFO: Setting fssShipVol for  " 
	   << focus_[i].demand().printingName(false)
	   << " to " << desiredShipVol.format() << std::endl;
    }

    lps.fssShipVolume( &(focus_[i].demand() ), desiredShipVol );
  }
}


// Get focused shortage schedule
const LgFrSortingPartScheduleFloat &
LgFrFocussedShortageScheduleControl::focussedShortageSchedule()
{
  LgFrLastProblemSolved & lps = implodePtr_->lastProblemSolved();

  if ( lps.imploded() && !validFSS_ ) {
    // FSS is not up-to-date, so get current FSS from LPS 

    // Remove all demands from WIT's focus horizon
    // Question from Wally: Why is every single demand being removed?
    // Wouldn't it be faster to just clear the ones that are in
    // focus_?
    {
      int i = 0; // Pulled out of the for below by RW2STL
      for ( i=0; i<setOfDemandsPtr_->size(); i++ ) {
	LgFrDemand d = (*setOfDemandsPtr_)[i];
	lps.focusHorizon( &d, -1 );
      }
    }

    // Check to see if focus horizons are being used and do the
    // right thing
    if ( lps.useFocusHorizons() )
      this->giveWitInputForFocusHorizons();
    else 
      this->giveWitInputForFssShipVol();

    // Get FSS from lps
    int lenList;
    char ** partList;
    float ** shortageVolList;
    lps.focusShortageVolume( 
		   &lenList,
		   &partList,
		   &shortageVolList);

    // Update FSS_ with FSS obtained from lps
    FSS_.localClear();
    FSS_.reserve(lenList);
    int nPeriods = lps.numberOfPeriods();
    std::cerr << "INFO: number of entries in FSS result is " << lenList << std::endl;
    size_t i;
    for ( i=0; i<lenList; i++ ) {
       const LgFrPart * part = setOfPartsPtr_->findValue( partList[i] );
       assert( part != 0 );
       const LgFrTimeVecFloat partShortage( nPeriods, shortageVolList[i] );
       FSS_.insert(  LgFrItemTimeVecPairFloat (*part, partShortage)  );
    }

    // Free storage returned by lps FocusShortageVol.
    for ( i=0; i<lenList; i++ ) {
       portableFree(partList[i]);
       portableFree(shortageVolList[i]);
    }
    portableFree(partList);
    portableFree(shortageVolList);

    // Mark FSS_ as valid
    validFSS_ = true;

    // Mark focus unchanged
    focusChanged_ = false;
  }

  return FSS_;
}


// Get focused shortage schedule's production time vector float for a part
LgFrTimeVecFloat
LgFrFocussedShortageScheduleControl::fssProductionVolume
                                     (const LgFrPart * const partPtr)
{
  if ( implodePtr_->lastProblemSolved().imploded() && !validFSS_ ) 
    // FSS is not up-to-date, make FSS current from LPS 
    focussedShortageSchedule();

  // Get FSS production time vector float from lps
  return implodePtr_->lastProblemSolved().fssProductionVolume(partPtr);
}

// Get latest focused shortage schedule
const LgFrSortingPartScheduleFloat &
LgFrFocussedShortageScheduleControl::latestFocussedShortageSchedule()
{
   if ( focusChanged_ ) markInvalid();
   return focussedShortageSchedule(); 
}

// Get latest focused shortage schedule's production time vector float for
// a part.
LgFrTimeVecFloat
LgFrFocussedShortageScheduleControl::latestFSSProductionVolume
                                     (const LgFrPart * const partPtr)
{
   if ( focusChanged_ ) markInvalid();
   return fssProductionVolume(partPtr);
}

void
LgFrFocussedShortageScheduleControl::updateFocussedShortageSchedule()
{
  if (focusChanged_)
    markInvalid();
}


// Mark FSS_ as containing an invalid focused shortage schedule.
void
LgFrFocussedShortageScheduleControl::markInvalid()
{
   validFSS_ = false;
}

// Return value of validFSS_
bool
LgFrFocussedShortageScheduleControl::isValid()
const
{
   return validFSS_;
}

// Return value of focusChanged_
bool
LgFrFocussedShortageScheduleControl::focusChanged()
const
{
   return focusChanged_;
}

// Return pointer to the fssCtrl algorithm in the specified scenario.
LgFrAlgorithm *
LgFrFocussedShortageScheduleControl::counterPart(
LgFrScenario & scenario)
const
{
   return &( scenario.focussedShortageScheduleControl() );
}

// Return implode pointer
LgFrImplode *
LgFrFocussedShortageScheduleControl::implodePtr()
const
{
   return implodePtr_;
}

// Print out the demand data
void 
LgFrFocussedShortageScheduleControl::print() const
{
   size_t i,t;
  
   std::cout << "\n************************************";
   std::cout << "\n Data for LgFrFocussedShortageScheduleControl: ";
   std::cout << "\n Demand Period Pair data:" <<std::endl;
   for ( i=0; i<focus_.size(); i++ ) {
      std::cout << "    "
           << (focus_[i].demand()).partPtr()->name()
           << "  "
           << (focus_[i].demand()).name()
           << "  "
           << focus_[i].period() 
           << std::endl;
   }
   std::cout << "\n focused shortage schedule data:" <<std::endl;
   for ( i=0; i<FSS_.size(); i++ ) {
#if 0
      std::cout << "    "
           << FSS_[i].item().name()
           << std::endl; 
      std::cout << "    ";
#endif
      for ( t=0; t<FSS_[i].timeVecFloat().length(); t++ ) {
         std::cout << "  "
              << (FSS_[i].timeVecFloat())[t];
      }
      std::cout << std::endl;
   }
}

void
LgFrFocussedShortageScheduleControl::copyFocusMember (const LgFrFocussedShortageScheduleControl& source)
{
  if ((&focus_) != &(source.focus_)) {
    focus_.clear();
    size_t sourceEntries = source.focus_.size();
    focus_.reserve (sourceEntries);
    int i = 0; // Pulled out of the for below by RW2STL
    for (i = 0; i < sourceEntries; ++i)
      focus_.push_back (source.focus_[i]);
  }
}


void
LgFrFocussedShortageScheduleControl::copyInPlace (
    const LgFrFocussedShortageScheduleControl& source)
{
// copyFocusMember (source);  Done using assignment operator now
  focus_ = source.focus_;
  FSS_ = source.FSS_;
  validFSS_ = source.validFSS_;
  focusChanged_ = source.focusChanged_;
}

// Default constructor
LgFrFocussedShortageScheduleControl::LgFrFocussedShortageScheduleControl()
: LgFrAlgorithm(),
  focus_(),
  FSS_(), 
  validFSS_(false), 
  focusChanged_(true), 
  implodePtr_(0),
  setOfPartsPtr_(0),
  setOfDemandsPtr_(0),
  id_(__LGFRFOCUSSEDSHORTAGESCHEDULECONTROL)
{
  assert (this->numberDependents() == 0);
  // All the work is done in the initialization part
}

// A Constructor
LgFrFocussedShortageScheduleControl::LgFrFocussedShortageScheduleControl(
   LgFrImplode & implode,
   LgFrSetOfParts& mySetOfParts,
   LgFrSetOfDemands& mySetOfDemands)
: LgFrAlgorithm(),
  focus_(),
  FSS_(), 
  validFSS_(false),
  focusChanged_(true),
  implodePtr_(&implode),
  setOfPartsPtr_(&mySetOfParts),
  setOfDemandsPtr_(&mySetOfDemands),
  id_(__LGFRFOCUSSEDSHORTAGESCHEDULECONTROL)
{
  assert (this->numberDependents() == 0);
  // Register with *implodePtr_ to be informed when implosion solution changes
  implodePtr_->addDependent( this );
}


// Another Constructor.  Makes a deep copy of focus_ and FSS_
LgFrFocussedShortageScheduleControl::LgFrFocussedShortageScheduleControl(
   const LgFrFocussedShortageScheduleControl & source,
   LgFrImplode & implode,
   LgFrSetOfParts& mySetOfParts,
   LgFrSetOfDemands& mySetOfDemands)
: LgFrAlgorithm(),              // Do not copy the base LgFrAlgoithm.
                                // Do not want it's subscibers.
  focus_(source.focus_),
  FSS_(source.FSS_), 
  validFSS_(source.validFSS_),
  focusChanged_(source.focusChanged_),
  implodePtr_(&implode),
  setOfPartsPtr_(&mySetOfParts),
  setOfDemandsPtr_(&mySetOfDemands),
  id_(__LGFRFOCUSSEDSHORTAGESCHEDULECONTROL)
{
  assert (this->numberDependents() == 0);  
  // Register with implode to be informed when implosion solution changes
  implodePtr_->addDependent( this );
}



// Copy constructor
// Be careful using this.  It is not useful for implementing
// an owners assignment because it does not take the associated objects
// (setOfDemandsPtr_, implodePtr_, etc.) as arguments.
LgFrFocussedShortageScheduleControl::LgFrFocussedShortageScheduleControl 
   (const LgFrFocussedShortageScheduleControl& source)
: LgFrAlgorithm(),              // Do not copy the base LgFrAlgoithm.
                                // Do not want it's subscibers.
  focus_(source.focus_),
  FSS_(source.FSS_), 
  validFSS_(source.validFSS_), 
  focusChanged_(source.focusChanged_), 
  implodePtr_(source.implodePtr_),
  setOfPartsPtr_(source.setOfPartsPtr_),
  setOfDemandsPtr_(source.setOfDemandsPtr_),
  id_(__LGFRFOCUSSEDSHORTAGESCHEDULECONTROL)
{
  abort();                      // Not expected to be useful anymore
  assert (this->numberDependents() == 0);  
  // Register with implode to be informed when implosion solution changes
  if (implodePtr_!=0) implodePtr_->addDependent( this );
}

// Destructor
LgFrFocussedShortageScheduleControl::~LgFrFocussedShortageScheduleControl()
{
  // Unregister with LPS
  if (implodePtr_!=0) implodePtr_->removeDependent( this );
}

// Assignment
// Be careful using this.  It is not useful for implementing
// an owners assignment because it does not take the associated objects
// (setOfDemandsPtr_, implodePtr_, etc.) as arguments.
LgFrFocussedShortageScheduleControl&
LgFrFocussedShortageScheduleControl::operator=
(const LgFrFocussedShortageScheduleControl& rhs)
{
  abort();                      // Not expected to be useful anymore
  if (this != &rhs) {
     (this->LgFrAlgorithm::operator= ) (rhs);
     focus_ = rhs.focus_;
     FSS_ = rhs.FSS_;
     validFSS_ = rhs.validFSS_;
     focusChanged_ = rhs.focusChanged_;

     if (implodePtr_!=0) implodePtr_->removeDependent( this );
     implodePtr_ = rhs.implodePtr_;
     if (implodePtr_!=0) implodePtr_->addDependent( this );

     setOfPartsPtr_ = rhs.setOfPartsPtr_;
     setOfDemandsPtr_ = rhs.setOfDemandsPtr_;
  }
  return (*this);
}

// Private function to find demand d in focus_
int 
LgFrFocussedShortageScheduleControl::findDemandIndex( 
  LgFrDemand d )
  const
{
   size_t i = 0; // Pulled out of the for below by RW2STL
   for ( i=0; i<focus_.size(); i++ ) {
      if ( focus_[i].demand() == d ) return i;
   }
   return -1;
}


#ifdef NDEBUG
#undef NDEBUG
#endif
// test LgFrFocussedShortageScheduleControl
void
LgFrFocussedShortageScheduleControl::test()
{
  // Test a constructor
  LgFrFocussedShortageScheduleControl defaultFssCtrl;
  assert (defaultFssCtrl.implodePtr_ == 0);
}


// test LgFrFocussedShortageScheduleControl
// This method expects the data for the first argument (scenario) to have come
// from /u/implode/wit/data/examples/diner12/wit.data.
// It expects that scenarios WIP, LPS, SOP, SOD, and FSS-Ctrl to be passed in the
// 2nd - 6th parameters
// It expects the 7th parameter to have been created by deep-copying the
// first scenario and expects the 8th parameter to be that scenarios FSS-Ctrl.
void
LgFrFocussedShortageScheduleControl::contextTest
    (
     LgFrScenario& dinerScenario,
     LgFrWorkingImplosionProblem & dinerWip,
     LgFrLastProblemSolved & dinerLps,
     LgFrSetOfParts & dinerSop,
     LgFrSetOfDemands & dinerSod,
     LgFrFocussedShortageScheduleControl & dinerFssC,
     LgFrScenario& /* dupScenario */,
     LgFrFocussedShortageScheduleControl & dupFssC
    )
{
  // Variables needed by tests
  size_t i,t;
  float fltEps = 0.0001f;
  LgFrImplode implode1 ( dinerLps );
  LgFrImplode implode2 ( dinerLps );

  // Test FSS constructed for dinerScenario
  assert (dinerSop[0].name() == "Skillet");
  assert (  ( (*(dinerFssC.setOfPartsPtr_)) [0])  .name() == "Skillet"   );

  // Test FSS constructed by deep-copying
  assert (dupFssC.focus_.size() == dinerFssC.focus_.size() );
  dupFssC.focusHorizon( *dinerSod.findValue("demand","GrlChs_Sand"), 
                         1 );
  assert ( ( dupFssC.focus_.size() - 1 ) == dinerFssC.focus_.size() );


  // Test setting and getting demands focus horizon
  assert( dinerFssC.focusChanged() );
  dinerFssC.focusHorizon( *dinerSod.findValue("demand","GrlChs_Sand"), 
                         1 );
  assert( 
     dinerFssC.focusHorizon( *dinerSod.findValue("demand","GrlChs_Sand") )
     == 1 );
  assert( dinerFssC.focusChanged() );

  dinerFssC.focusHorizon( *dinerSod.findValue("demand","GrlChs_Sand"), 
                         0 );
  assert( 
     dinerFssC.focusHorizon( *dinerSod.findValue("demand","GrlChs_Sand") )
     == 0 );

  dinerFssC.focusHorizon( *dinerSod.findValue("demand","GrlChs_Sand"), 
                         -1 );
  assert( 
     dinerFssC.focusHorizon( *dinerSod.findValue("demand","GrlChs_Sand") )
     == -1 );

  dinerFssC.focusHorizon( *dinerSod.findValue("demand","Ham_Sand"), 
                         1 );
  assert( 
     dinerFssC.focusHorizon( *dinerSod.findValue("demand","Ham_Sand") )
     == 1 );

  assert( dinerFssC.setOfPartsPtr_->operator[](0).name() == "Skillet" );

  // Do implosion so FSS can be obtained
  dinerScenario.newImplosionSolution();

  // Test Getting focused shortage schedule
  assert ( !dinerFssC.isValid() );
  assert (  dinerFssC.focusChanged() );
  const LgFrSortingPartScheduleFloat fss = 
     dinerFssC.focussedShortageSchedule();

  const LgFrSortingPartScheduleFloat aConstFssCtrlFSS = dinerFssC.FSS_;

  assert (  dinerFssC.isValid() );
  assert ( !dinerFssC.focusChanged() );
  assert ( fss.size() > 0 );
  assert ( fss.size() == dinerFssC.FSS_.size() );
  for ( i=0; i<fss.size(); i++ ) {
     assert( fss[i] == aConstFssCtrlFSS[i] );
  }
  // dinerFssC.print();
  // Test production volume
  // std::cout << "\nINFo: Print out the production volume:" << std::endl;

  for ( i = 0; i < dinerSop.size(); i++ )
    {
      LgFrPart part = dinerSop[i];
      if ( dinerFssC.implodePtr()->lastProblemSolved().category( &part )
          == LgFrPRODUCT )
        {
          LgFrTimeVecFloat fssProdTV = 
            dinerFssC.fssProductionVolume(&part);
            // std::cout << "\nINFO: Part " << i << ": " << part.name()
            //      << fssProdTV.format() << std::endl;
          if ( i == 9 || i == 11 || i == 12 || i == 13 || i == 14 )
            {
              assert( fssProdTV[0] == 0.0);
              assert( fssProdTV[1] == 0.0);
            }
          else if ( i == 10 )
            {
              assert( fssProdTV[0] == 13.0);
              assert( fssProdTV[1] == 20.0);
            }
          else if ( i == 15 )
            {
              assert( fssProdTV[0] == 4.0);
              assert( fssProdTV[1] == 1.0);
            }
          else if ( i == 16 )
            {
              assert( fssProdTV[0] == 7.0);
              assert( fssProdTV[1] == 8.0);
            }
          else if ( i == 17 )
            {
              assert( fssProdTV[0] == 0.0);
              assert( fssProdTV[1] == 6.0);
            }
          else if ( i == 18 )
            {
              assert( fssProdTV[0] == 0.0);
              assert( fssProdTV[1] == 3.0);
            }
        }
    }
  
  assert( dinerFssC.setOfPartsPtr_->operator[](0).name() == "Skillet" );

  // Update supply by shortage
  for ( i=0; i<fss.size(); i++ ) {
     // Get current supply
     LgFrTimeVecFloat supplyVol = 
        dinerLps.supplyVolume( &(fss[i].part()) );
     // Increment supply by shortage
     for ( t=0; t<supplyVol.length(); t++ )
        supplyVol[t] += (fss[i].timeVecFloat())[t];
     // Set new supply
     dinerWip.timeVec( fss[i].part(), supplyVol );  
  }

  assert( dinerFssC.setOfPartsPtr_->operator[](0).name() == "Skillet" );

  // Do implosion so FSS can be obtained
  dinerScenario.newImplosionSolution();
  // (dinerFssC.lpsPtr_)->implode(dinerWip);

  assert( dinerFssC.setOfPartsPtr_->operator[](0).name() == "Skillet" );

  // Get focussed shortage schedule
  // FSS should be same as previous, because it was not invalidated. 
  const LgFrSortingPartScheduleFloat oldFss(fss);
  assert ( !dinerFssC.isValid() );
  const LgFrSortingPartScheduleFloat fss1 = 
     dinerFssC.focussedShortageSchedule();
  assert ( dinerFssC.isValid() );
  // Entries in fss1 should be zero because supply was increased to
  // meet all demands in focus.
  assert( fss1.size() == 0 );
  

  // Test production volume
  // std::cout << "\nPrint out the production volume after changing supply:" << std::endl;
  for ( i = 0; i < dinerSop.size(); i ++ )
    {
      LgFrPart part = dinerSop[i];
      if ( dinerFssC.implodePtr()->lastProblemSolved().category( &part )
          == LgFrPRODUCT )
        {
          LgFrTimeVecFloat fssProdTV = 
            dinerFssC.fssProductionVolume(&part);

          if ( i == 9 || i == 11 || i == 12 || i == 13 || i == 14 )
            {
              assert( fabs( fssProdTV[0] - 0.0 ) < fltEps );
              assert( fabs( fssProdTV[1] - 0.0 ) < fltEps );
            }
          else if ( i == 10 )
            {
              assert( fabs( fssProdTV[0] - 13.0 ) < fltEps );
              assert( fabs( fssProdTV[1] - 20.0 ) < fltEps );
            }
          else if ( i == 15 )
            {
              assert( fabs( fssProdTV[0] - 4.0 ) < fltEps );
              assert( fabs( fssProdTV[1] - 1.0 ) < fltEps );
            }
          else if ( i == 16 )
            {
              assert( fabs( fssProdTV[0] - 7.0 ) < fltEps );
              assert( fabs( fssProdTV[1] - 8.0 ) < fltEps );
            }
          else if ( i == 17 )
            {
              assert( fabs( fssProdTV[0] - 0.0 ) < fltEps );
              assert( fabs( fssProdTV[1] - 6.0 ) < fltEps );
            }
          else if ( i == 18 )
            {
              assert( fabs( fssProdTV[0] - 0.0 ) < fltEps );
              assert( fabs( fssProdTV[1] - 3.0 ) < fltEps );
            }

           // std::cout << "\n Part " << i << ": " << part.name() << fssProdTV.format();
        }
    }

  // Test latesetFocussedShortageSchedule() 
  const LgFrSortingPartScheduleFloat fss3 = 
     dinerFssC.latestFocussedShortageSchedule();
  assert( fss3.size() == 0 );
  dinerFssC.focusHorizon( *dinerSod.findValue("demand","GrlChs_Sand"), 
                         1 );
  assert ( dinerFssC.focusChanged() );
  dinerFssC.focusHorizon( *dinerSod.findValue("demand","HamEgg_Sand"), 
                         1 );
  dinerFssC.focusHorizon( *dinerSod.findValue("demand","HamChs_Sand"), 
                         1 );
  dinerFssC.focusHorizon( *dinerSod.findValue("demand","Veggie_Omlet"), 
                         1 );
  dinerFssC.focusHorizon( *dinerSod.findValue("demand","HamChs_Omlet"), 
                         1 );
  dinerFssC.focusHorizon( *dinerSod.findValue("demand","Chs_Omlet"), 
                         1 );
  const LgFrSortingPartScheduleFloat fss4 = 
     dinerFssC.latestFocussedShortageSchedule();
  // dinerFssC.print();
  assert( fss4.size() == 5 );


  // Test latestFSSProductionVolume
  // std::cout << "\nPrint out the production volume after changing focus:" << std::endl;
  for ( i = 0; i < dinerSop.size(); i ++ )
    {
      LgFrPart part = dinerSop[i];
      if ( dinerFssC.implodePtr()->lastProblemSolved().category( &part )
          == LgFrPRODUCT )
        {
          LgFrTimeVecFloat fssProdTV = 
            dinerFssC.latestFSSProductionVolume(&part);

          if ( i == 9 || i == 11 || i == 12 || i == 13 || i == 14 )
            {
              assert( fabs( fssProdTV[0] - 0.0 ) < fltEps );
              assert( fabs( fssProdTV[1] - 0.0 ) < fltEps );
            }
          else if ( i == 10 )
            {
              assert( fabs( fssProdTV[0] - 22.0 ) < fltEps );
              assert( fabs( fssProdTV[1] - 29.0 ) < fltEps );
            }
          else if ( i == 15 )
            {
              assert( fabs( fssProdTV[0] - 4.0 ) < fltEps );
              assert( fabs( fssProdTV[1] - 6.0 ) < fltEps );
            }
          else if ( i == 16 )
            {
              assert( fabs( fssProdTV[0] - 7.0 ) < fltEps );
              assert( fabs( fssProdTV[1] - 8.0 ) < fltEps );
            }
          else if ( i == 17 )
            {
              assert( fabs( fssProdTV[0] - 6.0 ) < fltEps );
              assert( fabs( fssProdTV[1] - 9.0 ) < fltEps );
            }
          else if ( i == 18 )
            {
              assert( fabs( fssProdTV[0] - 3.0 ) < fltEps );
              assert( fabs( fssProdTV[1] - 4.0 ) < fltEps );
            }

          // std::cout << "\nPart " << i << ": " << part.name() << fssProdTV.format();
        }
    }

  // test clearing focus horizon
  assert( 
     dinerFssC.focusHorizon( *dinerSod.findValue("demand","Chs_Omlet") )
     == 1 );
  dinerFssC.clearFocusHorizons();
  assert( 
     dinerFssC.focusHorizon( *dinerSod.findValue("demand","Chs_Omlet") )
     == -1 );
  
}
