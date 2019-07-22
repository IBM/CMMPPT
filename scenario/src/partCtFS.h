#ifndef LGFR_PART_CATEGORY_FILTER_STRATEGY_H
#define LGFR_PART_CATEGORY_FILTER_STRATEGY_H
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

#include <scenario/src/scenSnFS.h>
#include <scenario/src/part.h>

// Filter strategy for parts based on part's category (LgFrPartCategory).

class LgFrScenario;

class LgFrPartCategoryFilterStrategy : public LgFrScenarioSensitiveFilterStrategy {
public:

  // Return true if itemTimeVec is not of the category represented by 
  // this strategy.
  // This base class always returns false and does not filter any
  // itemTimeVecs from the schedule.
  virtual
    bool
      exclude(
        const LgFrItemTimeVecPairFloat &)
          const;

  // Get the LgFrPartCategory associated with this filter
  LgFrPartCategory
     partCategory()
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

  // Preferred Constructor
  LgFrPartCategoryFilterStrategy (
          const LgFrPartCategory & pc,
	  const LgFrScenario & scenario);

  // Default Constructor
  LgFrPartCategoryFilterStrategy ();

  // Assignment operator
  LgFrPartCategoryFilterStrategy &
    operator=(
	      const LgFrPartCategoryFilterStrategy& rhs);

  // Copy constructor
  LgFrPartCategoryFilterStrategy (
     const LgFrPartCategoryFilterStrategy &);

  // Copy constructor with new Scenario
  LgFrPartCategoryFilterStrategy (
     const LgFrPartCategoryFilterStrategy &,
     LgFrScenario &);

  // Destructor
  ~LgFrPartCategoryFilterStrategy ();

  // test this class
  static void test();

  static void contextTest(LgFrScenario &);

private:

  LgFrPartCategory partCategory_;
  const int id_;

};

#endif
