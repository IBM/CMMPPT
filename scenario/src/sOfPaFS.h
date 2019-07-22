#ifndef LGFR_SET_OF_PARTS_FILTER_STRATEGY_H
#define LGFR_SET_OF_PARTS_FILTER_STRATEGY_H

#include <scenario/src/scenSnFS.h>

// Filter strategy for including items contained in a set of parts or demands.

// Forward references
class LgFrSetOfParts;

class LgFrSetOfPartsFilterStrategy : public LgFrScenarioSensitiveFilterStrategy {
public:

  // Return TRUE if TimeVec contains all zeros
  virtual
    RWBoolean
      exclude(
        const LgFrItemTimeVecPairFloat &)
          const;

  // Set the scenario and setOfPartsIdentifier to be used when filtering.
  virtual
     void
        setOfPartsIdentifier(
           const LgFrScenario &,
           int setOfPartsIdentifier );

  // Get setOfPartsIdentifier containing values to be used when filtering
  virtual
     int 
        setOfPartsIdentifier()
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

  // Assignment operator
  LgFrSetOfPartsFilterStrategy &
    operator=(
	      const LgFrSetOfPartsFilterStrategy& rhs);

  // Copy constructor
  LgFrSetOfPartsFilterStrategy (
     const LgFrSetOfPartsFilterStrategy &);

  // Copy constructor
  LgFrSetOfPartsFilterStrategy (
     const LgFrSetOfPartsFilterStrategy &, 
     LgFrScenario &);

  // Default Constructor
  LgFrSetOfPartsFilterStrategy ();

  // Destructor
  ~LgFrSetOfPartsFilterStrategy ();

  // test this class
  static void test();
  static void contextTest( 
     LgFrScenario &,
     LgFrSetOfParts & univSop );

private:

  int setOfPartsId_;
  const int id_;

};

#endif
