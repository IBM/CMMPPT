#ifndef LGFR_SCHEDULE_STRATEGY_H
#define LGFR_SCHEDULE_STRATEGY_H

#include <scenario/src/model.h>

#include <scenario/src/idnumber.h>

// Base Class for implementing Schedule Strategies used for populating
// a schedule by LgFrSortingSchedule

// This class follows the Strategy Pattern

// for checking user input in GUI
// Defaults to NOTKNOWN (which should usually be treated as if VALID)
// (Note that, on OS/2, UNKNOWN is changed to "7" by math.h or related
// functionality, which makes it a bad choice for a type.)
enum LgFrValidity { VALID, INVALID, NOTKNOWN };

// Forward references
class LgFrScenario;
class LgFrSetOfParts;
class LgFrSetOfDemands;
class LgFrWorkingImplosionProblem;
class LgFrLastProblemSolved;
class LgFrAlgorithm;
class LgFrImplode;
class LgFrExplode;
class LgFrUserInputModification;
class LgFrFocussedShortageScheduleControl;
class LgFrSortingScheduleFloat;
class LgFrSortingScheduleFloatSmartPointer;
class LgFrTimeVecFloat;
class LgFrItemTimeVecPairFloat;


class LgFrAttributeScheduleStrategy :
   public LgFrSubscription
{
public:

  // Populate Schedule with itemTimeVecs
  virtual
     void
        populate(
           LgFrSortingScheduleFloat& )
              const;

  // This is the kernal of the populate method which must be written
  // by the derived classes.  It must create the itemTimeVec, check
  // to see if the itemTimeVec is to be filtered out, and insert
  // the itemTimeVec into the schedule.
  virtual
     void
        kernalOfPopulate(
           LgFrSortingScheduleFloat& )
              const
                = 0;

  // Print an error message and go on.  
  // If *this is mutable, this method has to be overriden by a method
  // in the subclass to propate a change in the sortingSchedule into the
  // appropriate parts of the underlying system, e.g. workingImplosionProblem.
  virtual
     void 
        update(
            const LgFrItemTimeVecPairFloat &);
          
  // isMutable
  // Default implementation of isMutable always returns false.  Override
  // in the derived class for schedule Strategies which are user-mutable.
  virtual
     bool
        isMutable()
           const;

  // Validity - {VALID, INVALID, UNKNOWN} - entire timeVec of float& values
  // Default implementation of Validity always returns true.  Override
  // in the derived class for Schedule Strategies with validity ranges.
  virtual
     LgFrValidity
        validity(
           const LgFrTimeVecFloat&)
              const;

  // ModelClient updateFrom method - use the default

  // Make clone copy of this object
  virtual
     LgFrAttributeScheduleStrategy *
        clone() 
           const
              = 0;

  // Make clone copy of this object with a new Scenario
  virtual
     LgFrAttributeScheduleStrategy *
        clone(LgFrScenario &) 
           const
              = 0;


  // Assignment operator
  LgFrAttributeScheduleStrategy &
    operator=(
	      const LgFrAttributeScheduleStrategy& rhs);

  // Copy constructor associated with a newScenario
  LgFrAttributeScheduleStrategy (
     const LgFrAttributeScheduleStrategy &,
     LgFrScenario & );

  // Copy constructor 
  LgFrAttributeScheduleStrategy (
     const LgFrAttributeScheduleStrategy &);

  // Constructor
  LgFrAttributeScheduleStrategy (
     LgFrAlgorithm * algorithmPtr = 0 );

  // Destructor
  virtual
     ~LgFrAttributeScheduleStrategy ();

  // test this class
  static void test();

  ////??????
  // in scenario.h, class LgFrScenario declares (most of) the
  // following methods to be "friend"s.
  // I (vra) can't see how that would have ever been possible,
  // given that these are protected methods.
  // Perhaps the compilers are just more finicky now.
  // As a hack to get this working right away, I will remove the
  // protected keyword.
  //
  // An an alternative, one could add
  //    friend class LgFrAttributeScheduleStrategy;
  // to the classes that need access (such as LgFrScenario)
  // and make these methods protected again.
  //
  // The differences between these two styles is:
  //	dropping "protected" here allows everyone access,
  //	(which presumably isn't intended),
  //	but denies friends (such as LgFrScenario) access to private members
  //	(which presumably is intended).
  //
  //	in contrast, using friend class LgFrAttributeScheduleStrategy
  //	(and putting back the "protected" keyword here)
  //	allows the friend classes access to private members
  //	(presumably not intended),
  //	but denies everyone else access to protected members
  //	(presumably intended).
  //
  ////// protected:

  // Estimate size of schedule using this ass.
  // The populate method uses this to set an initial size of the
  // schedule.
  virtual
     size_t
        estimatedSize(
           const LgFrSortingScheduleFloat& )
              const;

  // Return the setOfParts which belongs to a scenario
  LgFrSetOfParts&
     setOfParts(
        LgFrScenario &);

  // Return the setOfDemands which belongs to a scenario
  LgFrSetOfDemands&
     setOfDemands(
        LgFrScenario &);

  // Return the WorkingImplosionProblem which belongs to a scenario
  LgFrWorkingImplosionProblem&
     workingImplosionProblem(
        LgFrScenario &);

  const LgFrWorkingImplosionProblem&
     workingImplosionProblem(
        const LgFrScenario &)
        const;

  // Return the LastProblemSolved which belongs to a scenario
  LgFrLastProblemSolved&
     lastProblemSolved(
        LgFrScenario &);

  const LgFrLastProblemSolved&
     lastProblemSolved(
        const LgFrScenario &)
        const;

  // Return the Implode which belongs to a scenario
  LgFrImplode&
     implode(
        LgFrScenario &);

  // Return the Implode which belongs to a scenario
  LgFrExplode&
     explode(
        LgFrScenario &);

  // Return the User Input Modification which belongs to a scenario
  LgFrUserInputModification&
     userInputModification(
        LgFrScenario &);
  
  // Return the focussed shortage schedule which belongs to a scenario
  LgFrFocussedShortageScheduleControl&
     focussedShortageScheduleControl(
        LgFrScenario &);

private:

  LgFrAlgorithm * algorithmPtr_;  
                            // Pointer to algorithm that Schedule strategy
                            // depends on.  If there are no algorithm
                            // dependencies then algorithmPtr_ is nul


};

#endif
