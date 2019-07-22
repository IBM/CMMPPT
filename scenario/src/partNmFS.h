#ifndef LGFR_PART_NAME_FILTER_STRATEGY_H
#define LGFR_PART_NAME_FILTER_STRATEGY_H

#include <rw/regexp.h>
#include <rw/cstring.h>
#include <scenario/src/filtStrt.h>

// Filter strategy for parts based on partName.
// This filter strategy matches partNames with RWCRegexp's.  You must be 
// careful using it.  For example the filter --->
// LgFrPartNameFilterStrategy("US") will not exclude
// a part with partName="WITUSE".  If you want an exact match, you must
// specify it as LgFrPartNameFilterStrategy("^US$").  See
// RW documentation for RWCRegexp ...

class LgFrPartNameFilterStrategy : public LgFrFilterStrategy {
public:

  // Return TRUE if itemTimeVec is not of the type represented by 
  // this strategy.
  // This base class always returns FALSE and does not filter any
  // itemTimeVecs from the schedule.
  virtual
    RWBoolean
      exclude(
        const LgFrItemTimeVecPairFloat &)
          const;

  // Get the Part Regexp as a RWCString associated with this Filter
  RWCString
     partRE()
        const;

  // Get the Part Regexp associated with this DemandName Filter
  RWCRegexp
     partRegexp()
        const;

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

  // Return Filter Strategy Identifier
  virtual
     isAReturnType
        isA()
           const;

  // Preferred Constructor
  LgFrPartNameFilterStrategy (
          const RWCString & pREstring );

  // Default Constructor
  LgFrPartNameFilterStrategy ();

  // Assignment operator
  LgFrPartNameFilterStrategy &
    operator=(
	      const LgFrPartNameFilterStrategy& rhs);

  // Copy constructor
  LgFrPartNameFilterStrategy (
     const LgFrPartNameFilterStrategy &);

  // Copy constructor with new Scenario
  LgFrPartNameFilterStrategy (
     const LgFrPartNameFilterStrategy &,
     LgFrScenario &);

  // Destructor
  ~LgFrPartNameFilterStrategy ();

  // test this class
  static void test();

private:

  const int id_;
  RWCString  partREstring_;
  RWCRegexp  partRegexp_;


};

#endif
