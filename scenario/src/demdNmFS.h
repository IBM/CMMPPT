#ifndef LGFR_DEMAND_NAME_FILTER_STRATEGY_H
#define LGFR_DEMAND_NAME_FILTER_STRATEGY_H
// RW2STL -- inserted:
#include <string>
// End of RW2STL -- inserted includes.
#include <scenario/src/filtStrt.h>

// Filter strategy for demands based on DemandName
// This filter takes RWCRegexp's as a filter.  You must be 
// careful using it.  For example the filter --->
// LgFrDemandNameFilterStrategy(".", "US") will not exclude
// a demand with demandName="WITUSE".  If you want an exact match, you must
// specify it as LgFrDemandNameFilterStrategy(".", "^US$").  See
// RW documentation for RWCRegexp ...

class LgFrDemandNameFilterStrategy : public LgFrFilterStrategy {
public:

  virtual int GetFSType() const { return __LGFRDEMANDNAMEFILTERSTRATEGY; }

  // Return true if itemTimeVec is not of the type represented by 
  // this strategy.
  // This base class always returns false and does not filter any
  // itemTimeVecs from the schedule.
  virtual
    bool
      exclude(
        const LgFrItemTimeVecPairFloat &)
          const;

  // Get the Part Regexp as a std::string associated with this Filter
  std::string
     partRE()
        const;

  // Get the Demand Regexp as a std::string associated with this Filter
  std::string
     demandRE()
        const;

  // Get the Part Regexp associated with this DemandName Filter
  std::string
     partRegexp()
        const;

  // Get the Demand Regexp associated with this DemandName Filter
  std::string
     demandRegexp()
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
  LgFrDemandNameFilterStrategy (
          const std::string & pREstring,
          const std::string & dREstring);

  // Default Constructor
  LgFrDemandNameFilterStrategy ();

  // Assignment operator
  LgFrDemandNameFilterStrategy &
    operator=(
	      const LgFrDemandNameFilterStrategy& rhs);

  // Copy constructor
  LgFrDemandNameFilterStrategy (
     const LgFrDemandNameFilterStrategy &);

  // Copy constructor with new Scenario
  LgFrDemandNameFilterStrategy (
     const LgFrDemandNameFilterStrategy &,
     LgFrScenario &);

  // Destructor
  ~LgFrDemandNameFilterStrategy ();

  // test this class
  static void test();

private:

  const int id_;
  std::string  partREstring_;
  std::string  demandREstring_;
  std::string  partRegexp_;
  std::string  demandRegexp_;


};

#endif
