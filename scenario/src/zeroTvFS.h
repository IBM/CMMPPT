#ifndef LGFR_ZERO_TIME_VEC_FILTER_STRATEGY_H
#define LGFR_ZERO_TIME_VEC_FILTER_STRATEGY_H
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

#include <scenario/src/filtStrt.h>

// Filter strategy for excluding itemTimeVec's with timeVecs == 0.0.

class LgFrZeroTimeVecFilterStrategy : public LgFrFilterStrategy {
public:

  virtual int GetFSType() const { return __LGFRZEROTIMEVECFILTERSTRATEGY; }

  // Return true if TimeVec contains all zeros
  virtual
    bool
      exclude(
        const LgFrItemTimeVecPairFloat &)
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

  // Assignment operator
  LgFrZeroTimeVecFilterStrategy &
    operator=(
	      const LgFrZeroTimeVecFilterStrategy& rhs);

  // Copy constructor
  LgFrZeroTimeVecFilterStrategy (
     const LgFrZeroTimeVecFilterStrategy &);

  // Copy constructor
  LgFrZeroTimeVecFilterStrategy (
     const LgFrZeroTimeVecFilterStrategy &, 
     LgFrScenario &);

  // Default Constructor
  LgFrZeroTimeVecFilterStrategy ();

  // Destructor
  ~LgFrZeroTimeVecFilterStrategy ();

  // test this class
  static void test();

private:

  const int id_;
};

#endif
