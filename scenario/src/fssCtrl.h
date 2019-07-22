#ifndef LGFR_FOCUSSED_SHORTAGE_SCHEDULE_CONTROL_H
#define LGFR_FOCUSSED_SHORTAGE_SCHEDULE_CONTROL_H
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.
// Author: JP 

// This class provides the client with a focused shortage schedule
// when given a focus horizon and an implosion solution.
// It also provides the fss production schedule.

#include <scenario/src/algorthm.h>
#include <scenario/src/partSchd.h>
#include <scenario/src/demdPerd.h>

// Forward References
class LgFrSetOfDemands;
class LgFrSetOfParts;
class LgFrImplode;


class LgFrFocussedShortageScheduleControl : public LgFrAlgorithm {

public:

  int virtual GetAlgorithmType() const { return __LGFRFOCUSSEDSHORTAGESCHEDULECONTROL; }

  // localUpdate - This method is invoked when the algorthm solution has
  // been updated.
  virtual
     void
        localUpdate (
           LgFrModel * p,
           void * d );

  // Set a demands focus horizon
  virtual
    void
      focusHorizon
         ( const LgFrDemand demand,
           const int        focusHorizon );

  // Get a demands focus horizon
  virtual
    int
      focusHorizon
         ( const LgFrDemand demand )
            const;

  // Remove all demands from the focus horizon
  virtual
     void
        clearFocusHorizons();

  // Get focused shortage schedule
  //  1. An implosion solution needs to exist.
  //  2. The first time invoked the focussedShortageSchedule is
  //     computed.  Subsequent calls return the prior 
  //     focussedShortageSchedule unless markInvalid() is invoked.
  //     This method will compute a new focussedShortageSchedule if
  //     the current schedule has been invalidated by calling markInvalid.
  virtual
    const LgFrSortingPartScheduleFloat &
      focussedShortageSchedule();

  // Get latest focused shortage schedule
  //  1. An implosion solution needs to exist.
  //  2. If an implossion solution has previously been returned and
  //     the focus has not changed and markInvalid() has not been invoked
  //     then the previously computed schedule is returned.  If markInvalid()
  //     or the focus has changed, then a new schedule is returned. 
  virtual
     const LgFrSortingPartScheduleFloat &
        latestFocussedShortageSchedule();

  // Get focused shortage schedule's production time vector float for a part
  //  1. An implosion solution needs to exist.
  //  2. The focussedShortageSchedule has to be valid.
  //     This method will compute a new focussedShortageSchedule if
  //     the current schedule has been invalidated by calling markInvalid.
  virtual
    LgFrTimeVecFloat 
      fssProductionVolume(const LgFrPart * const partPtr);

  // Get latest focused shortage schedule's production time vector float for
  // a part.
  //  1. An implosion solution needs to exist.
  //  2. If an implossion solution has previously been returned and
  //     the focus has not changed and markInvalid() has not been invoked
  //     then return the production volume based on the previously computed
  //     FSS schedule.  If the focus has changed, then a new production
  //     volume based on the new FSS schedule is returned. 
  virtual
     LgFrTimeVecFloat 
        latestFSSProductionVolume(const LgFrPart * const partPtr);

  // similar to latestFocussedShortageSchedule but does not return
  //  the schedule
  virtual
     void
        updateFocussedShortageSchedule();

  
  // Mark focused shortage schedule as invalid.  This should be
  // when a new implosion solution is computed.
  virtual
     void
        markInvalid();

  // Returns true if FocussedShortageScheduleControl contains a valid FSS.
  virtual
     bool
        isValid()
           const;

  // Returns true if the focus has changed (focus(demand,period))
  // since the last time a focused shortage schedule was obtained.
  // If a focus shortage schedule has not yet been obtained, true is returned.
  virtual
     bool
        focusChanged()
           const;

  // Copies source's data without changing the relationship between *this
  // and the object it subscripes to (LgFrImplode) and that subscribe to it.
  void
  virtual
  copyInPlace (const LgFrFocussedShortageScheduleControl & source);
  
  // Return pointer to the fssCtrl algorithm in the specified scenario.
  virtual
     LgFrAlgorithm *
        counterPart(
           LgFrScenario & )
              const;

    // Return pointer to the implode algorithm 
  virtual
     LgFrImplode *
        implodePtr() const;

  // Constructor.
   LgFrFocussedShortageScheduleControl(
       LgFrImplode           & implode,
       LgFrSetOfParts        & mySetOfParts,
       LgFrSetOfDemands      & mySetOfDemands);

   // Another Constructor.  Deep copy constructor.
   LgFrFocussedShortageScheduleControl(
      const LgFrFocussedShortageScheduleControl & fssCtrl,
      LgFrImplode & implode,
      LgFrSetOfParts& mySetOfParts,
      LgFrSetOfDemands& mySetOfDemands);
  
  // destructor
  ~LgFrFocussedShortageScheduleControl();
  
  // Print FocussedShortageScheduleControl data
  virtual void print() const;
  
  // test this class
  static void test();

  // test this class from an owner of a couple of instances
  // This method expects the data for the first argument (scenario) to have come
  // from /u/implode/wit/data/examples/diner12/wit.data.
  // It expects that scenarios WIP, LPS, SOP, SOD, and FSS-Ctrl to be passed in the
  // 2nd - 6th parameters
  // It expects the 7th parameter to have been created by deep-copying the
  // first scenario and expects the 8th parameter to be that scenarios FSS-Ctrl.
  static
  void
  contextTest(
              LgFrScenario&  dinerScenario,	  // Uses diner12 data
              LgFrWorkingImplosionProblem & dinerWip,
              LgFrLastProblemSolved & dinerLps,
              LgFrSetOfParts & dinerSop,
              LgFrSetOfDemands & dinerSod,
              LgFrFocussedShortageScheduleControl & dinerFssC,
              LgFrScenario&  dupScenario,   // Deep-copied dinerScenario
              LgFrFocussedShortageScheduleControl & dupFssC);

private:

  LgFrDemandPeriodPairVector        focus_;
  LgFrSortingPartScheduleFloat             FSS_;

  bool validFSS_;
  bool focusChanged_;
  
  LgFrImplode            * implodePtr_;
  const LgFrSetOfParts   * setOfPartsPtr_;
  const LgFrSetOfDemands * setOfDemandsPtr_;

  const int id_;


  // default constructor
  LgFrFocussedShortageScheduleControl();

  // Copy constructor
  LgFrFocussedShortageScheduleControl (
     const LgFrFocussedShortageScheduleControl& source);
  
  // Assignment 
  LgFrFocussedShortageScheduleControl& 
    operator=(const LgFrFocussedShortageScheduleControl& rhs);

  // Functions
  int findDemandIndex( LgFrDemand d ) const;
  void copyFocusMember (const LgFrFocussedShortageScheduleControl& source);
  void giveWitInputForFocusHorizons();
  void giveWitInputForFssShipVol();
};

#endif
