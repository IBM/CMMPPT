#ifndef LGFR_FILTER_STRATEGY_H
#define LGFR_FILTER_STRATEGY_H
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.


#include <scenario/src/model.h>
#include <scenario/src/idnumber.h>


// Base Class for implementing Filtering Strategies.
// Filtering strategies are used by a schedule strategy when populating
// a schedule.

// The filter allows one to exclude an itemTimeVec from the schedule
// being populated.

// Forward References
class LgFrItemTimeVecPairFloat;
class LgFrScenario;
class LgFrAlgorithm;
class LgFrSetOfParts;
class LgFrLastProblemSolved;

class LgFrFilterStrategy :
   public LgFrSubscription
{
public:

  virtual int GetFSType() const { return __LGFRFILTERSTRATEGY; }

  // Return true if itemTimeVec is not of the type represented by 
  // this strategy.
  // This base class always returns false and does not filter any
  // itemTimeVecs from the schedule.
  virtual
    bool
      exclude(
        const LgFrItemTimeVecPairFloat &)
          const;

  // ModelClient updateFrom method
/*
  virtual
     void
        localUpdate (
           LgFrModel * p,
           void * );
*/

  // Make clone copy of this object
  virtual
     LgFrFilterStrategy *
        clone() 
           const;

  // Make clone copy of this object with new Scenario
  virtual
     LgFrFilterStrategy *
        clone(LgFrScenario &) 
           const;

  // Assignment operator
  LgFrFilterStrategy &
    operator=(
	      const LgFrFilterStrategy& rhs);

  // Copy constructor
  LgFrFilterStrategy (
     const LgFrFilterStrategy &);

  // Copy constructor with a new Scenario
  LgFrFilterStrategy (
     const LgFrFilterStrategy &,
     LgFrScenario & );

  // Default Constructor
  LgFrFilterStrategy (
     LgFrAlgorithm * algorithmPtr = 0 );

  // Destructor
  virtual ~LgFrFilterStrategy ();

  // test this class
  static void test();

  // see the comment in schdStrt.h.
  /////protected:

   // Get a set of parts from scenario.
   static
      const LgFrSetOfParts &
         setOfParts(
            const LgFrScenario *,
            int setOfPartsIdentifier );

  // Return the LPS which belongs to a scenario
  const LgFrLastProblemSolved&
     lastProblemSolved(
        const LgFrScenario &)
        const;

private:

  LgFrAlgorithm * algorithmPtr_;
  const int id_;

};

#endif
